#ifndef CANBUS_HPP
#define CANBUS_HPP

#include "message.hpp"
#include <FlexCAN.h>
#include "CircularBuffer.hpp"

typedef void (*canbus_handler)(struct CanBus*, char*);

#define CANBUS_OUTPUT_BUFFER_COUNT 16
#define CANBUS_BOARD_BUFFER_COUNT 8
#define CANBUS_SPLIT_BUFFER_COUNT 4

struct CanBusMessage
{
	uint16_t SenderID;
	Message MessageBuffer;
	uint8_t MessageIndex;
	bool Enabled;
};

struct CanBus : IMessageDispatch<Message>
{
	CanBus(FlexCAN* fc, canbus_handler Callback, uint16_t SendAddress);
	FlexCAN* fc;
	canbus_handler Callback;
	ecocar::CircularBuffer<Message, CANBUS_OUTPUT_BUFFER_COUNT> Outgoing;
	CanBusMessage Incoming[CANBUS_BOARD_BUFFER_COUNT];
	uint16_t SendAddress;
	
	ecocar::CircularBuffer<CAN_message_t, CANBUS_SPLIT_BUFFER_COUNT> SplitOutgoing;
	void HandleInput(void);
	void HandleOutput(void);
	bool HandleLockup(void);
	bool Send(Message);
};

extern CanBus CANBUS;

#endif