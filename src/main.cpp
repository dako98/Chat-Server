#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <unordered_map>

#include "User.hpp"
#include "UserStore.hpp"
#include "Message.hpp"
#include "MessageBuilder.hpp"
#include "ChatHistory.hpp"
#include "HistoryStore.hpp"

#include "MessageSending.hpp"
#include "MessageReceiving.hpp"

#include "ThreadsafeQueue.h"
#include "Codes.hpp"

#define debug

const int PORT = 42123;

using boost::asio::ip::tcp;

bool reservedName(const std::string &name)
{
    const std::array<const std::string, 6> reservedNames = {"server",
                                                            "register",
                                                            "history", 
                                                            "login", 
                                                            "chat",
                                                            " "};
    bool reserved = false;
    for (auto &&i : reservedNames)
    {
        if (name == i)
        {
            reserved = true;
            break;
        }
    }
    return reserved;
}

bool authenticate(const Message &login)
{
    bool valid = false;

    UserStore *users = &UserStore::getInstance();

    if (login.getSender() != " " &&
        login.getContents() != " " &&
        users->getUser(login.getSender()).comparePassword(login.getContents()))
    {
        users->getUser(login.getSender()).online = true;
        valid = true;
    }
    return valid;
}

bool registerUser(tcp::socket &socket, const Message &receivedMessage)
{
    MessageBuilder builder;
    UserStore *users = &UserStore::getInstance();
    bool continueConnection = false;

    std::string name, password;
    name = receivedMessage.getSender();
    password = receivedMessage.getContents();

    if (!reservedName(name) &&
        users->getUser(name) == User{})
    {
        users->addUser(User{name, password});
        builder.setReceiver(name);
        builder.setSender("server");
        builder.setMessage(" ");

        Message response = builder.build();
        sendMessage(socket, response);
        continueConnection = true;
    }
    else
    {
        builder.setReceiver(name);
        builder.setSender("server");
        builder.setMessage("Name not allowed.");

        Message response = builder.build();
        sendMessage(socket, response);
        continueConnection = false;
    }
    return continueConnection;
}

bool giveChat(tcp::socket &socket, const Message &receivedMessage)
{
    MessageBuilder builder;
    UserStore *users = &UserStore::getInstance();
    bool continueConnection = false;

    if (users->getUser(receivedMessage.getContents()).online)
    {

        builder.setReceiver(receivedMessage.getSender());
        builder.setSender("server");
        builder.setMessage(" ");

        Message response = builder.build();

        sendMessage(socket, response);
        continueConnection = true;
    }
    else
    {
        builder.setReceiver(receivedMessage.getSender());
        builder.setSender("server");
        builder.setMessage("User unavailable.");

        Message response = builder.build();

        sendMessage(socket, response);
        //FIXME: properly handle unknown user as chat recipient
        continueConnection = true;
    }
    return continueConnection;
}

bool giveHistory(tcp::socket &socket, const Message &receivedMessage)
{
    MessageBuilder builder;
    UserStore *users = &UserStore::getInstance();
    bool continueConnection = false;

    HistoryStore *historyStore = &HistoryStore::getInstance();
    ChatHistory history;

    history = historyStore->getChat({receivedMessage.getSender(), receivedMessage.getContents()});
    std::vector<Message> messages = history.getHistory();

    builder.setReceiver(receivedMessage.getSender());
    builder.setSender("server");
    builder.setMessage(std::to_string(messages.size()));

    Message response = builder.build();

    sendMessage(socket, response);

    for (auto &&message : messages)
    {
        sendMessage(socket, message);
    }
    continueConnection = true;

    return continueConnection;
}

bool loginUser(tcp::socket &socket, const Message &receivedMessage)
{
    bool continueConnection = false;
    if (authenticate(receivedMessage))
    {
        UserStore::getInstance().getUser(receivedMessage.getSender()).online = true;

        MessageBuilder builder;

        builder.setReceiver(receivedMessage.getSender());
        builder.setSender("server");
        builder.setMessage(" ");

        Message response = builder.build();

        sendMessage(socket, response);

        continueConnection = true;
    }
    return continueConnection;
}

bool initialConnectionHandler(tcp::socket &socket, std::string &authenticatedUser,
                              std::string &recipientUser, bool &hasHistory)
{
    bool continueConection = false;

    Message receivedMessage;

    messageReceiverV2(socket, receivedMessage);

    // User wants to register
    if (receivedMessage.getReceiver() == "register")
    {
        continueConection = registerUser(socket, receivedMessage);
    }
    // User wants to login
    else if (receivedMessage.getReceiver() == "login")
    {
        continueConection = loginUser(socket, receivedMessage);
        if (continueConection)
        {
            authenticatedUser = receivedMessage.getSender();
        }
    }
    // User wants to chat with a user
    else if (receivedMessage.getReceiver() == "chat")
    {
        continueConection = giveChat(socket, receivedMessage);
        if (continueConection)
        {
            recipientUser = receivedMessage.getContents();
        }
    }
    // User wants history
    else if (receivedMessage.getReceiver() == "history")
    {
        continueConection = giveHistory(socket, receivedMessage);
        if (continueConection)
        {
            hasHistory = true;
        }
    }
    // Unknown initial message
    else
    {
        std::cout << "Received unknown message format: \"" << receivedMessage << "\".\n";
        continueConection = false;
    }

    return continueConection;
}

