#ifndef __CHATHISTORY_H__
#define __CHATHISTORY_H__

#include <vector>
#include <unordered_set>
#include <memory>

#include "Message.hpp"
#include "User.hpp"

//using msgType = std::shared_ptr<const Message>;
//using usrType = std::shared_ptr<const User>;

using msgType = Message;
using usrType = std::string;

class ChatHistory
{
static const int DEFAULT_CHAT_HISTORY_LENGTH = 5;
public:
    ChatHistory() = default;
    ChatHistory(std::string user1, std::string user2, Message message)
    {
        participants.insert(user1);
        participants.insert(user2);
        history.push_back(message);
    }

    ChatHistory(const ChatHistory &other) = default;

    const std::vector<Message> getHistory(int last = DEFAULT_CHAT_HISTORY_LENGTH) const;

    void addMessage(const Message &message);

    const std::unordered_set<std::string> &getParticipants() const;

private:
    std::vector<Message> history;
    std::unordered_set<std::string> participants;
};
#endif // __CHATHISTORY_H__
