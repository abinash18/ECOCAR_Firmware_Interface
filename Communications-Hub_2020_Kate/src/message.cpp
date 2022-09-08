#include "message.hpp"
#include "string.h"


Message::Message(void)
{
	memset(this->str, 0, MSG_SIZE);
}

Message::Message(const char* text)
{
	this->str[0] = 0;
	strncat(this->str, text, MSG_SIZE - 1);
}

char& Message::operator[](const int index)
{
	if(index < 0)
		return this->str[0];
	if(index >= MSG_SIZE)
		return this->str[MSG_SIZE - 1];
	return this->str[index];
}