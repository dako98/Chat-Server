#ifndef __MESSAGE_H__
#define __MESSAGE_H__


#include <string>
#include <memory>

#include "User.hpp"

//using usrType = std::string;

class Message
{
    const unsigned int MAX_FIELD_SIZE = 255;

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
        : receiver(other.receiver), sender(other.sender), contents(other.contents)
    {
    }

    Message &operator=(Message &other)
    {
        if (this != &other)
        {
            this->contents = std::move(other.contents);
            this->receiver = std::move(other.receiver);
            this->sender = std::move(other.sender);
        }
        return *this;
    }

    Message &operator=(const Message &other)
    {
        if (this!=&other)
        {        
        this->contents = other.contents;
        this->receiver = other.receiver;
        this->sender = other.sender;
        }
        return *this;
    }

    /*    Message(Message &&other)
    {
        std::swap(this->contents, other.contents);
        std::swap(this->receiver, other.receiver);
        std::swap(this->sender, other.sender);
    }*/

    friend std::ostream &operator<<(std::ostream &out, const Message &obj);
    friend std::istream &operator>>(std::istream &in, Message &obj);

    ~Message();

private:
    std::string contents;
    std::string receiver;
    std::string sender;
};

#endif // __MESSAGE_H__
