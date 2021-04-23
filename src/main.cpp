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
    const std::array<const std::string, 4> reservedNames = {"server", "register", "history", "login"};
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

/* TODO: Have 2 threads, one for receiving messages and one for sending.
They share a message queue. The messages always have a recipient name*/

/* TODO:
1. Read and write to console.
2. Authenticate
*/
void handleSocketConnection(tcp::socket &&socket)
{
    // Initialising connection
    std::cout << "got connection from: "
              << " " << socket.remote_endpoint().address().to_string()
              << " port " << socket.remote_endpoint().port() << std::endl;

    // Getting local variables
    UserStore *users = &UserStore::getInstance();
    std::string providedName, providedPassword;
    User thisUser = users->getUser(providedName);

    ThreadSafeQueue<Message> messageQueue;

    std::string readBufferStr;
    auto readBuffer = boost::asio::buffer(readBufferStr);

    // Chat initialisation
    MessageBuilder initialMessageBuilder;
    initialMessageBuilder.setAll(socket);
    Message initialMessage = initialMessageBuilder.build();

    if (users->getUser(initialMessage.getSender()).comparePassword(initialMessage.getContents()))
    {
        /* code */
    }

    bool chatting = true;

    while (chatting)
    {
        messageReceiver(socket, messageQueue);

        // print messages

        // read messages to send

        messageSender(socket, messageQueue);
    }

    std::cout
        << "writing" << std::endl;

    std::string reply = "socket write - hello world!";

    // Attempting to read from connected client
    std::vector<Message> history;

    boost::system::error_code error;
    while (!error)
    {
        std::string buffer;
        buffer.resize(8);
        size_t readLength = socket.read_some(boost::asio::buffer(buffer), error);
        std::cout << "read " << readLength << ": \"" << buffer.substr(0, readLength) << "\"" << std::endl;
    }

    auto replyBuffer = boost::asio::buffer(reply);

    // simulate a long write/startup without having to actually code it (for example purposes, so that we can see the main thread keeps accepting connections)
    std::this_thread::sleep_for(std::chrono::seconds(10));

    boost::asio::write(socket, replyBuffer); // shorthand for loop doing socket.write_some()

    std::cout << "closing "
              << socket.local_endpoint().address().to_string() << ":" << socket.local_endpoint().port()
              << "<->"
              << socket.remote_endpoint().address().to_string() << ":" << socket.remote_endpoint().port()
              << std::endl;
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

bool initialConnectionHandler(tcp::socket &socket, std::string &authenticatedUser)
{
    bool continueConection = false;

    UserStore *users = &UserStore::getInstance();

    Message receivedMessage;

    Message authenticationMessage;

    messageReceiverV2(socket, receivedMessage);

    // User wants to register
    if (receivedMessage.getReceiver() == "register")
    {
        std::string name, password;
        name = receivedMessage.getSender();
        password = receivedMessage.getContents();

        if (!reservedName(name) &&
            users->getUser(name) == User{})
        {
            users->addUser(User{name, password});
            sendMessage(socket, {name, "server", " "});
        }
        else
        {
            sendMessage(socket, {name, "server", "Name not allowed."});
            continueConection = false;
        }
    }
    // User wants to login
    else if (receivedMessage.getReceiver() == "login")
    {
        if (authenticate(receivedMessage))
        {
            authenticatedUser = receivedMessage.getSender();
            continueConection = true;
        }

    }
    // User wants to chat with a user
    else if (receivedMessage.getReceiver() == "server")
    {
        if (users->getUser(receivedMessage.getContents()).online)
        {
            sendMessage(socket, {receivedMessage.getSender(), "server", " "});
            continueConection = true;
        }
    }
    // User wants history
    else if (receivedMessage.getReceiver() == "history")
    {
        HistoryStore *historyStore = &HistoryStore::getInstance();
        ChatHistory history;

        history = historyStore->getChat({receivedMessage.getSender(), receivedMessage.getContents()});
        std::vector<Message> messages = history.getHistory();

        sendMessage(socket, {receivedMessage.getSender(),
                             "server",
                             std::to_string(messages.size())});

        for (auto &&message : messages)
        {
            sendMessage(socket, message);
        }
        continueConection = true;
    }
    // Unknown initial message
    else
    {
        std::cout << "Received unknown message format: \"" << receivedMessage << "\".\n";
        continueConection = false;
    }

    return continueConection;
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

    // TODO: Use initialConnectionHandler

    UserStore *users = &UserStore::getInstance();
    std::string currentUserName;
    std::string currentRecipientName;
    Message authenticationMessage;
    bool valid = initialConnectionHandler(socket, currentRecipientName);

    if (!valid)
    {
        return;
    }

    // authentication
    //    initialAuthentication(socket, users, currentUserName, authenticationMessage);

//    currentRecipientName = authenticationMessage.getReceiver();

    Message receivedMessage; // outside loop for performance
    // send/receive loop
    while (users->getUser(currentUserName).online) // figure out a better way to get user consistently
    {
        // receive message
        messageReceiverV2(socket, receivedMessage);

        int status = validateMessage(receivedMessage, currentUserName, currentRecipientName);

        switch (status)
        {
        case StatusCodes::OK:
            // all good
            break;

        case StatusCodes::WRONG_SENDER:
            // terminate session
            // return
            break;
        case StatusCodes::WRONG_RECEIVER:
            // TODO: figure it out.

            break;
        default:
            throw std::exception(); // something is really wrong
            break;
        }

        // TODO: refactor arguments
        HistoryStore::getInstance().appendHistory(
            {currentUserName, currentRecipientName},
            std::vector<Message>{receivedMessage});

        // send message to recipient queue
        sharedMessagePool[receivedMessage.getReceiver()].waitAndPush(receivedMessage);

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
        // exit condition
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
