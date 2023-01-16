/**
 * High level abstraction of the serial library for use with python bindings.
 */
#include "serial_interface.h"
#include <memory>
#include <stdio.h>
#include <string.h>

#define TOTAL_MESSAGE_SIZE 32
#define DATA_TERMINATOR "\n"

#define MAX_COMMAND_SIZE 3
#define MAX_VAR_NAME_SIZE 3
#define MAX_DATA_SIZE 3

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    /**
     * Returns 1 if success full otherwise prints error and returns 0.
     */
    int cbi_begin(char *port_address, int baud);
    /**
     * Returns a string of the raw response i.e. VAL --- ---
     */
    char *cbi_get_raw(int *device, char *variable);

    void cbi_set_int32_t(int *device, char *variable, int32_t value);
    void cbi_set_uint32_t(int *device, char *variable, uint32_t value);
    void cbi_set_float(int *device, char *variable, float value);

    void cbi_alm(int *device, char *name);

    void cbi_sub(int *device, int type, char *name);

    void free_cbi();
#ifdef __cplusplus
}
#endif // __cplusplus