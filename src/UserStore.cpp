#include "UserStore.hpp"

UserStore::UserStore()
    : users()
{
}

bool UserStore::addUser(const User &newUser)
{
    bool exists = users[newUser.getName()].getName() != "";

    if (!exists)
    {
        users[newUser.getName()] = newUser;
    }
    return !exists;
}

/**
 * @brief gives the user object for editing
 * 
 * @param name user from database
 * @return User& 
 */
User &UserStore::getUser(const std::string &name)
{
    return users[name];
}

std::vector<std::string> UserStore::getOnline() const
{
    std::vector<std::string> onlineUsers;

    for (auto &&i : users)
    {
        if (i.second.online)
        {
            onlineUsers.push_back(i.first);
        }
    }
    return onlineUsers;
}

std::ostream &operator<<(std::ostream &out, const UserStore &obj)
{
    for (auto &&i : obj.users)
    {
        out << i.second << '\n';
    }
    return out;
}

UserStore &UserStore::getInstance()
{
    static UserStore instance;
    return instance;
}

bool reservedName(const std::string &name)
{
    const std::array<const std::string, 6> reservedNames = {"server",
                                                            "register",
                                                            "history",
                                                            "login",
                                                            "chat",
                                                            " "};
    bool reserved = false;
    for (auto &&i : reservedNames)
    {
        if (name == i)
        {
            reserved = true;
            break;
        }
    }
    return reserved;
}

std::istream &operator>>(std::istream &in, UserStore &obj)
{
    User user;
    while (!in.eof())
    {
        in >> user;
        obj.addUser(user);
    }
    return in;
}
