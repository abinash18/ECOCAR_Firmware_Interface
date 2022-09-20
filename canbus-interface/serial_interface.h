#include <libserialport.h>
#include <stdint.h>
extern "C" int print_ports();

struct port
{
    struct sp_port *device;
};

extern "C" uint64_t get_first_active_port();