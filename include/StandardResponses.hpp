#ifndef __STANDARDRESPONSES_H__
#define __STANDARDRESPONSES_H__

#include <boost/asio.hpp>

#include "ThreadsafeQueue.h"
#include "Message.hpp"

using boost::asio::ip::tcp;

bool registerUser(tcp::socket &socket, const Message &receivedMessage);
bool giveChat(tcp::socket &socket, const Message &receivedMessage);

void giveOnline(tcp::socket &socket, const Message &receivedMessage);
bool giveHistory(tcp::socket &socket, const Message &receivedMessage);
bool loginUser(tcp::socket &socket, const Message &receivedMessage, 
               std::unordered_map<std::string, ThreadSafeQueue<Message>> &sharedMessagePool);

bool initialConnectionHandler(tcp::socket &socket, std::string &authenticatedUser,
                              std::string &recipientUser, bool &hasHistory, 
                              std::unordered_map<std::string, ThreadSafeQueue<Message>> &sharedMessagePool);
                              
bool authenticate(const Message &login);

#endif // __STANDARDRESPONSES_H__
