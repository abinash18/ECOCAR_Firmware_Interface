/**
 *  @Author: Abinash Singh
 *  @Date: 2023-01-19 19:59:29
 */
#include "util.h"
#include <libserialport.h>
#include <stdint.h>

/** PORT MODES */
#define SI_READ 1
#define SI_WRITE 2
#define SI_READ_WRITE 3

#define MAX_MESSAGE_SIZE 32

#ifdef __cplusplus

extern "C"
{
#endif // __cplusplus

    /**
     * @brief The librarys structure to hold port information.
     */
    struct device
    {
        struct sp_port *port;
        struct sp_event_set *event_set_rx;
        struct sp_event_set *event_set_tx;
        // char *name; /* Set port name here while opening using malloc */
    };

    /**
     * @brief Returns a string contained in a string buffer, used as a helper function
     * in python. Because converting a pointer to a different type in python is a headache.
     * @param string_buffer Pointer to char buffer.
     * @return A const char pointer to the string.
     */
    char *si_get_string(void *string_buffer);

    /**
     * @brief Prints all active ports on the machine into the output stream.
     * @return 0.
     */
    int si_print_ports();

    /**
     * @brief Allocates memory on the heap for the device structure. And it sets the param port_device to the location.
     * Note: reference to pointer is used due to setting the pointers value and returning it in the parameter.
     * @param port_device a reference to the device structure pointer on the heap.
     */
    void si_allocate_device(void **port_device);

    /**
     * @brief
     * @param type
     */
    void si_flush_buffer(void *device, int type);

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
    void si_print_port_info(void *port_device);

    /**
     * @brief
     * @param port_device
     */
    void si_print_port_config(void *port_device);

    // TODO: in the future there will be a method to allocate a buffer which will then return a pointer.
    /**
     * @brief Frees the memory allocated by the library.
     * @param port_device
     */
    void si_free_lib(void *port_device);

    /**
     * @brief Reads the serial input at the port provieded until the termination character provided is reached. Non-Blocking
     * @param port_device The allocated and opened device to read from
     * @param terminator a character that the method will read to.
     * @param buffer the buffer to store the result in.
     */
    void si_read_until(void *port_device, char terminator, void **buffer);

    /**
     * @brief Writes to the opened port the data provided with an added newline character.
     * Data cannot be changing. Writes the amount of bytes given by size to the output buffer.
     * If wait is true then this method will wait until the output buffer is drained.
     * @param port_device The allocated and opened port to write to.
     * @param data Pointer to constant array of data.
     * @param size The size of the data array. Can be smaller but not larger than the size of the array.
     * @param wait If set to true the method will wait until the output buffer is drained. Otherwise the method will return.
     * @return The number of bytes written. Cannot gurentee that all bytes have been transmitted unless wait is set to true.
     */
    int si_write_line(void *port_device, const char *data, int size, bool wait);

    /**
     * @brief Waits on a system event for the given amount of time.
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

    /**
     * @brief Allocates a buffer in memory of size at the pointer location provided by the reference.
     * NOTE: The buffer is initialized to all 0's.
     * @param buffer A refrence to a pointer that is to store the buffer's location.
     * @param num_elements Number of elements to assign to the element.
     * @param element_size Size of each element.
     */
    void si_allocate_buffer(void **buffer, int num_elements, int element_size);

    /**
     * @brief Frees buffers allocated on the heap.
     * @param buffer Reference to the buffer pointer.
     */
    void si_free_buffer(void **buffer);

    /**
     * @brief Closes the device port.
     * @param device An allocated and opened device.
     */
    void si_close(void **device);

#ifdef __cplusplus
}
#endif // __cplusplus