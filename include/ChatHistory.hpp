#ifndef __CHATHISTORY_H__
#define __CHATHISTORY_H__

#include<vector>

#include "Message.hpp"
#include "User.hpp"

const int DEFAULT_CHAT_HISTORY_LENGTH = 5;
class ChatHistory
{
public:
    const Message &getHistory(int last = DEFAULT_CHAT_HISTORY_LENGTH) const;
    void addMessage(const Message &message);

    const std::vector<User> &getParticipants() const;

private:
    std::vector<Message> history;
    std::vector<User> participants;
};
#endif // __CHATHISTORY_H__
