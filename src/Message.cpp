#include <iostream>
#include "Message.hpp"
#include "MessageBuilder.hpp"
/*
Message::Message(const std::string &text,
                 const User &sender,
                 const User &receiver)
    : contents(text), sender(sender), receiver(receiver)
{
#ifdef debug

    std::cout << "Message Constructor called, SENDER:\"" << sender
              << "\" RECEIVER:\"" << receiver
              << "\" CONTENTS:\"" << contents << "\"\n";
#endif
}*/

Message::~Message()
{
#ifdef debug

#include <iostream>

    std::cout << "Message Destructor called, SENDER:\"" << sender
              << "\" RECEIVER:\"" << receiver
              //<< "\" CONTENTS:\"" << *Pcontents << "\"\n";
              << "\" CONTENTS:\"" << contents << "\"\n";
#endif
}

std::string Message::getSender() const
{
    return sender;
}
std::string Message::getReceiver() const
{
    return receiver;
}
std::string Message::getContents() const
{
    return contents;
}
std::ostream &operator<<(std::ostream &out, const Message &obj)
{
    
#ifdef debug
        out<< "Message { Sender: \"" << (obj.sender)
           << "\" receiver: \"" << (obj.receiver)
           << "\" contents: \"" << (obj.contents) << "\" }";
#endif
#ifndef debug
        out << "{" << obj.sender << " " << obj.receiver << " " << obj.contents << "}";
#endif
        return out;
}

std::istream &operator>>(std::istream &in, Message &obj)
{
    if (in.peek() == '{')
    {
        
        in.ignore();
        std::string sender, receiver, contents;
        contents.reserve(obj.MAX_FIELD_SIZE);
        in >> sender >> receiver;

        in.ignore();
        std::getline(in, contents, '}');
        in.ignore();

        MessageBuilder builder;
        builder.setSender(sender);
        builder.setReceiver(receiver);
        builder.setMessage(contents);
        obj = builder.build();
    }

    return in;
}
