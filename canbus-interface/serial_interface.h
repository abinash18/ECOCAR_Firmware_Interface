#include "util.h"
#include <libserialport.h>
#include <stdint.h>

#define SI_READ 1
#define SI_WRITE 2
#define SI_READ_WRITE 3

#ifdef __cplusplus

extern "C"
{
#endif // __cplusplus

    struct device
    {
        struct sp_port *port;
        struct sp_event_set *event_set_rx;
        struct sp_event_set *event_set_tx;
        char *name = "test";
    };

    int print_ports();

    uint64_t get_first_active_port();

    void si_allocate_device(void **port_device);

    void si_open_channel(int baud, const char *port_name, void *port_device, int mode);

    void print_port_info(void *port_device);
    void print_port_config(void *port_device);
    void free_lib(void *port_device);

    void read_until(void *port_device, char terminator, char **buffer);

    int si_write(void *port_device, char **data, int size);

    void wait_event(sp_event_set *event, int milliseconds);
    void set_event_set(sp_event_set *event_set);
    void free_event_set(sp_event_set *event_set);

#ifdef __cplusplus
}
#endif // __cplusplus