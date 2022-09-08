#include "variables.hpp"
#include <cstring>
#include <cstddef>
#include <cstdlib>
#include <stdio.h>
#include <inttypes.h>
#include <Arduino.h>
#include "debug.hpp"

static stored_var* get_var_item(char* name)
{
	if(name == NULL) 
	{
		DebugMsg("Requested null item\n");
		return NULL;
	}
	for(int i = 0; i < NUM_VARIABLES; i++)
	{
		if(std::strncmp(name, VARIABLES[i].name, VAR_MAX_NAME) == 0)
		{
			return &(VARIABLES[i]);
		}
	}
	DebugMsg("Variable "); DebugMsg(name);
	DebugMsg(" not found\n");
	return NULL;
}

bool setvar(char* name, char* value)
{
	stored_var* item = get_var_item(name);
	if(item == NULL)
		return false;
	DebugMsg("Set ");
	switch(item->type)
	{
		case TYPE_U32:
		{
			uint32_t newval = strtoul(value, NULL, 10);
			item->data.u32 = newval;
			DebugMsg("U32");
		}
		break;
		case TYPE_I32:
		{
			int32_t newval = strtol(value, NULL, 10);
			item->data.i32 = newval;
			DebugMsg("I32");
		}
		break;
		case TYPE_F32:
		{
			float newval = strtof(value, NULL);
			item->data.f32 = newval;
			DebugMsg("F32");
		}
		break;
	}
	DebugMsg(" "); DebugMsg(name); DebugMsg(" to "); DebugMsg(value); DebugLn();
	return true;
}

bool getvar(char* name, char* buf)
{
	stored_var* item = get_var_item(name);
	return getvar(item, buf);
}

bool getvar(stored_var* item, char* buf)
{
	size_t buffer_len = VAR_MAX_VAL + 1; //trailing NUL from snprintf
	if(item == NULL)
		return false;
	switch(item->type)
	{
		case TYPE_I32:
		{
			snprintf(buf, buffer_len, "%" PRId32, item->data.i32);
		}
		break;
		case TYPE_U32:
		{
			snprintf(buf, buffer_len, "%" PRIu32, item->data.u32);
		}
		break;
		case TYPE_F32:
		{
			snprintf(buf, buffer_len, "%.6G", item->data.f32);
		}
		break;
	}
	return true;
}

bool setvar(stored_var* new_item)
{
	if(new_item == NULL)
	{
		DebugMsg("Null variable set?\n");
		return false;
	}
	stored_var* existing = get_var_item(new_item->name);
	if(existing == NULL)
	{
		DebugMsg("Could not find variable ");
		DebugMsg(new_item->name);
		DebugLn();
		return false;
	}
	if(new_item->type != existing->type)
	{
		DebugMsg("Type Mismatch\n");
		return false;
	}
	existing->data = new_item->data;
	return true;
}

stored_var* getvarbyindex(int index)
{
	if(index >= NUM_VARIABLES || index < 0)
		return NULL;
	return VARIABLES + index;
}