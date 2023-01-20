/**
 *  @Author: Abinash Singh
 *  @Date: 2023-01-19 19:59:29
 */
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

    /**
     * @brief
     */
    struct device
    {
        struct sp_port *port;
        struct sp_event_set *event_set_rx;
        struct sp_event_set *event_set_tx;
        // char *name; /* Set port name here while opening using malloc */
    };

    /**
     * @brief Prints all active ports on the machine into the output stream.
     * @return 0.
     */
    int print_ports();

    /**
     * @brief Allocates memory on the heap for the device structure. And it sets the param port_device to the location.
     * Note: reference to pointer is used due to setting the pointers value and returning it in the parameter.
     * @param port_device a reference to the device structure pointer on the heap.
     */
    void si_allocate_device(void **port_device);

    /**
     * @brief Opens a channel with default settings and the baud provided. Channel opened will be retured as a device structure,
     * and will be stored in the memory allocated at the port_device pointer. Call si_allocate_device before invoking this method.
     * It will open a serial channel with 8 bits no parity and 1 stop bit
     * (8N1). You can also specify open mode, it can be one of SI_{READ, WRITE, READ_WRITE}.
     * @param baud Baud rate to open channel to.
     * @param port_name A string or char array containing the full path of the port.
     * @param port_device A pointer to a device structure aallocated by calling si_allocate_device.
     * @param mode Set the open mode, it can be one of SI_{READ, WRITE, READ_WRITE}.
     */
    void si_open_channel(int baud, const char *port_name, void *port_device, int mode);

    /**
     * @brief Prints information about the port provided onto the output stream. Note: Port device needs to be allocated
     * and opened before calling.
     * @param port_device Pointer to device structure allocated.
     */
    void print_port_info(void *port_device);

    /**
     * @brief
     * @param port_device
     */
    void print_port_config(void *port_device);

    /**
     * @brief
     * @param port_device
     */
    void free_lib(void *port_device);

    /**
     * @brief
     * @param port_device
     * @param terminator
     * @param buffer
     */
    void read_until(void *port_device, char terminator, char **buffer);

    /**
     * @brief
     * @param port_device
     * @param data
     * @param size
     * @param wait
     * @return
     */
    int si_write_line(void *port_device, const char *data, int size, bool wait);

    /**
     * @brief
     * @param event
     * @param milliseconds
     */
    void wait_event(sp_event_set *event, int milliseconds);

    /**
     * @brief
     * @param event_set
     */
    void set_event_set(sp_event_set *event_set);

    /**
     * @brief
     * @param event_set
     */
    void free_event_set(sp_event_set *event_set);

#ifdef __cplusplus
}
#endif // __cplusplus