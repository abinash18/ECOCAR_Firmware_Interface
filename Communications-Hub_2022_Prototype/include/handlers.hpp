#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "client.hpp"
#include "canbus.hpp"


void StandardHandler(serial_client* sc, char* message);

void CanHandler(CanBus* cb, char* message);

void SendToAll(char* message);

#endif