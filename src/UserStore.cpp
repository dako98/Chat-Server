#include "UserStore.hpp"

UserStore::UserStore()
    : users()
{
}

bool UserStore::addUser(const User &newUser)
{
    bool exists = users[newUser.getName()].getName() != "";

    if (!exists)
    {   // FIXME: Make so we take the copy and move it to the map
        users[newUser.getName()] = newUser;
    }
    return !exists;
}

User UserStore::getUser(const std::string name) const
{
    return getUser(name);
}

UserStore &UserStore::getInstance()
{
    static UserStore instance;
    return instance;
}