unsigned int handleReceivedMessage(const Message &receivedMessage,
                                   std::unordered_map<std::string, ThreadSafeQueue<Message>> &sharedMessagePool,
                                   const std::string &thisClient, const std::string &recipient);

void startAsyncReceiver(tcp::socket &socket,
                        std::unordered_map<std::string, ThreadSafeQueue<Message>> &sharedMessagePool,
                        const std::string thisClient,
                        const std::string recipient,
                        unsigned int &statusCode)
{
    Message receivedMessage;
    unsigned int messageCode;
    for (;;)
    {
        //        receiveMessage(socket, receivedMessage);
        messageReceiverV2(socket, receivedMessage);

#ifdef debug
        std::cout << "Received message in read thread: " << receivedMessage << "\n";
#endif

        //messageQueue.waitAndPush(receivedMessage);
        messageCode = handleReceivedMessage(receivedMessage, sharedMessagePool, thisClient, recipient);
        if (messageCode != StatusCodes::OK)
        {
            statusCode = messageCode;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

unsigned int handleReceivedMessage(const Message &receivedMessage,
                                   std::unordered_map<std::string, ThreadSafeQueue<Message>> &sharedMessagePool,
                                   const std::string &thisClient, const std::string &recipient)
{
    unsigned int status = validateMessage(receivedMessage, thisClient, recipient);
    if (status == StatusCodes::OK)
    {
        // FIXME: Check where the history gets lost
        HistoryStore::getInstance().appendHistory(
            {thisClient, recipient},
            std::vector<Message>{receivedMessage});

        // route message to recipient queue
        sharedMessagePool[receivedMessage.getReceiver()].waitAndPush(receivedMessage);
    }
    return status;
}

void connection(tcp::socket &&socket,
                std::unordered_map<std::string, ThreadSafeQueue<Message>> &sharedMessagePool)
{
#ifdef debug
    // connection info
    std::cout << "got connection from: "
              << " " << socket.remote_endpoint().address().to_string()
              << " port " << socket.remote_endpoint().port() << std::endl;
#endif

    UserStore *users = &UserStore::getInstance();
    std::string currentUserName = "";
    std::string currentRecipientName = "";
    bool hasHistory = false;
    bool valid = true;
    bool readThreadCreated = false;
    unsigned int status = StatusCodes::OK;
    std::thread readThread;
    while (valid)
    {

        valid = initialConnectionHandler(socket, currentUserName, currentRecipientName, hasHistory);

        if (!valid)
        {
            //            return;
        }

        
        // send/receive loop
        while (users->getUser(currentUserName).online && /*currentRecipientName != "" &&*/ hasHistory) // figure out a better way to get user consistently
        {
            // receive message.
            // FIXME: Users can only send one message at a time.
            if (!readThreadCreated)
            {
                readThread = std::thread(startAsyncReceiver,
                                         std::ref(socket),
                                         std::ref(sharedMessagePool),
                                         (currentUserName),
                                         (currentRecipientName),
                                         std::ref(status)); // if the status changes, the thread has already stopped
                readThreadCreated = true;
            }

            switch (status)
            {
            case StatusCodes::OK:
                // all good
                break;

            case StatusCodes::WRONG_SENDER:
                // terminate session
                hasHistory = false;
                break;

            case StatusCodes::WRONG_RECEIVER:
                currentRecipientName = "";
                break;

            case StatusCodes::WRONG_CONTENTS:
                // discard packet
                continue;
                break;

            case StatusCodes::TERMINATED:
                // session ended
                return;

            default:
                throw std::exception(); // something is really wrong
                break;
            }

            // receive messages for client

            std::vector<Message> messagesToClient;
            // this is to ensure that we don't get stuck in receiving messages.
            int count = sharedMessagePool[currentUserName].size();
            messagesToClient.reserve(count);
            for (int i = 0; i < count; ++i)
            {
                messagesToClient.push_back(sharedMessagePool[currentUserName].waitAndPop());
            }

            // send message to client
            messageSenderV2(socket, messagesToClient);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        hasHistory = false;

        if (status != StatusCodes::OK)
        {
            // TODO: join message receiver thread and clear queue.
            readThread.join();
        }
    }
}

int main()
{
    std::unordered_map<std::string, ThreadSafeQueue<Message>> sharedMessagePool;

    // Not sure if good practice.
    UserStore *users = &UserStore::getInstance();
    HistoryStore *messageStore = &HistoryStore::getInstance();
    User server{"server", " "};

    User testUser1("client1", "pass1");
    User testUser2("client2", "pass2");

    users->addUser(server);
    users->addUser(testUser1);
    users->addUser(testUser2);

    try
    {
        boost::asio::io_service io_service;

        tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), PORT);
        //tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 13);
        tcp::acceptor acceptor(io_service, endpoint);
        std::vector<std::thread> threads;
        while (true)
        {
            tcp::socket socket(io_service);

            std::cout << "waiting for connection on " << acceptor.local_endpoint().address().to_string()
                      << ":" << acceptor.local_endpoint().port() << std::endl;
            acceptor.accept(socket);

            //            threads.push_back(std::thread(handleSocketConnection, std::move(socket)));
            threads.push_back(std::thread(connection, std::move(socket), std::ref(sharedMessagePool)));
        }

        for (auto &t : threads)
        {
            t.join();
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
