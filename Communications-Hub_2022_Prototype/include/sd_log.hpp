#ifndef SD_LOG_HPP
#define SD_LOG_HPP

#include "SdFat2.h"
#include <stdint.h>
#define SD_LOG_BUFFER_SIZE (8 * 1024)
#define SD_LOG_HIGH_WATER_MARK (3.0f / 4.0f)


class SDLogger
{
	private:
	SdFatSdioEX sdex;
	File vars, events, alarms;
	bool errorstate = false;
	uint32_t problem_timestamp = 0;
	char vars_buffer[SD_LOG_BUFFER_SIZE];
	char events_buffer[SD_LOG_BUFFER_SIZE];
	char alarms_buffer[SD_LOG_BUFFER_SIZE];
	short var_write_offset, event_write_offset, alarm_write_offset;
	void WriteHeaders(void);

	public:
	void begin(void);
	void appendVars(void);
	void write(void);
	void appendEvent(uint32_t time, char* message, char* extended);
	void appendAlarm(uint32_t time, char* message, char* extended);

};

extern SDLogger SDLogger1;

#endif