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
 * @param participants 
 * @param messages 
 * @return true - succesful add
 * @return false - chat not found
 */
bool HistoryStore::appendHistory(const std::unordered_set<User> &participants,
                                 const std::vector<Message> &messages)
{
    std::unordered_set<std::string> partNames;
    for (auto &i : participants)
    {
        partNames.insert(i.getName());
    }

    for (auto &currChat : chats)
    {
        
        if (currChat.getParticipants() == partNames)
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
