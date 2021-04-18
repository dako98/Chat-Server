#include "Message.hpp"

Message::Message(const std::string &text,
                 const std::string &sender,
                 const std::string &receiver)
    : contents(text), sender(sender), receiver(receiver)
{
    #ifdef debug

#include <iostream>

    std::cout << "Message Constructor called, SENDER:\"" << sender
              << "\" RECEIVER:\"" << receiver
              << "\" CONTENTS:\"" << contents << "\"\n";
#endif
}

Message::~Message()
{
#ifdef debug

#include <iostream>

    std::cout << "Message Destructor called, SENDER:\"" << sender
              << "\" RECEIVER:\"" << receiver
              << "\" CONTENTS:\"" << contents << "\"\n";
#endif
}
