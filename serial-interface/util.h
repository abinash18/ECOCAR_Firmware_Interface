/**
 *  @Author: Abinash Singh
 *  @Date: 2023-01-19 19:59:29
 */
#include <libserialport.h>
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    /**
     * @brief
     * @param
     * @return
     */
    int check(enum sp_return result);

    /* Helper function to give a name for each parity mode. */
    const char *parity_name(enum sp_parity parity);

#ifdef __cplusplus
}
#endif // __cplusplus