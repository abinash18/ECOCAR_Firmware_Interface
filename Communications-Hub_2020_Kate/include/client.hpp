#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdint.h>
#include "CircularBuffer.hpp"
#include "variables.hpp"
#include "message.hpp"

#include <Arduino.h>

#define INPUT_BUF_SIZE MSG_SIZE /*needed to be larger if we're handling nonstandard messages*/
#define SERIALCLIENT_OUTPUT_BUFFER_COUNT 16 /*number of messages queued up to be sent to client*/
#define SERIALCLIENT_OUTGOING_CACHED_VARIABLES 32 /*number of variables to check for duplicates*/
#define SERIALCLIENT_INCOMING_FILTER_COUNT 64 /*to select specific messages to process*/


class SerPort{
	public:
	union {
		HardwareSerial* HW;
		usb_serial_class* USB;
	} ;
	char type;
	int available(void);
	int read(void);
	void begin(uint32_t baud);
	int availableForWrite(void);
	size_t write(uint8_t data);
	size_t print(int32_t number);
	size_t print(const char* data);
	size_t print(float data);

	SerPort(HardwareSerial* HWS);
	SerPort(usb_serial_class* USB);
	SerPort(const SerPort&) = default;
};

typedef void (*serial_handler)(struct serial_client*, char*);

struct serial_client : IMessageDispatch<Message>
{
	serial_client(SerPort port, serial_handler callback);
	SerPort Port;
	serial_handler Callback;
	ecocar::CircularBuffer<Message, SERIALCLIENT_OUTPUT_BUFFER_COUNT> Outgoing;
	char Incoming[INPUT_BUF_SIZE];
	int8_t output_index;
	int8_t input_index;
	void HandleInput(void);
	void HandleOutput(void);

	OutgoingVariable ValueCache[SERIALCLIENT_OUTGOING_CACHED_VARIABLES];
	IncomingVariable IncomingFilter[SERIALCLIENT_INCOMING_FILTER_COUNT];

	bool Send(Message);
};

extern serial_client CLIENTS[];
extern int NUM_CLIENTS;


#endif