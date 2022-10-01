#include <libserialport.h>
#include <stdint.h>
#include "util.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
    int print_ports(char test[]);
    struct device
    {
        struct sp_port *port;
    };

    uint64_t get_first_active_port();

    void* open_channel(int baud, const char *port_name);

    void print_port_info(void* device);
    void print_port_config(void* device);
    void free_lib(void* device);
#ifdef __cplusplus
}
#endif // __cplusplus