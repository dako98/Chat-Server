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
    for (auto &&i : obj.chats)
    {
        out << i;
        //out << i << '\n';
    }
    out << '}';
    return out;
}

HistoryStore &HistoryStore::getInstance()
{
    static HistoryStore instance;
    return instance;
}
