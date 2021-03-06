#ifndef __USER_H__
#define __USER_H__


#include <string>
#include <iostream>

class User
{

public:
    User()
        : name(""), password(""), online(false) {}
    User(const std::string &name,
         const std::string &password);

    User(const User &other);

    User &operator=(const User &other);
    bool operator<(const User &other) const;
    bool operator==(const User &other) const;

    bool comparePassword(const std::string &string) const;

    std::string getName() const;

    ~User();

    bool online;

    void serialise() const;

    friend std::ostream &operator<<(std::ostream &out, const User &obj);
    friend std::istream &operator>>(std::istream &in, User &obj);


private:
    std::string name;
    std::string password;
};
namespace std
{
    template <>
    struct hash<User>
    {
        size_t operator()(const User &x) const
        {
            return hash<std::string>()(x.getName());
        }
    };
}

#endif // __USER_H__
