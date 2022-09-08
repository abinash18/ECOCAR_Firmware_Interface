#include "variables.hpp"
#include <string.h>
#include <Arduino.h>
#include "debug.hpp"

/*New change in version 2.1: the colon is ignored in variable names
  so a GET for TEST:5 is the same as GET for TEST
  This is useful for getting variable names, where you GET *VN#3 but have VAL *VN#3:F
  also for the upcoming message sending*/

/*If matchExact is false, it will also match the "+ALL" filter and will ignore flags
  If removeMatch is true, the flagged temporary variables will be removed on match*/
bool IncomingFilterMatch(IncomingVariable* ivArray, uint8_t ivcount, char* var_name, uint8_t var_type, bool matchExact, bool removeMatch)
{
	//colon handling, see note above
	char var_basename[VAR_MAX_NAME + 1] = {0};
	{
		char* colon_loc = strchr(var_name, ':');
		int chars_to_copy = VAR_MAX_NAME;
		if(colon_loc && (colon_loc - var_name < VAR_MAX_NAME))
		{
			chars_to_copy = colon_loc - var_name;
		}
		strncpy(var_basename, var_name, chars_to_copy);
	}

	for(int i = 0; i <ivcount; i++)
	{
		IncomingVariable* iv = ivArray + i;
		if((iv->VType & SVT_TYPE_MASK) == (var_type & SVT_TYPE_MASK))
		//analyze names of the same type (var, evt, alm)
		{
			if(!matchExact && (strncmp("+ALL", iv->varName, VAR_MAX_NAME) == 0))
			{
				//the special value means all vars are enabled
				DebugMsg("IFM +ALL");DebugLn();
				return true;
			}
			if(strncmp(var_basename, iv->varName, VAR_MAX_NAME) == 0)
			{
				if(matchExact && (iv->VType != var_type))
				{
					continue;					
				}

				//the selected name is in the list

				DebugMsg("IFM Accept "); DebugMsg(var_name); DebugLn();
				if(removeMatch && (iv->VType & SVT_FLAG_ONCE))
				{
					//the client wants this variable only once
					DebugMsg("IFM Temporary"); DebugLn();
					iv->VType = SVT_DELETED;
				}
				return true;
			}
			continue;
		}
		else if (iv->VType == SVT_NONE)
		{
			//this is the end of the list
			DebugMsg("IFM Denied "); DebugMsg(var_name); DebugLn();
			return false;
		}
	}
	//every item in the list is filled
	DebugMsg("IFM all filled up"); DebugLn();
	return false;
}

static void OutgoingCacheAdd(OutgoingVariable* ov, char* var_name, char* var_val)
{
	ov->enabled = true;
	strncpy(ov->varName, var_name, VAR_MAX_NAME);
	strncpy(ov->varVal, var_val, VAR_MAX_VAL);
	ov->nextSendTime = millis() + VARIABLE_OUTGOING_REPEAT_TIME;
}

static void OutgoingCacheUpdate(OutgoingVariable* ov, uint32_t now, char* var_val)
{
	ov->nextSendTime = now + VARIABLE_OUTGOING_REPEAT_TIME;
	#if VARIABLE_OUTGOING_REPEAT_JITTER
		//get some semi-random data to offset next variable by += 512 ms
		uint32_t clock = ARM_DWT_CYCCNT;
		uint8_t offset = ((clock << 6) | ((uint8_t)clock >> 2)); //rotate 2 lsb to msb
		ov->nextSendTime ^= ((uint16_t)offset << 2);
	#endif
	strncpy(ov->varVal, var_val, VAR_MAX_VAL);
}

bool OutgoingCacheCheck(OutgoingVariable* ovArray, uint8_t ovcount, char* var_name, char* var_val)
{
	
	for(int i = 0; i < ovcount; i++)
	{
		OutgoingVariable* ov = ovArray + i;
		if(ov->enabled)
		{
			if(strncmp(var_name, ov->varName, VAR_MAX_NAME) == 0)
			{
				if(strncmp(var_val, ov->varVal, VAR_MAX_VAL) == 0)
				{
					uint32_t now = millis();
					if(now > ov->nextSendTime)
					{
						//update next send time and return true
						OutgoingCacheUpdate(ov, now, var_val);
						DebugMsg("OCC Send "); DebugMsg(var_name); DebugLn();
						return true;
					}
					else
					{
						//it has been sent recently
						DebugMsg("OCC limit "); DebugMsg(var_name); DebugLn();
						return false;
					}
					
				}
				else
				{
					//if data is new, always proceed
					OutgoingCacheUpdate(ov, millis(), var_val);
					DebugMsg("OCC Changed "); DebugMsg(var_name); DebugLn();
					return true;
				}
				
			}
			continue;
		}
		else
		{
			//add variable to cache
			DebugMsg("OCC Adding "); DebugMsg(var_name); DebugLn();
			OutgoingCacheAdd(ov, var_name, var_val);
			return true;
		}
		
	}
	//cache is full
	DebugMsg("OCC Full "); DebugMsg(var_name); DebugLn();
	return true;
}

bool IncomingFilterAdd(IncomingVariable* ivArray, uint8_t ivcount, char* var_name, uint8_t var_type)
{
	char var_basename[VAR_MAX_NAME + 1] = {0};
	{
		char* colon_loc = strchr(var_name, ':');
		int chars_to_copy = VAR_MAX_NAME;
		if(colon_loc && (colon_loc - var_name < VAR_MAX_NAME))
		{
			chars_to_copy = colon_loc - var_name;
		}
		strncpy(var_basename, var_name, chars_to_copy);
	}

	bool matchExact = true, removeMatch = false;
	if(IncomingFilterMatch(ivArray, ivcount, var_basename, var_type, matchExact, removeMatch))
	{
		DebugMsg("IFA Already filtered: "); DebugMsg(var_name); DebugLn();
		return false;
	}
	for(int i = 0; i < ivcount; i++)
	{
		IncomingVariable* iv = ivArray + i;
		if(iv->VType == SVT_DELETED || iv->VType == SVT_NONE)
		{
			DebugMsg("IFA Adding "); DebugMsg(var_name); DebugLn();
			iv->VType = var_type;
			strncpy(iv->varName, var_basename, VAR_MAX_NAME);
			return true;
		}
	}
	DebugMsg("IFA Full"); DebugLn();
	return false;
}

bool IncomingFilterRemove(IncomingVariable* ivArray, uint8_t ivcount, char* var_name, uint8_t var_type)
{
	char var_basename[VAR_MAX_NAME + 1] = {0};
	{
		char* colon_loc = strchr(var_name, ':');
		int chars_to_copy = VAR_MAX_NAME;
		if(colon_loc && (colon_loc - var_name < VAR_MAX_NAME))
		{
			chars_to_copy = colon_loc - var_name;
		}
		strncpy(var_basename, var_name, chars_to_copy);
	}

	for(int i = 0; i < ivcount; i++)
	{
		IncomingVariable* iv = ivArray + i;
		if(iv->VType == SVT_NONE)
		{
			DebugMsg("IFR Not Found "); DebugMsg(var_name); DebugLn();
			return false;
		}
		if((iv->VType) == var_type)
		{
			if(strncmp(iv->varName, var_basename, VAR_MAX_NAME) == 0)
			{
				DebugMsg("IFR Success "); DebugMsg(var_name); DebugLn();
				iv->VType = SVT_DELETED;
				iv->varName[0] = '\0';
				return true;
			}
		}
	}
	DebugMsg("IFR Couldn't find "); DebugMsg(var_name); DebugLn();
	return false;
}