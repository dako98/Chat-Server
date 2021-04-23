#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define debug

#include <string>
#include <memory>

#include "User.hpp"

//using usrType = std::string;

class Message
{

public:
    std::string getSender() const;
    std::string getReceiver() const;
    std::string getContents() const;

    Message()
        : contents(""), receiver(""), sender("")
    {
    }
/*
    Message(const std::string &text,
            const User &sender,
            const User &receiver)
        : receiver(receiver.getName()), sender(sender.getName()), contents(text)
    {
    }*/
    Message(const std::string &text,
            const std::string &sender,
            const std::string &receiver)
        : receiver(receiver), sender(sender), contents(text)
    {
    }
    Message(const Message &other)
    {
        this->contents = other.contents;
        this->receiver = other.receiver;
        this->sender = other.sender;
    }

    Message &operator=(Message &&other)
    {
        this->contents= std::move( other.contents);
        this->receiver= std::move( other.receiver);
        this->sender = std::move(other.sender);
        return *this;
    }

    Message &operator=(const Message &other) = default;

    /*    Message(Message &&other)
    {
        std::swap(this->contents, other.contents);
        std::swap(this->receiver, other.receiver);
        std::swap(this->sender, other.sender);
    }*/

    friend std::ostream &operator<<(std::ostream &out, const Message &obj);

    ~Message();

private:
    std::string contents;
    std::string receiver;
    std::string sender;
};

#endif // __MESSAGE_H__
