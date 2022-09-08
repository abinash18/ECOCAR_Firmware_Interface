#ifndef DEBUG_HPP
#define DEBUG_HPP

#define DEBUG_HPP_ENABLED 0
/*print out debug messages on USB port*/

#if DEBUG_HPP_ENABLED

#define DebugMsg(x) Serial.print(x)
#define DebugMsgHex(x) Serial.print(x, HEX)
#define DebugLn() Serial.println()

#else

#define DebugMsg(x) 
#define DebugMsgHex(x) 
#define DebugLn() 

#endif

#endif