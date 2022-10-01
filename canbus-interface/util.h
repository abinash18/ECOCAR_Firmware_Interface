#include <libserialport.h>
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
int check(enum sp_return result);
/* Helper function to give a name for each parity mode. */
const char *parity_name(enum sp_parity parity);
#ifdef __cplusplus
}
#endif // __cplusplus