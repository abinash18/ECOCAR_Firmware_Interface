#include "client.hpp"
#include "variables.hpp"
#include "debug.hpp"
#include "specialfunctions.hpp"
#include <TimeLib.h>
#include "errno.h"
#include "CircularBuffer.hpp"
#include "canbus.hpp"

#include "preprocess.hpp"

void special_get(IMessageDispatch<Message>* md, char* var_name)
{
	if(strcmp(var_name, "*VCOUNT") == 0)
	{
		Message msg;
		snprintf(msg.str, MSG_SIZE, "VAL *VCOUNT %d", NUM_VARIABLES );
		md->Send(msg);
	}
	else if (strncmp(var_name, "*VN#", 4) == 0)
	{
		errno = 0;
		long index = strtol(var_name + 4, NULL, 10);
		if(errno == EINVAL)
			return;
		stored_var* cv = getvarbyindex(index);
		if(cv)
		{
			Message msg;
			char vartype = cv->type == TYPE_F32 ? 'F' : cv->type == TYPE_I32 ? 'I' :
			 cv->type == TYPE_U32 ? 'U' : '?';
			snprintf(msg.str, MSG_SIZE, "VAL *VN#%d:%c %." xstr(VAR_MAX_NAME) "s",
			 (int) index,  vartype, cv->name);
			md->Send(msg);
		}
	}
	else if (strcmp(var_name, "*RTCDATE") == 0)
	{
		Message msg;
		if(timeStatus() == timeNotSet)
		{
			snprintf(msg.str, MSG_SIZE, "VAL *RTCDATE NOTSET");
		}
		else
		{
			snprintf(msg.str, MSG_SIZE, "VAL *RTCDATE %04d%02d%02d", 
			 (int)year(), (int) month(), (int) day());
		}
		md->Send(msg);
	}
	else if (strcmp(var_name, "*RTCTIME") == 0)
	{
		Message msg;
		if(timeStatus() == timeNotSet)
		{
			snprintf(msg.str, MSG_SIZE, "VAL *RTCTIME NOTSET");
		}
		else
		{
			snprintf(msg.str, MSG_SIZE, "VAL *RTCTIME %02d%02d%02d",
			 hour(), minute(), second());
		}
		md->Send(msg);
	}
	else if(strcmp(var_name, "*INFO") == 0) 
	{
		md->Send("VAL *INFO V" xstr(VERSION) " UABECOCAR");
	}
	else if(strcmp(var_name, "*OK") == 0)
	{
		md->Send("VAL *OK 1");
	}
}

void special_get(serial_client* sc, char* var_name)
{
	if(strcmp(var_name, "+DBGFILTER") == 0)
	{
		char varname[VAR_MAX_NAME + 1] = {0};
		for(int i = 0; i < SERIALCLIENT_INCOMING_FILTER_COUNT; i++)
		{
			IncomingVariable* iv = sc->IncomingFilter + i;
			strncpy(varname, iv->varName, VAR_MAX_NAME);
			DebugMsg(i); DebugMsg(" "); DebugMsg(varname); DebugMsg(" "); DebugMsgHex(iv->VType); DebugLn();
		}
	}
	else if (strcmp(var_name, "+DBGCANBUS") == 0)
	{
		for(int i = 0; i < CANBUS_BOARD_BUFFER_COUNT; i++)
		{
			CanBusMessage* cb = CANBUS.Incoming + i;
			char bufstuff[9] = {0};
			strncpy(bufstuff, cb->MessageBuffer.str, 8);
			DebugMsg(i); DebugMsg(cb->Enabled ? " EN " : " DI ");
			DebugMsg(cb->SenderID); DebugMsg(" "); DebugMsg(cb->MessageIndex);
			DebugMsg(" "); DebugMsg(bufstuff); DebugLn();
		}
	}
	else if (strcmp(var_name, "+CANADDR") == 0)
	{
		Message msg;
		snprintf(msg.str, MSG_SIZE, "VAL +CANADDR %03X", (unsigned)CANBUS.SendAddress);
		sc->Send(msg);
	}
	else if (strcmp(var_name, "+OK") == 0)
	{
		sc->Send("VAL +OK 1");
	}
	else
	{
		special_get(static_cast<IMessageDispatch<Message>*>(sc), var_name);
	}
	
}