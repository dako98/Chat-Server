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

const int DEFAULT_CHAT_HISTORY_LENGTH = 5;
class ChatHistory
{
public:
    ChatHistory() = default;
    ChatHistory(usrType user1, usrType user2, msgType message)
    {
        participants.insert(user1);
        participants.insert(user2);
        history.push_back(message);
    }

    ChatHistory(const ChatHistory &other) = default;

    const std::vector<msgType> getHistory(int last = DEFAULT_CHAT_HISTORY_LENGTH) const;

    void addMessage(const msgType &message);

    const std::unordered_set<usrType> &getParticipants() const;

private:
    std::vector<msgType> history;
    std::unordered_set<usrType> participants;
};
#endif // __CHATHISTORY_H__
