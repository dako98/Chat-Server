#ifndef __HISTORYSTORE_H__
#define __HISTORYSTORE_H__

#include <unordered_set>

#include "ChatHistory.hpp"

class HistoryStore
{

public:
    static HistoryStore &getInstance();

    HistoryStore(HistoryStore const &) = delete;
    void operator=(HistoryStore const &) = delete;

    //    void addChat(const ChatHistory &chat);
    ChatHistory getChat(const std::unordered_set<User> &participant) const;
    bool appendHistory(const std::unordered_set<User> &participants,
                       const std::vector<Message> &messages);

private:
    HistoryStore();

    std::vector<ChatHistory> chats;
};
#endif // __HISTORYSTORE_H__
