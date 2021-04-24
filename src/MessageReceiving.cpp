#include "Codes.hpp"
#include "MessageReceiving.hpp"
#include "MessageBuilder.hpp"
#include "HistoryStore.hpp"

int messageReceiver(tcp::socket &socket, ThreadSafeQueue<Message> &messageQueue)
{
    boost::system::error_code error;

    MessageBuilder receivedMessage;
    error = receivedMessage.setAll(socket);
    if (error)
        throw std::exception();

    Message constructedMessage = receivedMessage.build();

    HistoryStore::getInstance().appendHistory(
        {constructedMessage.getSender(), constructedMessage.getReceiver()}, {constructedMessage});

    messageQueue.waitAndPush(constructedMessage);
}

boost::system::error_code messageReceiverV2(tcp::socket &socket, Message &message)
{
    boost::system::error_code error;

    MessageBuilder receivedMessage;
    error = receivedMessage.setAll(socket);
    if (error)
        throw std::exception();

    message = receivedMessage.build();
    return error;
}

bool initialAuthentication(tcp::socket &socket, UserStore *users,
                           std::string &userAuthenticatedAs, Message &authenticationMessage)
{
    bool valid = false;

    // receive authentication message
    MessageBuilder initialMessageBuilder;
    initialMessageBuilder.setAll(socket);
    Message initialMessage = initialMessageBuilder.build();

#ifdef debug
    std::cout << initialMessage << '\n';
#endif

    std::string currentUserName = users->getUser(initialMessage.getSender()).getName();

    // if user exists and passwords match
    if (currentUserName != "" &&
        users->getUser(currentUserName).comparePassword(initialMessage.getContents()))
    {
        users->getUser(currentUserName).online = true;
        userAuthenticatedAs = currentUserName;
        authenticationMessage = std::move(initialMessage);
        valid = true;
    }

    return valid;
}

unsigned int validateMessage(const Message &msg, const std::string &sender, const std::string &receiver)
{
    unsigned int code = StatusCodes::OK;

    if (msg.getReceiver() != receiver)
        code |= StatusCodes::WRONG_RECEIVER;
    else if (msg.getSender() != sender)
        code |= StatusCodes::WRONG_SENDER;
    else
        code |= StatusCodes::OK;

    return code;
}
