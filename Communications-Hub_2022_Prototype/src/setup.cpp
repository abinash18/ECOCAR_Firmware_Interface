#include <Arduino.h>
#include "handlers.hpp"
#include "client.hpp"
#include "canbus.hpp"

serial_client CLIENTS[] = 
{
	serial_client(&Serial1, StandardHandler),
	serial_client(&Serial2, StandardHandler),
	serial_client(&Serial3, StandardHandler),
	serial_client(&Serial4, StandardHandler),
	serial_client(&Serial5, StandardHandler),
};

int NUM_CLIENTS = sizeof(CLIENTS)/sizeof(CLIENTS[0]);


CanBus CANBUS(&Can0, CanHandler, 0x01CC);

//The below has been moved to sdcard_load_vars.cpp

// stored_var VARIABLES[] = 
// {
// 	/*Roboteq Board*/
// 	{"MOTOR_I",   TYPE_F32},
// 	{"MOTOR_V",   TYPE_F32},
// 	{"MOTOR_P",   TYPE_F32},
// 	{"CONTROL_I", TYPE_F32},
// 	{"CONTROL_V", TYPE_F32},
// 	{"CONTROL_P", TYPE_F32},
// 	{"RPM",       TYPE_I32},
// 	{"SPEED_KM/H", TYPE_I32},
// 	{{'C','O','M','_','L','O','O','P','/','D','S'}, TYPE_F32},
// };

//int NUM_VARIABLES = sizeof(VARIABLES) / sizeof(VARIABLES[0]);