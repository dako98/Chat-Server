#include "MessageSending.hpp"

void messageSender(tcp::socket &socket, ThreadSafeQueue<Message> &messageQueue)
{
    int code = 0;
    while (!messageQueue.empty())
    {
        Message messageToSend = messageQueue.waitAndPop();
        code = sendMessage(socket, messageToSend);
        if (code != 0)
        {
            // TODO: handle if some error occured
            throw std::exception();
        }
    }
}

void messageSenderV2(tcp::socket &socket, std::vector<Message> &messagesToSend)
{
    int code = 0;

    for (auto &&i : messagesToSend)
    {
        code = sendMessage(socket, i);
        if (code != 0)
        {
            // TODO: handle if some error occured
            throw std::exception();
        }
    }
}

int sendMessage(tcp::socket &socket, const Message &message)
{
    int code = 0;

    std::string bufferStr = "";
    std::string receiver = (char)message.getReceiver().length() + message.getReceiver();
    std::string sender = (char)message.getSender().length() + message.getSender();
    std::string text = (char)message.getContents().length() + message.getContents();
    bufferStr += receiver;
    bufferStr += sender;
    bufferStr += text;

    auto buffer = boost::asio::buffer(bufferStr);

    boost::asio::write(socket, buffer); // shorthand for loop doing socket.write_some()

    return code;
}