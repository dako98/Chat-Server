#include <iostream>
#include <string>
#include <boost/asio.hpp>
//#include <queue>

#include "User.hpp"
#include "Message.hpp"
#include "ChatHistory.hpp"
#include "HistoryStore.hpp"
#include "UserStore.hpp"
#include "MessageBuilder.hpp"

#include "ThreadsafeQueue.h"

#define debug

const int PORT = 42123;

using boost::asio::ip::tcp;

int sendMessage(tcp::socket &socket, const Message &message);
/* TODO: Have 2 threads, one for receiving messages and one for sending.
They share a message queue. The messages always have a recipient name*/

void messageSender(tcp::socket &socket, ThreadSafeQueue<Message> &messageQueue)
{
    int code = 0;
    while (!messageQueue.empty())
    {
        Message messageToSend = messageQueue.waitAndPop();
        sendMessage(socket, messageToSend);
        if (code != 0)
        {
            // some error occured
        }
    }
}

int sendMessage(tcp::socket &socket, const Message &message)
{
    int code = 0;
    std::string bufferStr;

    bufferStr =
        message.getReceiver().length() + "|" + message.getReceiver() +
        (message.getSender().length() + "|") + message.getSender() +
        (message.getContents().length() + "|") + message.getContents();

    auto buffer = boost::asio::buffer(bufferStr);

    boost::asio::write(socket, buffer); // shorthand for loop doing socket.write_some()

    throw std::exception();
    return code;
}

int messageReceiver(tcp::socket &socket, ThreadSafeQueue<Message> &messageQueue)
{
    boost::system::error_code error;

    MessageBuilder receivedMessage;
    error = receivedMessage.setReceiver(socket);
    if (error)
        throw std::exception();

    error = receivedMessage.setSender(socket);
    if (error)
        throw std::exception();

    error = receivedMessage.setMessage(socket);
    if (error)
        throw std::exception();

    Message constructedMessage = receivedMessage.build();
    
    // TODO: Refactor so messages use user name instead of objects.
    HistoryStore::getInstance().appendHistory(
        {{constructedMessage.getSender(), ""},
         {constructedMessage.getReceiver(), ""}},
        {constructedMessage});

    messageQueue.waitAndPush(constructedMessage);

    //    std::stringstream stre(buffer);
}

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

    std::string recepientName;
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

int main()
{

    // Not sure if good practice.
    UserStore *users = &UserStore::getInstance();
    HistoryStore *messageStore = &HistoryStore::getInstance();

    User testUser1("user1", "pass1");
    User testUser2("user2", "pass2");
    Message testMessage("text1", testUser1, testUser2);

    //    ChatHistory testHistory; // = ChatHistory();
    //    testHistory.addMessage(testMessage);

    messageStore->appendHistory(std::unordered_set<User>{testUser1, testUser2}, std::vector<Message>{testMessage});
    ChatHistory historyMessages = messageStore->getChat({testUser1, testUser2});
    for (auto &&i : historyMessages.getHistory())
    {
        std::cout << i;
    }

    users->addUser(testUser1);

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

            threads.push_back(std::thread(handleSocketConnection, std::move(socket)));
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
