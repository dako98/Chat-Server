#include <strstream>
#include <boost/asio/read_until.hpp>

#include "MessageBuilder.hpp"

//#define Socket boost::asio::ip::tcp::socket

//using Socket = boost::asio::ip::tcp::socket;

boost::system::error_code MessageBuilder::setSender(Socket &socket)
{
    boost::system::error_code error;
    int readLength = 0;

    char size;
//    std::string rBuffer;

//    rBuffer.resize(1);

    while (readLength < 1)
        readLength += socket.read_some(boost::asio::buffer(rBuffer,1), error);
    readLength = 0;

    size = rBuffer[0];

    int receiverLen = size;
    rBuffer.resize(receiverLen);

    while (readLength < receiverLen)
        readLength = socket.read_some(boost::asio::buffer(rBuffer, receiverLen), error);

    sender = rBuffer;

    return error;
}
boost::system::error_code MessageBuilder::setReceiver(Socket &socket)
{
    boost::system::error_code error;
    
    int readLength = 0;

    char size;

    while (readLength < 1)
        readLength += socket.read_some(boost::asio::buffer(rBuffer,1), error);
    readLength = 0;

    size = rBuffer[0];

    int receiverLen = size;

    while (readLength < receiverLen)
    {
        readLength += socket.read_some(boost::asio::buffer(rBuffer,receiverLen), error);
        receiver += rBuffer;
    }

    //std::string rBuffer = "";
    //receiver = rBuffer;

    return error;
}
boost::system::error_code MessageBuilder::setMessage(Socket &socket)
{
    boost::system::error_code error;
    int readLength = 0;

    char size;
//    std::string rBuffer;

    rBuffer.resize(1);

    while (readLength < 1)
        readLength += socket.read_some(boost::asio::buffer(rBuffer,1), error);
    readLength = 0;

    size = rBuffer[0];

    int receiverLen = size;
    rBuffer.resize(receiverLen);

    while (readLength < receiverLen)
        readLength = socket.read_some(boost::asio::buffer(rBuffer,receiverLen), error);

    message = rBuffer;

    return error;
}
Message MessageBuilder::build() const
{
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

//#undef Socket
    