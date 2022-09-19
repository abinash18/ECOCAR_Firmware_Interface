#include <libserialport.h>

extern "C" int print_ports();

struct port
{
    sp_port *device;
};