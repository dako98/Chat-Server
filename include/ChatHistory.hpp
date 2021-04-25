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
    ChatHistory(const std::string &user1, const std::string &user2, const Message &message)
    {
        participants.insert(user1);
        participants.insert(user2);
        history.push_back(message);
    }

    ChatHistory(const std::unordered_set<std::string> &newParticipants, const std::vector<Message> &messages)
        : participants(newParticipants), history(messages)
    {
    }

    ChatHistory(const ChatHistory &other) = default;

    const std::vector<Message> getHistory(int last = DEFAULT_CHAT_HISTORY_LENGTH) const;

    void addMessage(const Message &message);

    const std::unordered_set<std::string> &getParticipants() const;

    friend std::ostream &operator<<(std::ostream &out, const ChatHistory &obj);
    friend std::istream &operator>>(std::istream &out, ChatHistory &obj);


private:
    std::vector<Message> history;
    std::unordered_set<std::string> participants;
};
#endif // __CHATHISTORY_H__
