#include<algorithm>

#include "HistoryStore.hpp"

HistoryStore::HistoryStore()
{
}

ChatHistory HistoryStore::getChat(const std::unordered_set<User> &participants) const
{
    std::unordered_set<std::string> partNames;
    for (auto &&i : participants)
    {
        partNames.insert(i.getName());
    }

    for (auto &&currChat : chats)
    {
        if (currChat.getParticipants() == partNames)
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

    for (auto &currChat : chats)
    {
        
        if (currChat.getParticipants() == participants)
        {
            for (auto &message : messages)
            {
                currChat.addMessage(message);
            }
            return true;
        }
    }
    return false;
}

HistoryStore &HistoryStore::getInstance()
{
    static HistoryStore instance;
    return instance;
}
