#include <libserialport.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
    int print_ports(char test[]);

    struct port
    {
        struct sp_port *device;
    };

    uint64_t get_first_active_port();

    long open_channel(int baud, const char *port_name);

    void free_lib();
#ifdef __cplusplus
}
#endif // __cplusplus