## - 11 characters fixed for a variable

Types:
- integer, unsigned
    - int32_t, uint32_t
- float 
    - single precision

# - 11 characters for alarms and events and an optional 12 characters of extra data to include.

----

# Protocol
* Mesages are 31 bytes in string ending with newline (\n) 32 bytes total.

* messages are formated as so:

(3 letter command) (11-letter name) (12 character data)

## Commands:
- SET {variable} {value}\n
- GET {variable}\n
- VAL {variable} {value}\n __REPLY__

- EVT {name}\n __REPLY__
- ALM {name}\n

- SUB {EVT|ALM|VAR} {NAME}\n

## Wildcards:
- +OK Ping the CT board returns 1

- +CANADDR CAN address
- +DBGFILTER variable filter
- DBGCANBUS canbus buffer
- *OK comms hub ping
- *RTCDATE date YYYYMMDD
- *RTCTIME time HHMMSS
- *VCOUNT number of variables
- *VN#X X is a nummber; returns the variable name at position X and its type i.e. VAL *VN#0:F MOTOR_I