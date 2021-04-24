#include "UserStore.hpp"

UserStore::UserStore()
    : users()
{
}

bool UserStore::addUser(const User &newUser)
{
    bool exists = users[newUser.getName()].getName() != "";

    if (!exists)
    { // FIXME: Make so we take the copy and move it to the map
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

UserStore &UserStore::getInstance()
{
    static UserStore instance;
    return instance;
}
