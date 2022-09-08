#ifndef SPECIAL_FUNCTIONS_HPP
#define SPECIAL_FUNCTIONS_HPP

#include "client.hpp"
#include "CircularBuffer.hpp"

#define VERSION 2.3


void special_get(serial_client* sc, char* req);
void special_get(IMessageDispatch<Message>*, char*);



#endif