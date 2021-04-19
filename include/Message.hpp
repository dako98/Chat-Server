#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define debug

#include <string>
#include <memory>

#include "User.hpp"

using usrType = const User *;

class Message
{

public:
    User &getSender() const;
    User &getReceiver() const;
    std::string &getContents() const;

    /* Construct with copies    
    Message(const std::string &text,
            const User &sender,
            const User &receiver);*/

    Message(/*const std::shared_ptr<const std::string> &text,*/
            const std::string &text,
            const usrType &sender,
            const usrType &receiver)
        : Preceiver(receiver), Psender(sender)
          //, Pcontents(text)
          ,
          contents(text)
    {
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

    //TODO: Shouldn't the message have the contents?
    //    const std::shared_ptr<const std::string> Pcontents;
    const std::string contents;
    usrType Preceiver;
    usrType Psender;

};

#endif // __MESSAGE_H__
