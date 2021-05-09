#include <algorithm>

#include "HistoryStore.hpp"

HistoryStore::HistoryStore()
{
}

ChatHistory HistoryStore::getChat(const std::unordered_set<std::string> &participants) const
{
    //    std::unordered_set<std::string> partNames;
    /*for (auto &&i : participants)
    {
        partNames.insert(i.getName());
    }*/

    for (auto &&currChat : chats)
    {
        if (currChat.getParticipants() == participants)
        {
            return currChat;
        }
    }
    return ChatHistory();
}

/**
 * @brief Add messages to chat history
 * 
 * @param participants names of participants
 * @param messages messages to pushback
 * @return true - succesful add
 * @return false - chat not found
 */
bool HistoryStore::appendHistory(const std::unordered_set<std::string> &participants,
                                 const std::vector<Message> &messages)
{
    /**
     * Locking is for the whole method because if two threads search for a
     * chat that doesn't exist, they will both add a new chat that will likely
     * never be used and will just take up space.
     */

    std::lock_guard<std::mutex> lock(mut);

    bool found = false;
    for (auto &currChat : chats)
    {
        if (currChat.getParticipants() == participants)
        {
            found = true;
            for (auto &message : messages)
            {
                currChat.addMessage(message);
            }
        }
    }

    if (!found)
    {
        chats.push_back(ChatHistory{participants, messages});
    }
    cond.notify_one();

    return found;
}

std::istream &operator>>(std::istream &in, HistoryStore &obj)
{
    ChatHistory history;
    if (in.peek() == '{')
    {
        in.ignore();

        while (in.peek() != '}')
        {
            in >> history;
            obj.chats.push_back(history);
        }
        in.ignore();
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const HistoryStore &obj)
{
    out << '{';

    std::lock_guard<std::mutex> lock(obj.mut);

    for (auto &&i : obj.chats)
    {
        out << i;
        //out << i << '\n';
    }

    obj.cond.notify_one();

    out << '}';
    return out;
}

HistoryStore &HistoryStore::getInstance()
{
    static HistoryStore instance;
    return instance;
}
