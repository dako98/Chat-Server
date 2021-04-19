#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include "User.hpp"
#include "Message.hpp"
#include "ChatHistory.hpp"

#define debug

const int PORT = 42123;

using boost::asio::ip::tcp;

int main()
{
    /*
    try
    {
        boost::asio::io_service io_service;

        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), PORT));
        tcp::socket socket(io_service);
        
        std::cout << "waiting for connection..." << std::endl;
        acceptor.accept(socket);

        std::cout << "got connection from: " << socket.remote_endpoint().address().to_string()
            << " port " << socket.remote_endpoint().port() << std::endl;

        std::cout << "closing" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }*/

    User testUser1("user1", "pass1");
    User testUser2("user2", "pass2");
    std::string text("asdqwerty");

    std::shared_ptr<User> smPtr = std::make_shared<User>("user3", "pass3");

//    Message testMessage(std::make_shared<const std::string>(text),
    Message testMessage(text,

                        /*
    std::make_shared<const User>(testUser1),
    std::make_shared<const User>(testUser2));
    */
                        //                        smPtr, smPtr);
                        &testUser1, &testUser2);
    ChatHistory testChat(smPtr.get(), smPtr.get(),
                         /*std::make_shared<Message>(testMessage)*/
                         &testMessage);
    auto hist = testChat.getHistory(1);
    for (auto &&i : hist)
    {
        std::cout << *i;
    }

    return 0;
}
