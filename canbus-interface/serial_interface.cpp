#include "serial_interface.h"
#include <stdio.h>
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
    /* Example of a helper function for error handling. */
    int check(enum sp_return result)
    {
        int error_code;
        char *error_message;

        switch (result)
        {

            /* Handle each of the four negative error codes that can be returned.
             *
             * In this example, we will end the program on any error, using
             * a different return code for each possible class of error. */

        case SP_ERR_ARG:
            /* When SP_ERR_ARG is returned, there was a problem with one
             * or more of the arguments passed to the function, e.g. a null
             * pointer or an invalid value. This generally implies a bug in
             * the calling code. */
            printf("Error: Invalid argument.\n");
            // end_program(1);

        case SP_ERR_FAIL:
            /* When SP_ERR_FAIL is returned, there was an error from the OS,
             * which we can obtain the error code and message for. These
             * calls must be made in the same thread as the call that
             * returned SP_ERR_FAIL, and before any other system functions
             * are called in that thread, or they may not return the
             * correct results. */
            error_code = sp_last_error_code();
            error_message = sp_last_error_message();
            printf("Error: Failed: OS error code: %d, message: '%s'\n",
                   error_code, error_message);
            /* The error message should be freed after use. */
            sp_free_error_message(error_message);
            // end_program(2);

        case SP_ERR_SUPP:
            /* When SP_ERR_SUPP is returned, the function was asked to do
             * something that isn't supported by the current OS or device,
             * or that libserialport doesn't know how to do in the current
             * version. */
            printf("Error: Not supported.\n");
            // end_program(3);

        case SP_ERR_MEM:
            /* When SP_ERR_MEM is returned, libserialport wasn't able to
             * allocate some memory it needed. Since the library doesn't
             * normally use any large data structures, this probably means
             * the system is critically low on memory and recovery will
             * require very careful handling. The library itself will
             * always try to handle any allocation failure safely.
             *
             * In this example, we'll just try to exit gracefully without
             * calling printf, which might need to allocate further memory. */
            // end_program(4);

        case SP_OK:
        default:
            /* A return value of SP_OK, defined as zero, means that the
             * operation succeeded. */
            printf("Operation succeeded.\n");

            /* Some fuctions can also return a value greater than zero to
             * indicate a numeric result, such as the number of bytes read by
             * sp_blocking_read(). So when writing an error handling wrapper
             * function like this one, it's helpful to return the result so
             * that it can be used. */
            return result;
        }
    }
    /**
     * Search ports for serial devices, copied from examples folder in libserialport library source.
     *
     */
    int print_ports(char test[])
    {
        printf("%s", test);
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

    /**
     * Returns a device 
     */
    long open_channel(int baud, const char *port_name)
    {
        int *handle = nullptr;
        struct sp_port *port;
        check(sp_get_port_by_name(port_name, &port));
        printf("Opening port.\n");
        check(sp_open(port, SP_MODE_READ_WRITE));
        printf("Setting port to %i 8N1, no flow control.\n", baud);
        check(sp_set_baudrate(port, baud));
        check(sp_set_bits(port, 8));
        check(sp_set_parity(port, SP_PARITY_NONE));
        check(sp_set_stopbits(port, 1));
        check(sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE));
        struct device *p;
        
        (*p).port = port;
        return (long)p;
    }

    void free_lib()
    {
    }
#ifdef __cplusplus
}
#endif // __cplusplus
