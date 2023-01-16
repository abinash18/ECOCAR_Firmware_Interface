#include "cbi.h"
#define DEBUG 1
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
    void free_cbi()
    {
    }

    // char *cbi_get_raw(int *device, char *variable)
    // {
    //     /* We'll allow a 1 second timeout for send and receive. */
    //     unsigned int timeout = 1000;
    //     sp_port *port = (*(struct port_device *)device).port;
    //     /* The data we will send. */
    //     char *data = "GET test";
    //     int size = strlen(data);

    //     /* On success, sp_blocking_write() and sp_blocking_read()
    //      * return the number of bytes sent/received before the
    //      * timeout expired. We'll store that result here. */
    //     int result;

    //     /* Send data. */
    //     printf("Sending '%s' (%d bytes) on port %s.\n",
    //            data, size, sp_get_port_name(port));
    //     result = check(sp_blocking_write(port, data, size, timeout));

    //     /* Check whether we sent all of the data. */
    //     if (result == size)
    //         printf("Sent %d bytes successfully.\n", size);
    //     else
    //         printf("Timed out, %d/%d bytes sent.\n", result, size);

    //     /* Now we can call sp_wait() to await any event in the set.
    //      * It will return when an event occurs, or the timeout elapses. */
    //     printf("Waiting up to 5 seconds for RX on any port...\n");
    //     check(sp_wait((*(struct port_device *)device).event_set_rx, 10000));

    //     char *buf;

    //     int bytes_waiting = check(sp_input_waiting(port));
    //     // printf("Port %s: %d bytes received.\n",
    //     // sp_get_port_name(ports[0]), bytes_waiting);
    //     buf = (char *)malloc(bytes_waiting);
    //     sp_blocking_read(port, buf, bytes_waiting, 1000);
    //     printf("%s", buf);
    //     free(buf);
    //     check(sp_wait((*(struct port_device *)device).event_set_rx, 10000));
    // }

    void cbi_set_int32_t(int *device, char *variable, int32_t value)
    {
    }

    void cbi_set_uint32_t(int *device, char *variable, uint32_t value)
    {
    }

    void cbi_set_float(int *device, char *variable, float value)
    {
    }

    void cbi_alm(int *device, char *name)
    {
    }

    void cbi_sub(int *device, int type, char *name)
    {
    }

#ifdef __cplusplus
}
#endif // __cpluspls