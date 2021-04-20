#ifndef __USERSDATABASE_H__
#define __USERSDATABASE_H__

#include <map>
#include <string>

#include "User.hpp"
class UserStore
{

public:
    static UserStore &getInstance()
    {
        static UserStore usersDB;
        return usersDB;
    }

    UserStore(UserStore const &) = delete;
    void operator=(UserStore const &) = delete;

    bool addUser(const User newUser);
    const std::string getName(const std::string &name) const;

private:
    UserStore();

    std::map<const std::string, const User> users;
};

#endif // __USERSDATABASE_H__
