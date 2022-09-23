/**
 * High level abstraction of the serial library for use with python bindings.
 */
#include "serial_interface.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    int cbi_get(int *port_pointer);

    void free_cbi();
#ifdef __cplusplus
}
#endif // __cplusplus