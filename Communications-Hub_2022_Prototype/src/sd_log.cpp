#include <stdio.h>
#include <TimeLib.h>
#include "sd_log.hpp"
#include "variables.hpp"
#include "debug.hpp"
#include "handlers.hpp"

#include "preprocess.hpp"


//my very lazy error handling
#define e(x) if(!errorstate) x
#define es(x) if(!errorstate) errorstate = x

#define buffer_append(x) (this-> x ## s_buffer + this-> x ## _write_offset)
#define remaining(x) ( sizeof(this-> x ## s_buffer) - this-> x ## _write_offset)



void SDLogger::begin(void)
{
	DebugMsg("SDEX begin\n");
	if(!this->sdex.begin())
	{
		errorstate = 1;
		DebugMsg("SDEX Begin error\n");
	}
	e(this->sdex.chvol());
	e(DebugMsg("SDEX Start OK\n"));
	char dir[20];
	snprintf(dir, sizeof(dir), "%04d%02d%02d/%02d%02d%02d",
	 year(), month(), day(), hour(), minute(), second());
	e()
	{
		if(!this->sdex.chdir(dir))
		{
			DebugMsg("Need to create SD folder\n");
			es(!this->sdex.mkdir(dir));
			es(!this->sdex.chdir(dir));
		}
		else
		{
			DebugMsg("Folder already exists\n");
		}
	}
	es(!vars.open("datalog.tsv", O_WRITE | O_CREAT | O_TRUNC));
	es(!events.open("events.tsv", O_WRITE | O_CREAT | O_TRUNC));
	es(!alarms.open("alarms.tsv", O_WRITE | O_CREAT | O_TRUNC));
	DebugMsg("Files Opened\n");
	this->var_write_offset = 0;
	this->event_write_offset = 0;
	this->alarm_write_offset = 0;
	this->vars_buffer[0] = 0;
	this->events_buffer[0] = 0;
	this->alarms_buffer[0] = 0;
	WriteHeaders();
}

void SDLogger::appendVars(void)
{
	e()
	{
		int written = 
		// snprintf(this->vars_buffer + this->vars_write_offset,
		//  sizeof(this->buffer) - this->write_offset, 
		//  "%lu\t", (unsigned long) millis());
		snprintf(buffer_append(var), remaining(var), "%lu\t", (unsigned long) millis());
		this->var_write_offset += written;

		for(int i = 0; i < NUM_VARIABLES; i++)
		{
			stored_var* cv = getvarbyindex(i);
			char value[VAR_MAX_VAL + 1];
			if(getvar(cv, value))
			{
				int written = 
				snprintf(buffer_append(var),
				 remaining(var), 
				 "%." xstr(VAR_MAX_VAL) "s\t", 
				 value);
				this->var_write_offset += written;
			}
		}

		if(this->var_write_offset > 0)
			this->vars_buffer[this->var_write_offset - 1] = '\n';
		//write if buffer is over 7/8 full
		if((float) this->var_write_offset / sizeof(this->vars_buffer) > SD_LOG_HIGH_WATER_MARK )
		{
			this->write();
		}
		//DebugMsg("SD Append OK\n");
	}
}


void SDLogger::WriteHeaders(void)
{
	e({
		int written = 
		snprintf(this->vars_buffer + this->var_write_offset,
		 sizeof(this->vars_buffer) - this->var_write_offset, 
		 "timestamp\t");
		this->var_write_offset += written;

		for(int i = 0; i < NUM_VARIABLES; i++)
		{
			stored_var* cv = getvarbyindex(i);
			if(cv)
			{
				int written = 
				snprintf(this->vars_buffer + this->var_write_offset,
				 sizeof(this->vars_buffer) - this->var_write_offset, 
				 "%." xstr(VAR_MAX_NAME) "s\t", 
				 cv->name);
				this->var_write_offset += written;
			}
		}
		if (this->var_write_offset > 0)
			this->vars_buffer[this->var_write_offset - 1] = '\n'; //replace last tab with newline

		this->event_write_offset += snprintf(this->events_buffer, remaining(event), "Milliseconds\tEvent\tInfo\tTime\n");
		this->alarm_write_offset += snprintf(this->alarms_buffer, remaining(alarm), "Milliseconds\tAlarm\tInfo\tTime\n");
	})
}

void SDLogger::write(void)
{
	e(DebugMsg("SD Write\n"));
	if(errorstate && (millis() - problem_timestamp > 30000))
	{
		char msg[] = "ALM SDCARD FAULT";
		problem_timestamp = millis();
		SendToAll(msg);
	}
	e()
	{
		es(this->vars.write(this->vars_buffer, this->var_write_offset) != var_write_offset);
		e(this->vars.flush());
		this->var_write_offset = 0;
		this->vars_buffer[0] = 0;
		es(this->events.write(this->events_buffer, this->event_write_offset) != event_write_offset);
		e(this->events.flush());
		this->event_write_offset = 0;
		this->events_buffer[0] = 0;
		es(this->alarms.write(this->alarms_buffer, this->alarm_write_offset) != alarm_write_offset);
		e(this->alarms.flush());
		this->alarm_write_offset = 0;
		this->alarms_buffer[0] = 0;
		DebugMsg("SD Write OK\n");
	}
}

void SDLogger::appendEvent(uint32_t time, char* message, char* extended)
{
	e()
	{
		int written = snprintf(buffer_append(event), remaining(event), "%lu\t%s\t%s\t%02d:%02d:%02d\n",
		 (unsigned long) time, message, extended, hour(), minute(), second());
		event_write_offset += written;

		if((float) this->event_write_offset / sizeof(this->events_buffer) > SD_LOG_HIGH_WATER_MARK )
		{
			this->write();
		}
	}
}

void SDLogger::appendAlarm(uint32_t time, char* message, char* extended)
{
	e()
	{
		int written = snprintf(buffer_append(alarm), remaining(alarm), "%lu\t%s\t%s\t%02d:%02d:%02d\n",
		 (unsigned long) time, message, extended, hour(), minute(), second());
		alarm_write_offset += written;

		if((float) this->alarm_write_offset / sizeof(this->alarms_buffer) > SD_LOG_HIGH_WATER_MARK )
		{
			this->write();
		}
	}
}

SDLogger SDLogger1;