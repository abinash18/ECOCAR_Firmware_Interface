#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include <stdint.h>

#define VAR_MAX_NAME 11
/*VAR_MAX_NAME is (somewhat) arbitrarily set at 11 bytes
it was from the comms hub so name + flag + value = 16 bytes*/
#define VAR_MAX_VAL 12
/* VAR_MAX_VAL is the longest string a value can be
-2147483648 (max length of i32) is 11 characters
-1.23456E-38 (max length of f32 with %.6g) is 12 characters
add one for last NUL if needed*/

#define VARIABLE_OUTGOING_REPEAT_TIME 2000 /*[ms] how long to wait between repeating identical variable values*/
#define VARIABLE_OUTGOING_REPEAT_JITTER 1 /*enable or disable intentional jitter*/

struct OutgoingVariable
{
	bool enabled;
	char varName[VAR_MAX_NAME];
	char varVal[VAR_MAX_VAL];
	uint32_t nextSendTime;
};

enum SubscribedVariableType
{
	SVT_NONE = 0,
	SVT_VAR,
	SVT_EVT,
	SVT_ALM,
	SVT_DELETED,
};

const int SVT_FLAG_ONCE = 0x80; //flag is set for GET requests, which should reply only once

const int SVT_TYPE_MASK = 0x7F; //how many bits are type data and not flags

struct IncomingVariable
{
	uint8_t VType;
	char varName[VAR_MAX_NAME];
};

bool IncomingFilterMatch(IncomingVariable* ivArray, uint8_t ivcount, char* var_name, uint8_t var_type, bool matchExact = false, bool removeMatch = true);
bool OutgoingCacheCheck(OutgoingVariable* ovArray, uint8_t ovcount, char* var_name, char* var_val);
bool IncomingFilterRemove(IncomingVariable* ivArray, uint8_t ivcount, char* var_name, uint8_t var_type);
bool IncomingFilterAdd(IncomingVariable* ivArray, uint8_t ivcount, char* var_name, uint8_t var_type);

//the contents of the old variables.hpp follow

struct stored_var;
extern stored_var VARIABLES[];
extern int NUM_VARIABLES;

union u_variable{
	uint32_t u32;
	int32_t i32;
	float f32;
};

struct stored_var
{
	char name[VAR_MAX_NAME];
	uint8_t type;
	union u_variable data;
};


enum stored_var_type
{
	TYPE_U32,
	TYPE_I32,
	TYPE_F32
};

bool setvar(char* name, char* value);
bool setvar(stored_var* item);
/*output_buf should be VAR_MAX_VAL + 1 characters*/
bool getvar(char* name, char* output_buf);
bool getvar(stored_var* item, char* output_buf);
stored_var* getvarbyindex(int index);

#endif