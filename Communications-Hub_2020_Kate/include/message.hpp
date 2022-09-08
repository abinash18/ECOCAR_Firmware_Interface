#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#define MSG_SIZE 32 /*bytes per message*/

struct Message{
	char str[MSG_SIZE];
	Message(void);
	Message(const char*);
	char& operator[](const int index);
};


#endif