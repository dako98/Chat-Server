#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define debug

#include <string>
#include <memory>

#include "User.hpp"

using usrType = std::string;

class Message
{

public:
    usrType getSender() const;
    usrType getReceiver() const;
    std::string getContents() const;

    /* Construct with copies    
    Message(const std::string &text,
            const User &sender,
            const User &receiver);*/

    Message(const std::string &text,
            const User &sender,
            const User &receiver)
        : receiver(receiver.getName()), sender(sender.getName()), contents(text)
    {
    }
    Message(const Message& other)
    {
        this->contents = other.contents;
        this->receiver = other.receiver;
        this->sender = other.sender;
    }

    friend std::ostream &operator<<(std::ostream &out, const Message &obj);

    ~Message();

private:
    // TODO: Figure out if you should have copies of
    // users or pointers to them.
    /*    Old definitions
    const User sender;
    const User receiver;
    const std::string contents;*/

    //    const std::shared_ptr<const User> Preceiver;
    //    const std::shared_ptr<const User> Psender;

    // TODO: Shouldn't the message have the contents?
    //    const std::shared_ptr<const std::string> Pcontents;
    std::string contents;

    // FIXME: Save IDs instead of pointers.
    // Users can move in memory and pointers
    // can become invalid.
    usrType receiver;
    usrType sender;
};

#endif // __MESSAGE_H__
