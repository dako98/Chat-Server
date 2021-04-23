#include <strstream>
#include <boost/asio/read_until.hpp>

#include "MessageBuilder.hpp"


boost::system::error_code MessageBuilder::setSender(Socket &socket)
{
    boost::system::error_code error;
    int readLength = 0;

    char size;


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

    char size;

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

    char size;
    

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

//#undef Socket
