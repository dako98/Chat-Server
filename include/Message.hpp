#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define debug

#include <string>

class Message
{

public:
    std::string& getSender() const;
    std::string& getReceiver() const;
    std::string& getContents() const;

    Message(const std::string &text,
            const std::string &sender,
            const std::string &receiver);

    ~Message();

private:
    const std::string sender;
    const std::string receiver;
    const std::string contents;
};

#endif // __MESSAGE_H__
