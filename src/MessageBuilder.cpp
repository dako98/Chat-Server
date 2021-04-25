#include <strstream>
#include <boost/asio/read_until.hpp>

#include "MessageBuilder.hpp"

std::string MessageBuilder::setField(Socket &socket, boost::system::error_code& error)
{
    int readLength = 0;

    unsigned char size;

    while (readLength < 1 && !error)
        readLength += socket.read_some(boost::asio::buffer(rBuffer, 1), error);
    readLength = 0;

    size = rBuffer[0];

    int fieldLen = size;
    rBuffer.resize(fieldLen);

    std::string field = "";

    while (readLength < fieldLen && !error)
    {
        readLength += socket.read_some(boost::asio::buffer(rBuffer, fieldLen), error);
        field += rBuffer;
    }

    return field;
}

/*
boost::system::error_code MessageBuilder::setSender(Socket &socket)
{
    boost::system::error_code error;
    int readLength = 0;

    unsigned char size;


    while (readLength < 1 && !error)
        readLength += socket.read_some(boost::asio::buffer(rBuffer, 1), error);
    readLength = 0;

    size = rBuffer[0];

    int receiverLen = size;
    rBuffer.resize(receiverLen);

    while (readLength < receiverLen && !error)
    {
        readLength += socket.read_some(boost::asio::buffer(rBuffer, receiverLen), error);
        sender += rBuffer;
    }

    return error;
}
boost::system::error_code MessageBuilder::setReceiver(Socket &socket)
{
    boost::system::error_code error;

    int readLength = 0;

    unsigned char size;
    

    while (readLength < 1 && !error)
        readLength += socket.read_some(boost::asio::buffer(rBuffer, 1), error);
    readLength = 0;

    size = rBuffer[0];

    int receiverLen = size;
    rBuffer.resize(receiverLen);

    while (readLength < receiverLen && !error)
    {
        readLength += socket.read_some(boost::asio::buffer(rBuffer, receiverLen), error);
        receiver += rBuffer;
    }

    return error;
}
boost::system::error_code MessageBuilder::setMessage(Socket &socket)
{
    boost::system::error_code error;
    int readLength = 0;

    unsigned char size;
    

    while (readLength < 1 && !error)
        readLength += socket.read_some(boost::asio::buffer(rBuffer, 1), error);
    readLength = 0;

    size = rBuffer[0];

    int receiverLen = size;
    rBuffer.resize(receiverLen);

    while (readLength < receiverLen && !error)
    {
        readLength += socket.read_some(boost::asio::buffer(rBuffer, receiverLen), error);
        message += rBuffer;
}
    return error;
}
*/

boost::system::error_code MessageBuilder::setReceiver(Socket &socket)
{
    boost::system::error_code code;
    receiver = setField(socket, code);
    return code;
}
boost::system::error_code MessageBuilder::setMessage(Socket &socket)
{
        boost::system::error_code code;
    message = setField(socket, code);
    return code;
}
boost::system::error_code MessageBuilder::setSender(Socket &socket)
{
        boost::system::error_code code;
    sender = setField(socket, code);
    return code;
}

Message MessageBuilder::build() const
{
    /* 
    * We don't make any checks here, since it's the
    * users responsibility to use it correctly.
    * Exceptions will be thrown and must be handled. 
    */

    return std::move(Message(message, sender, receiver));
}

boost::system::error_code MessageBuilder::setAll(Socket &socket)
{
    boost::system::error_code error;

    if (
        !(error = setReceiver(socket)) &&
        !(error = setSender(socket)) &&
        !(error = setMessage(socket)))
        return error;
}

bool MessageBuilder::setSender(const std::string &sender)
{
    bool valid = false;
    if (sender.length() < Message::MAX_FIELD_SIZE)
    {
        this->sender = sender;
        valid = true;
    }
    return valid;
}
bool MessageBuilder::setReceiver(const std::string &receiver)
{
    bool valid = false;
    if (receiver.length() < Message::MAX_FIELD_SIZE)
    {
        this->receiver = receiver;
        valid = true;
    }
    return valid;
}
bool MessageBuilder::setMessage(const std::string &message)
{
    bool valid = false;
    if (message.length() < Message::MAX_FIELD_SIZE)
    {
        this->message = message;
        valid = true;
    }
    return valid;
}
