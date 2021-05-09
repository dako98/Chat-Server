#include <unordered_map>

#include "StandardResponses.hpp"

#include "MessageBuilder.hpp"
#include "MessageSending.hpp"
#include "MessageReceiving.hpp"
#include "UserStore.hpp"
#include "HistoryStore.hpp"
#include "Codes.hpp"

bool registerUser(tcp::socket &socket, const Message &receivedMessage)
{
    MessageBuilder builder;
    UserStore *users = &UserStore::getInstance();
    bool continueConnection = false;

    std::string name, password;
    name = receivedMessage.getSender();
    password = receivedMessage.getContents();

    builder.setReceiver(name);
    builder.setSender("server");

    if (!reservedName(name) &&
        users->getUser(name) == User{})
    {
        users->addUser(User{name, password});
        builder.setMessage(" ");

        continueConnection = true;
    }
    else
    {
        builder.setMessage("Name not allowed.");

        continueConnection = false;
    }
    Message response = builder.build();
    sendMessage(socket, response);

    return continueConnection;
}

bool giveChat(tcp::socket &socket, const Message &receivedMessage)
{
    MessageBuilder builder;
    UserStore *users = &UserStore::getInstance();
    bool continueConnection = false;

    if (users->getUser(receivedMessage.getContents()).online)
    {

        builder.setReceiver(receivedMessage.getSender());
        builder.setSender("server");
        builder.setMessage(" ");

        Message response = builder.build();

        sendMessage(socket, response);
        continueConnection = true;
    }
    else
    {
        builder.setReceiver(receivedMessage.getSender());
        builder.setSender("server");
        builder.setMessage("User unavailable.");

        Message response = builder.build();

        sendMessage(socket, response);

        continueConnection = true;
    }
    return continueConnection;
}

void giveOnline(tcp::socket &socket, const Message &receivedMessage)
{
    std::vector<std::string> onlineUsers = UserStore::getInstance().getOnline();

    MessageBuilder builder;

    builder.setReceiver(receivedMessage.getSender());
    builder.setSender("server");

    builder.setMessage(std::to_string(onlineUsers.size()));
    sendMessage(socket, builder.build());

    for (auto &&i : onlineUsers)
    {
        builder.setMessage(i);

        sendMessage(socket, builder.build());
    }
}

bool giveHistory(tcp::socket &socket, const Message &receivedMessage)
{
    MessageBuilder builder;
    UserStore *users = &UserStore::getInstance();
    bool continueConnection = false;

    HistoryStore *historyStore = &HistoryStore::getInstance();
    ChatHistory history;

    history = historyStore->getChat({receivedMessage.getSender(), receivedMessage.getContents()});
    std::vector<Message> messages = history.getHistory();

    builder.setReceiver(receivedMessage.getSender());
    builder.setSender("server");
    builder.setMessage(std::to_string(messages.size()));

    Message response = builder.build();

    sendMessage(socket, response);

    for (auto &&message : messages)
    {
        sendMessage(socket, message);
    }
    continueConnection = true;

    return continueConnection;
}

bool loginUser(tcp::socket &socket, const Message &receivedMessage, std::unordered_map<std::string, ThreadSafeQueue<Message>> &sharedMessagePool)
{
    bool continueConnection = false;
    MessageBuilder builder;

    if (authenticate(receivedMessage))
    {

        builder.setReceiver(receivedMessage.getSender());
        builder.setSender("server");

        if (UserStore::getInstance().getUser(receivedMessage.getSender()).online == false)
        {
            // Ensures that the message queue is created.
            sharedMessagePool[receivedMessage.getSender()];
            UserStore::getInstance().getUser(receivedMessage.getSender()).online = true;
            builder.setMessage(" ");
            continueConnection = true;
        }
        else
        {
            builder.setMessage("User already online.");
            continueConnection = false;
        }

        Message response = builder.build();

        sendMessage(socket, response);
    }
    else
    {
        builder.setReceiver(receivedMessage.getSender());
        builder.setSender("server");
        builder.setMessage("Invalid name or password.");

        Message response = builder.build();

        sendMessage(socket, response);

        continueConnection = false;
    }
    return continueConnection;
}

bool initialConnectionHandler(tcp::socket &socket, std::string &authenticatedUser,
                              std::string &recipientUser, bool &hasHistory,
                              std::unordered_map<std::string, ThreadSafeQueue<Message>> &sharedMessagePool)
{
    bool continueConection = false;

    Message receivedMessage;

    messageReceiverV2(socket, receivedMessage);

    // User wants to register
    if (receivedMessage.getReceiver() == "register")
    {
        continueConection = registerUser(socket, receivedMessage);
    }
    // User wants to login
    else if (receivedMessage.getReceiver() == "login")
    {
        continueConection = loginUser(socket, receivedMessage, sharedMessagePool);
        if (continueConection)
        {
            authenticatedUser = receivedMessage.getSender();
            giveOnline(socket, receivedMessage);
        }
    }
    // User wants to chat with a user
    else if (receivedMessage.getReceiver() == "chat")
    {
        continueConection = giveChat(socket, receivedMessage);
        if (continueConection)
        {
            recipientUser = receivedMessage.getContents();
        }
    }
    // User wants history
    else if (receivedMessage.getReceiver() == "history")
    {
        continueConection = giveHistory(socket, receivedMessage);
        if (continueConection)
        {
            hasHistory = true;
        }
    }
    else if (validateMessage(receivedMessage, authenticatedUser, recipientUser) == StatusCodes::TERMINATED)
    {
        std::cout << authenticatedUser << " closed the conection.\n";
        continueConection = false;
    }
    // Unknown initial message
    else
    {
        std::cout << "Received unknown message format: \"" << receivedMessage << "\".\n";
        continueConection = false;
    }

    return continueConection;
}

bool authenticate(const Message &login)
{
    bool valid = false;

    UserStore *users = &UserStore::getInstance();

    if (login.getSender() != " " &&
        login.getContents() != " " &&
        users->getUser(login.getSender()).comparePassword(login.getContents()))
    {
        //        users->getUser(login.getSender()).online = true;
        valid = true;
    }
    return valid;
}
