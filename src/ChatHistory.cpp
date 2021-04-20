#include "ChatHistory.hpp"

const std::vector<msgType> ChatHistory::getHistory(int last) const
{
    // TODO: Study "return local addr" warning
    if (last > 0)
    {
        /* code */

        last = std::min<int>(last, history.size());
        return std::vector<msgType>(history.end() - last, history.end());
    }
    return std::vector<msgType>();
    //return history;
}
void ChatHistory::addMessage(const msgType &message)
{
    history.push_back(message);
}

const std::unordered_set<usrType> &ChatHistory::getParticipants() const
{
    return participants;
}
