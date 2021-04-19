#include <iostream>
#include "Message.hpp"
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

    std::cout << "Message Destructor called, SENDER:\"" << *Psender
              << "\" RECEIVER:\"" << *Preceiver
              //<< "\" CONTENTS:\"" << *Pcontents << "\"\n";
              << "\" CONTENTS:\"" << contents << "\"\n";
#endif
}

User &Message::getSender() const
{
    throw std::exception();
}
User &Message::getReceiver() const
{
    throw std::exception();
}
std::string &Message::getContents() const
{
    throw std::exception();
}
std::ostream &operator<<(std::ostream &out, const Message &obj)
{
    return out
           //<< "Message { Sender: " << *(obj.Psender.get())
           //<< " receiver: " << *(obj.Preceiver.get())
           // << " contents: " << *(obj.Pcontents.get()) << " }\n";
           << "Message { Sender: " << *(obj.Psender)
           << " receiver: " << *(obj.Preceiver)
           << " contents: " << (obj.contents) << " }\n";
}
