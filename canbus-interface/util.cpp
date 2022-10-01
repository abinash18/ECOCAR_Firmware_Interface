#include "util.h"
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
    /* Helper function to give a name for each parity mode. */
    const char *parity_name(enum sp_parity parity)
    {
        switch (parity) {
        case SP_PARITY_INVALID:
            return "(Invalid)";
        case SP_PARITY_NONE:
            return "None";
        case SP_PARITY_ODD:
            return "Odd";
        case SP_PARITY_EVEN:
            return "Even";
        case SP_PARITY_MARK:
            return "Mark";
        case SP_PARITY_SPACE:
            return "Space";
        default:
            return NULL;
        }
    }
#ifdef __cplusplus
}
#endif // __cplusplus