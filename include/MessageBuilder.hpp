#ifndef __MESSAGEBUILDER_H__
#define __MESSAGEBUILDER_H__

#include <boost/asio.hpp>

#include "Message.hpp"

//const unsigned char MAX_SIZE = 255;

class MessageBuilder
{
    // We limit the scope of this so it doesn't leak into other classes
    using Socket = boost::asio::ip::tcp::socket;

public:
    MessageBuilder()
        : sender(""), receiver(""), message(""), rBuffer(" ")
    {
        rBuffer.resize(Message::MAX_FIELD_SIZE + 1);
    }

    boost::system::error_code setAll(Socket &socket);
    boost::system::error_code setSender(Socket &socket);
    boost::system::error_code setReceiver(Socket &socket);
    boost::system::error_code setMessage(Socket &socket);

    bool setSender(const std::string &sender);
    bool setReceiver(const std::string &receiver);
    bool setMessage(const std::string &message);

    Message build() const;

private:
    std::string setField(Socket &socket, boost::system::error_code &error);

    std::string sender;
    std::string receiver;
    std::string message;
    std::string rBuffer;
};

#endif // __MESSAGEBUILDER_H__
