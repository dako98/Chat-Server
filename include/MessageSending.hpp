#ifndef __MESSAGESENDING_H__
#define __MESSAGESENDING_H__

#include <boost/asio.hpp>

#include "Message.hpp"
#include "ThreadsafeQueue.h"

using boost::asio::ip::tcp;


int sendMessage(tcp::socket &socket, const Message &message);
void messageSender(tcp::socket &socket, ThreadSafeQueue<Message> &messageQueue);
void messageSenderV2(tcp::socket &socket, std::vector<Message> &messagesToSend);

#endif // __MESSAGESENDING_H__
