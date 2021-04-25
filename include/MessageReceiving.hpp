#ifndef __MESSAGERECEIVING_H__
#define __MESSAGERECEIVING_H__


#include <boost/asio.hpp>

#include "Message.hpp"
#include "UserStore.hpp"
#include "ThreadsafeQueue.h"

using boost::asio::ip::tcp;

//TODO: Remove unused function
int messageReceiver(tcp::socket &socket, ThreadSafeQueue<Message> &messageQueue);

/**
 * @brief reads one message from the socket
 * 
 * @param socket 
 * @param messages 
 * @return boost::system::error_code 
 */
boost::system::error_code messageReceiverV2(tcp::socket &socket, Message &message);

//TODO: Remove unused function
bool initialAuthentication(tcp::socket &socket, UserStore *users,
                           std::string &userAuthenticatedAs, Message &authenticationMessage);

unsigned int validateMessage(const Message &msg, const std::string &sender, const std::string &receiver);

#endif // __MESSAGERECEIVING_H__
