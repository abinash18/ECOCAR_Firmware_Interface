#include "serial_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
    void read_until(void *port_device, char terminator, char **buffer)
    {
        struct device *device_p = (struct device *)port_device;
        bool terminator_reached = false;
        //*buffer = (char *)calloc(32 + 1, sizeof(char));
        char *current_byte = (char *)calloc(1, sizeof(char));
        int bytes_read = 0;
        check(sp_wait(device_p->event_set_rx, 1000));
        while (!terminator_reached)
        {
            while (check(sp_input_waiting(device_p->port)) == 0)
            {
            };
            bytes_read = check(sp_blocking_read((device_p->port), current_byte, sizeof(char), 1000));
            printf("%s", current_byte);
            if (bytes_read != 0)
                strcat(*buffer, current_byte);
            if (strchr(*buffer, '\n') != NULL)
            {
                // printf("Readhecd");
                strcat(*buffer, "\0");
                terminator_reached = true;
                continue;
            }
            check(sp_wait(device_p->event_set_rx, 1000));
        }
        free(current_byte);
    }

    /**
     * WTF vscode? like couldnt u tell me i was overloading a iostream function by accident?????
     */
    int si_write(void *port_device, char **data, int size)
    {
        int result = 0;
        printf("Writing: %s\n", (char *)*data);
        result = check(sp_blocking_write(((struct device *)port_device)->port, *data, size, 1000));
        check(sp_drain(((struct device *)port_device)->port));
        /* Check whether we sent all of the data. */
        if (result == size)
            printf("Sent %d bytes successfully.\n", size);
        else
            printf("Timed out, %d/%d bytes sent.\n", result, size);
        return 0;
    }

    /**
     * Search ports for serial devices, copied from examples folder in libserialport library source.
     *
     */
    int print_ports()
    {

        printf("Looking for device...\n");
        /* A pointer to a null-terminated array of pointers to
         * struct sp_port, which will contain the ports found.*/
        struct sp_port **port_list;

        printf("Getting port list.\n");
        /* Call sp_list_ports() to get the ports. The port_list
         * pointer will be updated to refer to the array created. */
        enum sp_return result = sp_list_ports(&port_list);

        if (result != SP_OK)
        {
            printf("sp_list_ports() failed!\n");
            return -1;
        }

        /* Iterate through the ports. When port_list[i] is NULL
         * this indicates the end of the list. */
        int i;
        for (i = 0; port_list[i] != NULL; i++)
        {
            struct sp_port *port = port_list[i];

            /* Get the name of the port. */
            char *port_name = sp_get_port_name(port);

            printf("Found port: %s\n", port_name);
        }

        printf("Found %d ports.\n", i);

        printf("Freeing port list.\n");

        /* Free the array created by sp_list_ports(). */
        sp_free_port_list(port_list);

        /* Note that this will also free all the sp_port structures
         * it points to. If you want to keep one of them (e.g. to
         * use that port in the rest of your program), take a copy
         * of it first using sp_copy_port(). */

        return 0;
    }

    uint64_t get_first_active_port()
    {
        return 0;
    }

    void free_lib(void *device)
    {
        check(sp_close((struct sp_port *)device));
        sp_free_port((struct sp_port *)device);
    }

    /**
     * Returns a void pointer to the device structure. This is done so there is no need for a device wrapper in python
     */
    void si_allocate_device(void **port_device)
    {
        struct device *p = (struct device *)calloc(1, sizeof(struct device));
        *port_device = p;
        printf("Allocated Device %p : %s\n", p, (*p).name);
    }

    /**
     * Returns a device
     */
    void si_open_channel(int baud, const char *port_name, void *port_device, int mode)
    {
        struct sp_port *port;
        check(sp_get_port_by_name(port_name, &port));
        printf("Opening port: '%s'\n", port_name);
        sp_mode temp_mode;
        switch (mode)
        {
        case SI_READ:
            temp_mode = SP_MODE_READ;
            break;
        case SI_WRITE:
            temp_mode = SP_MODE_WRITE;
            break;
        case SI_READ_WRITE:
            temp_mode = SP_MODE_READ_WRITE;
            break;
        default:
            temp_mode = SP_MODE_READ_WRITE;
            break;
        }
        check(sp_open(port, temp_mode));
        printf("Setting port to %i 8N1, no flow control.\n", baud);
        check(sp_set_baudrate(port, baud));
        check(sp_set_bits(port, 8));
        check(sp_set_parity(port, SP_PARITY_NONE));
        check(sp_set_stopbits(port, 1));
        check(sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE));
        printf("Before setting port.\n");
        (*((struct device *)port_device)).port = port;
        printf("Reached.\n");
        struct sp_event_set *event_set_RX;

        check(sp_new_event_set(&event_set_RX));

        struct sp_event_set *event_set_TX;

        check(sp_new_event_set(&event_set_TX));
        // printf("Adding port RX event to event set.\n");
        check(sp_add_port_events(event_set_RX, port, SP_EVENT_RX_READY));
        check(sp_add_port_events(event_set_TX, port, SP_EVENT_TX_READY));
        ((struct device *)port_device)->event_set_rx = event_set_RX;
        ((struct device *)port_device)->event_set_tx = event_set_TX;
    }

    void print_port_info(void *device)
    {
        /* A pointer to a struct sp_port, which will refer to
         * the port found. */
        struct sp_port *port = (struct sp_port *)device;

        /* Display some basic information about the port. */
        printf("Port name: %s\n", sp_get_port_name(port));
        printf("Description: %s\n", sp_get_port_description(port));

        /* Identify the transport which this port is connected through,
         * e.g. native port, USB or Bluetooth. */
        enum sp_transport transport = sp_get_port_transport(port);

        if (transport == SP_TRANSPORT_NATIVE)
        {
            /* This is a "native" port, usually directly connected
             * to the system rather than some external interface. */
            printf("Type: Native\n");
        }
        else if (transport == SP_TRANSPORT_USB)
        {
            /* This is a USB to serial converter of some kind. */
            printf("Type: USB\n");

            /* Display string information from the USB descriptors. */
            printf("Manufacturer: %s\n", sp_get_port_usb_manufacturer(port));
            printf("Product: %s\n", sp_get_port_usb_product(port));
            printf("Serial: %s\n", sp_get_port_usb_serial(port));

            /* Display USB vendor and product IDs. */
            int usb_vid, usb_pid;
            sp_get_port_usb_vid_pid(port, &usb_vid, &usb_pid);
            printf("VID: %04X PID: %04X\n", usb_vid, usb_pid);

            /* Display bus and address. */
            int usb_bus, usb_address;
            sp_get_port_usb_bus_address(port, &usb_bus, &usb_address);
            printf("Bus: %d Address: %d\n", usb_bus, usb_address);
        }
        else if (transport == SP_TRANSPORT_BLUETOOTH)
        {
            /* This is a Bluetooth serial port. */
            printf("Type: Bluetooth\n");

            /* Display Bluetooth MAC address. */
            printf("MAC: %s\n", sp_get_port_bluetooth_address(port));
        }

        // printf("Freeing port.\n");

        // /* Free the port structure created by sp_get_port_by_name(). */
        // sp_free_port(port);

        /* Note that this will also free the port name and other
         * strings retrieved from the port structure. If you want
         * to keep these, copy them before freeing the port. */
    }

    void print_port_config(void *device)
    {
        /* A pointer to a struct sp_port, which will refer to
         * the port found. */
        struct sp_port *port = (struct sp_port *)device;

        /* Display some basic information about the port. */
        printf("Port name: %s\n", sp_get_port_name(port));
        printf("Description: %s\n", sp_get_port_description(port));

        /* The port must be open to access its configuration. */
        // printf("Opening port.\n");
        // check(sp_open(port, SP_MODE_READ_WRITE));

        /* There are two ways to access a port's configuration:
         *
         * 1. You can read and write a whole configuration (all settings at
         *    once) using sp_get_config() and sp_set_config(). This is handy
         *    if you want to change between some preset combinations, or save
         *    and restore an existing configuration. It also ensures the
         *    changes are made together, via an efficient set of calls into
         *    the OS - in some cases a single system call can be used.
         *
         *    Use accessor functions like sp_get_config_baudrate() and
         *    sp_set_config_baudrate() to get and set individual settings
         *    from a configuration.
         *
         *    Configurations are allocated using sp_new_config() and freed
         *    with sp_free_config(). You need to manage them yourself.
         *
         * 2. As a shortcut, you can set individual settings on a port
         *    directly by calling functions like sp_set_baudrate() and
         *    sp_set_parity(). This saves you the work of allocating
         *    a temporary config, setting it up, applying it to a port
         *    and then freeing it.
         *
         * In this example we'll do a bit of both: apply some initial settings
         * to the port, read out that config and display it, then switch to a
         * different configuration and back using sp_set_config(). */

        /* First let's set some initial settings directly on the port.
         *
         * You should always configure all settings before using a port.
         * There are no "default" settings applied by libserialport.
         * When you open a port it has the defaults from the OS or driver,
         * or the settings left over by the last program to use it. */
        printf("Setting port to 125000 8N1, no flow control.\n");
        check(sp_set_baudrate(port, 125000));
        check(sp_set_bits(port, 8));
        check(sp_set_parity(port, SP_PARITY_NONE));
        check(sp_set_stopbits(port, 1));
        check(sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE));

        /* A pointer to a struct sp_port_config, which we'll use for the config
         * read back from the port. The pointer will be set by sp_new_config(). */
        struct sp_port_config *initial_config;

        /* Allocate a configuration for us to read the port config into. */
        check(sp_new_config(&initial_config));

        /* Read the current config from the port into that configuration. */
        check(sp_get_config(port, initial_config));

        /* Display some of the settings read back from the port. */
        int baudrate, bits, stopbits;
        enum sp_parity parity;
        check(sp_get_config_baudrate(initial_config, &baudrate));
        check(sp_get_config_bits(initial_config, &bits));
        check(sp_get_config_stopbits(initial_config, &stopbits));
        check(sp_get_config_parity(initial_config, &parity));
        printf("Baudrate: %d, data bits: %d, parity: %s, stop bits: %d\n",
               baudrate, bits, parity_name(parity), stopbits);

        /* Create a different configuration to have ready for use. */
        printf("Creating new config for 9600 7E2, XON/XOFF flow control.\n");
        struct sp_port_config *other_config;
        check(sp_new_config(&other_config));
        check(sp_set_config_baudrate(other_config, 9600));
        check(sp_set_config_bits(other_config, 7));
        check(sp_set_config_parity(other_config, SP_PARITY_EVEN));
        check(sp_set_config_stopbits(other_config, 2));
        check(sp_set_config_flowcontrol(other_config, SP_FLOWCONTROL_XONXOFF));

        /* We can apply the new config to the port in one call. */
        printf("Applying new configuration.\n");
        check(sp_set_config(port, other_config));

        /* And now switch back to our original config. */
        printf("Setting port back to previous config.\n");
        check(sp_set_config(port, initial_config));

        // /* Now clean up by closing the port and freeing structures. */
        // check(sp_close(port));
        // sp_free_port(port);
        sp_free_config(initial_config);
        sp_free_config(other_config);
    }

#ifdef __cplusplus
}
#endif // __cplusplus
