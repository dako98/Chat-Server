#ifndef __HISTORYSTORE_H__
#define __HISTORYSTORE_H__

#include <unordered_set>
#include <mutex>
#include <condition_variable>

#include "ChatHistory.hpp"

class HistoryStore
{

public:
    static HistoryStore &getInstance();

    HistoryStore(HistoryStore const &) = delete;
    void operator=(HistoryStore const &) = delete;

    //    void addChat(const ChatHistory &chat);
    ChatHistory getChat(const std::unordered_set<std::string> &participant) const;
    
    bool appendHistory(const std::unordered_set<std::string> &participants,
                       const std::vector<Message> &messages);

    friend std::ostream &operator<<(std::ostream &out, const HistoryStore &obj);
    friend std::istream &operator>>(std::istream &in, HistoryStore &obj);


private:
    HistoryStore();

    std::vector<ChatHistory> chats;
    mutable std::mutex mut;
    mutable std::condition_variable cond;
};
#endif // __HISTORYSTORE_H__
