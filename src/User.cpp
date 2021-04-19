#include <iostream>

#include "User.hpp"

/*User::User()
{
    std::cout << "user ctor";
    throw std::exception();
}*/

User::User(const std::string &name,
           const std::string &password)
    : name(name), password(password)
{
#ifdef debug

    std::cout << "User Constructor called, NAME:\"" << name
              << "\" PASSWORD:\"" << password << "\"\n";
#endif
}

User::~User()
{
#ifdef debug

    std::cout << "User Destructor called, NAME:\"" << name
              << "\" PASSWORD:\"" << password << "\"\n";
#endif
}

void User::serialise() const
{
    throw std::exception();
}

std::ostream &operator<<(std::ostream &out, const User &obj)
{
    return out << obj.name << ' ' << obj.password;
}
