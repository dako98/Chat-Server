#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include "User.hpp"
#include "Message.hpp"
#include "ChatHistory.hpp"
#include "HistoryStore.hpp"
#include "UserStore.hpp"

#define debug

const int PORT = 42123;

using boost::asio::ip::tcp;

void handleSocketConnection(tcp::socket &&socket)
{
    std::cout << "got connection from: "
              << " " << socket.remote_endpoint().address().to_string()
              << " port " << socket.remote_endpoint().port() << std::endl;

    std::cout << "writing" << std::endl;

    std::string reply = "socket write - hello world!";

    std::vector<Message> history;

    history;

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

    messageStore->appendHistory(std::unordered_set<User>{testUser1,testUser2}, std::vector<Message>{testMessage});
    ChatHistory historyMessages = messageStore->getChat({testUser1,testUser2});
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
