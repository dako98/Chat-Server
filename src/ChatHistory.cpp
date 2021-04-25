#include <sstream>

#include "ChatHistory.hpp"

const std::vector<Message> ChatHistory::getHistory(int last) const
{
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

std::istream &operator>>(std::istream &in, ChatHistory &obj)
{
    if (in.peek() == '{')
    {
        in.ignore();

        std::string buffer;
        std::getline(in, buffer);

        std::stringstream ss(buffer);
        std::string user;

        while (ss)
        {
            ss >> user;
            obj.participants.insert(user);
        }

        Message message;
        while (in.peek() != '}')
        {
            in >> message;
            obj.addMessage(message);
        }
        in.ignore();
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const ChatHistory &obj)
{
    out << '{';
    for (auto &&i : obj.participants)
    {
        out << i << ' ';
    }
    out << '\n';
    for (auto &&i : obj.history)
    {
        out << i << '\n';
    }
    out << '}';

    return out;
}
