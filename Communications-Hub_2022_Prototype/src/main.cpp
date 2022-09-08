#include <Arduino.h>
#include <FlexCAN.h>
#include <stdint.h>
#include "client.hpp"
#include "canbus.hpp"
#include <TimeLib.h>
#include "debug.hpp"
#include "sd_log.hpp"
#include "handlers.hpp"

#define CAN_STBY_PIN 2
#define LED_PIN 13

#define SDLOG_INTERVAL 50 /*ms*/
#define SDLOG_FLUSH 15000 /*ms*/

uint32_t log_time, logflush_time;

time_t timesync(void)
{
	return Teensy3Clock.get();
}

static void MeasurePerformance(void);

//put in a header
void LoadVariablesFromSDCard(void);


void setup() {
	Serial .begin(125000);
	Serial1.begin(125000);
	Serial2.begin(125000);
	Serial3.begin(125000);
	Serial4.begin(125000);
	Serial5.begin(125000);

	#if DEBUG_HPP_ENABLED
		delay(10000);
	#endif

	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);

	//take can bus out of standby
	pinMode(CAN_STBY_PIN, OUTPUT);
	digitalWrite(CAN_STBY_PIN, LOW);

	//something for the rtc
	setSyncProvider(timesync);

	Can0.begin(1000000L);
	Can0.setNumTXBoxes(1); //hopefully force in-order transmission


	//enable cycle counter
	// https://forum.pjrc.com/threads/28407-Teensyduino-access-to-counting-cpu-cycles?p=71036&viewfull=1#post71036
	ARM_DEMCR |= ARM_DEMCR_TRCENA;
	ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

	LoadVariablesFromSDCard();

	SDLogger1.begin();
	log_time = logflush_time = millis();
}

void loop() {
	for(int i = 0; i < NUM_CLIENTS; i++)
	{
		serial_client* sc = &CLIENTS[i];
		sc->HandleOutput();
	}

	for(int i = 0; i < NUM_CLIENTS; i++)
	{
		serial_client* sc = &CLIENTS[i];
		sc->HandleInput();
	}

	CANBUS.HandleInput();
	CANBUS.HandleOutput();
	if(CANBUS.HandleLockup())
	{
		char msg[32];
		snprintf(msg, sizeof(msg), "EVT CANLOCK %03X", (unsigned)CANBUS.SendAddress);
		//Since the outgoing buffer is full, we need to throw away a message
		//if we want the lockup message to get sent
		CANBUS.Outgoing.Pop();
		SendToAll(msg);	
	}

	if(millis() - log_time >= SDLOG_INTERVAL)
	{
		SDLogger1.appendVars();
		log_time = millis();
	}

	if(millis() - logflush_time >= SDLOG_FLUSH)
	{
		SDLogger1.write();
		logflush_time = millis();
	}

	MeasurePerformance();

	digitalWrite(13,((millis() & (1 << 9)) > 0));
}

static void MeasurePerformance(void)
{
	static uint32_t loopstart = 0;
	static uint32_t loops = 0;
	static stored_var perfctr = {{'C','O','M','_','L','O','O','P','/','D','S'}, TYPE_F32};
	
	uint32_t now = millis();
	
	++loops;
	if(now - loopstart >= 100 )
	{
		float time_ds = (float)(now - loopstart) / 100;
		perfctr.data.f32 = (float) loops / time_ds;
		setvar(&perfctr);
		loopstart = now;
		loops = 0;
	}
}