#include "util.h"
#include <libserialport.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
    int print_ports();
    struct device
    {
        struct sp_port *port;
        struct sp_event_set *event_set;
    };

    uint64_t get_first_active_port();

    void *open_channel(int baud, const char *port_name);

    void print_port_info(void *device);
    void print_port_config(void *device);
    void free_lib(void *device);

    void read_until(struct device *device, char terminator, char **buffer);
    int si_write(struct device *device, char **data, int size);

    void wait_event(sp_event_set *event, int milliseconds);
    void set_event_set(sp_event_set *event_set);
    void free_event_set(sp_event_set *event_set);

#ifdef __cplusplus
}
#endif // __cplusplus