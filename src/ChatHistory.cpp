#include "ChatHistory.hpp"

const std::vector<Message> ChatHistory::getHistory(int last) const
{
    // TODO: Study "return local addr" warning
    if (last > 0)
    {
        /* code */

        last = std::min<int>(last, history.size());
        return std::vector<Message>(history.end() - last, history.end());
    }
    return std::vector<Message>();
    //return history;
}
void ChatHistory::addMessage(const Message &message)
{
    history.push_back(message);
}

const std::unordered_set<std::string> &ChatHistory::getParticipants() const
{
    return participants;
}

std::ostream& operator<<(std::ostream &out, const ChatHistory &obj) 
{
    for (auto &&i : obj.participants)
    {
        out << i << ' ';
    }
    out << '\n';
    for (auto &&i : obj.history)
    {
        out << i << '\n';
    }
    out << '\n';

    return out;
}
