#include "handlers.hpp"
#include "variables.hpp"
#include "debug.hpp"
#include "specialfunctions.hpp"
#include "sd_log.hpp"

#define ACTION_LEN 3

#include "preprocess.hpp"

static const char* msg_parse_string = 
"%" xstr(ACTION_LEN) "s "
"%" xstr(VAR_MAX_NAME) "s "
"%" xstr(VAR_MAX_VAL) "s";

static void filter_send(char* message, char* var_name, uint8_t filter_type, serial_client* source, bool matchExact = false);

/*The standard handler parses the input on the 3 serial ports
(and the USB port if enabled) and pushes commands out to the bus
or to other ports if needed*/

void StandardHandler(serial_client* sc, char* message)
{
	char action[ACTION_LEN + 1] = {0}; //GET, SET, etc
	char var_name[VAR_MAX_NAME + 1] = {0}; //eg MOTOR_V
	char var_value[VAR_MAX_VAL + 1] = {0}; //eg 12.41

	sscanf(message, msg_parse_string, action, var_name, var_value);
	
	if(strcmp(action, "GET") == 0)
	{
		if(var_name[0] == '+' || var_name[0] == '*')
		{
			//translator special variables
			DebugMsg("Special Get "); DebugMsg(var_name); DebugLn();
			special_get(sc, var_name);
		}
		else
		{
			//read from storage db
			DebugMsg("Regular Get "); DebugMsg(var_name); DebugLn();
			char value_buffer [VAR_MAX_VAL + 1];
            Message msg = {0};
            if(getvar(var_name, value_buffer))
            {
                snprintf(msg.str, MSG_SIZE, "VAL %s %s", var_name, value_buffer);
                DebugMsg(msg.str); DebugLn();
                sc->Outgoing.Push(msg);
            }
		}
		
	}
	else if (strcmp(action, "SET") == 0)
	{
		if(var_name[0] == '+' || var_name[0] == '*')
		{
			DebugMsg("Special Set "); DebugMsg(var_name); DebugLn();
		}
		else
		{
			DebugMsg("Regular Set "); DebugMsg(var_name); DebugMsg(" to "); DebugMsg(var_value); DebugLn();
			if(OutgoingCacheCheck(sc->ValueCache, SERIALCLIENT_OUTGOING_CACHED_VARIABLES, var_name, var_value))
			{
				//add to local db
				if(setvar(var_name, var_value))
            	{
                	DebugMsg("SET OK\n");
            	}
				//send through can bus as SET
				CANBUS.Outgoing.Push(message);
				//filter send to other boards
				memcpy(message, "VAL", 3);
				filter_send(message, var_name, SVT_VAR, sc);
			}
		}
	}
	else if (strcmp(action, "EVT") == 0)
	{
		DebugMsg("Event "); DebugMsg(var_name); DebugLn();
		CANBUS.Outgoing.Push(message);
		filter_send(message, var_name, SVT_EVT, sc);
		SDLogger1.appendEvent(millis(), var_name, var_value);
	}
	else if (strcmp(action, "ALM") == 0)
	{
		DebugMsg("Alarm "); DebugMsg(var_name); DebugLn();
		CANBUS.Outgoing.Push(message);
		filter_send(message, var_name, SVT_ALM, sc);
		SDLogger1.appendAlarm(millis(), var_name, var_value);
	}
	else if (strcmp(action, "SUB") == 0)
	{
		DebugMsg("Subscribe "); DebugMsg(var_name); DebugLn();
		uint8_t vtype;
		if(strcmp(var_name, "VAR") == 0)
			vtype = SVT_VAR;
		else if (strcmp(var_name, "EVT") == 0)
			vtype = SVT_EVT;
		else if (strcmp(var_name, "ALM") == 0)
			vtype = SVT_ALM;
		else
			return;

		IncomingFilterAdd(sc->IncomingFilter, SERIALCLIENT_INCOMING_FILTER_COUNT, var_value, vtype);		
	}
	else if (strcmp(action, "UNS") == 0)
	{
		DebugMsg("Unsubscribe "); DebugMsg(var_name); DebugLn();
		uint8_t vtype;
		if(strcmp(var_name, "VAR") == 0)
			vtype = SVT_VAR;
		else if (strcmp(var_name, "EVT") == 0)
			vtype = SVT_EVT;
		else if (strcmp(var_name, "ALM") == 0)
			vtype = SVT_ALM;
		else
			return;

		IncomingFilterRemove(sc->IncomingFilter, SERIALCLIENT_INCOMING_FILTER_COUNT, var_value, vtype);
	}
}

static void filter_send(char* message, char* var_name, uint8_t filter_type, serial_client* source, bool matchExact)
{
	for(int i = 0; i < NUM_CLIENTS; i++)
	{
		serial_client* sc = CLIENTS + i;
		if(sc == source) continue;
		if(IncomingFilterMatch(sc->IncomingFilter, SERIALCLIENT_INCOMING_FILTER_COUNT,
		var_name, filter_type, matchExact))
		{
			sc->Outgoing.Push(message);
		}
	}
}

void CanHandler(CanBus* cb, char* message)
{
	char action[ACTION_LEN + 1] = {0}; 
	char var_name[VAR_MAX_NAME + 1] = {0}; 
	char var_value[VAR_MAX_VAL + 1] = {0}; 

	sscanf(message, msg_parse_string, action, var_name, var_value);

	if(strcmp(action, "SET") == 0 )
	{
		setvar(var_name, var_value);
		memcpy(message, "VAL", 3); //translate other boards' SET to VAL
		filter_send(message, var_name, SVT_VAR, NULL);
	}
	else if (strcmp(action, "VAL") == 0)
	{
		//ignore
	}
	else if (strcmp(action, "EVT") == 0)
	{
		filter_send(message, var_name, SVT_EVT, NULL);
		SDLogger1.appendEvent(millis(), var_name, var_value);
	}
	else if (strcmp(action, "ALM") == 0)
	{
		filter_send(message, var_name, SVT_ALM, NULL);
		SDLogger1.appendAlarm(millis(), var_name, var_value);
	}
	else if (strcmp(action, "GET") == 0)
	{
		if(var_name[0] == '*')
		{
			special_get(cb, var_name);
		}
		else
		{
			char value_buffer [VAR_MAX_VAL + 1];
            Message msg = {0};
            if(getvar(var_name, value_buffer))
            {
                snprintf(msg.str, MSG_SIZE, "VAL %s %s", var_name, value_buffer);
                cb->Outgoing.Push(msg);
            }
		}
	}
}

void SendToAll(char* message)
{
	char action[ACTION_LEN + 1] = {0}; 
	char var_name[VAR_MAX_NAME + 1] = {0}; 
	char var_value[VAR_MAX_VAL + 1] = {0}; 

	sscanf(message, msg_parse_string, action, var_name, var_value);

	if(strcmp(action, "ALM") == 0)
	{
		CANBUS.Outgoing.Push(message);
		filter_send(message, var_name, SVT_ALM, NULL);
	}
	else if (strcmp(action, "EVT") == 0)
	{
		CANBUS.Outgoing.Push(message);
		filter_send(message, var_name, SVT_EVT, NULL);
	}
}