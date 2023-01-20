#include "serial-interface/cbi.h"
#include "serial-interface/util.h"
#include <libserialport.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#define BAUD_RATE 125000

	/* Example of how to wait for events on multiple ports.
	 *
	 * This example file is released to the public domain. */

	/* Helper function for error handling. */
	int check(enum sp_return result);

	int main(int argc, char **argv)
	{
		// /* Get the port names from the command line. */
		// if (argc < 2)
		// {
		// 	printf("Usage: %s <port name>...\n", argv[0]);
		// 	return -1;
		// }
		// int num_ports = argc - 1;
		// char **port_names = argv + 1;

		struct device *p;

		si_allocate_device((void **)(&p));

		si_open_channel(BAUD_RATE, "/dev/ttyACM0", p, SI_READ_WRITE);

		printf("Waiting up to 5 seconds for RX on any port...\n");
		check(sp_wait(p->event_set_rx, 1000));
		char *buffer;
		// buffer = (char *)calloc(32, sizeof(char));
		// read_until(p, '\n', &buffer);
		// printf("\x1b[31mRecieved string:\033[0m %s\n", buffer);
		// free(buffer);
		// read_until(p, '\n', &buffer);
		// printf("\x1b[31mRecieved string:\033[0m %s\n", buffer);
		// free(buffer);
		// buffer = (char *)calloc(32 + 1, sizeof(char));
		// read_until(p, '\n', &buffer);
		// printf("\x1b[31mRecieved string:\033[0m %s\n", buffer);
		// free(buffer);

		const char *data = "GET VOLTAGE"; //{'G', 'E', 'T', ' ', 'V', 'O', 'L', 'T', 'A', 'G', 'E'};

		const char *papa = "SET VOLTAGE 99"; //{'S', 'E', 'T', ' ', 'V', 'O', 'L', 'T', 'A', 'G', 'E', ' ', '1', '0', '0'};
		int size = strlen(data);
		char end = '\n';
		int result;
		// check(sp_wait(p->event_set_tx, 1000));
		si_write_line(p, papa, strlen(papa), true);
		// check(sp_wait(p->event_set_rx, 1000));
		//   read_until(p, '\n', &buffer);
		//    char d[];
		//   sscanf("%s%s", data, end);

		for (int i = 0; i < 1000; i++)
		{
			si_write_line(p, data, size, false);
			printf("iter : %d\n", i);
			// check(sp_wait(p->event_set_rx, 1000));
			buffer = (char *)calloc(32, sizeof(char));
			read_until(p, '\n', &buffer);
			printf("\x1b[32mRecieved string:\033[0m %s\n", buffer);
			free(buffer);
		}

		// char *data2 = "\n";
		// check(sp_wait(p->event_set_tx, 1000));
		// result = si_write(p, &data2, size);

		// check(sp_wait(p->event_set_rx, 1000));
		// buffer = (char *)calloc(32, sizeof(char));
		// read_until(p, '\n', &buffer);
		// printf("\x1b[31mRecieved string:\033[0m %s\n", buffer);
		// free(buffer);
		// int s = 0;
		// while (true)
		// {
		// 	check(sp_wait(p->event_set_rx, 1000));
		// 	buffer = (char *)calloc(32, sizeof(char));
		// 	read_until(p, '\n', &buffer);
		// 	printf("\x1b[31mRecieved string:\033[0m %s\n", buffer);
		// 	free(buffer);
		// 	s++;
		// 	printf("%d\n", s);
		// }
		// check(sp_wait(p->event_set_rx, 1000));
		// buffer = (char *)calloc(32, sizeof(char));
		// read_until(p, '\n', &buffer);
		// printf("\x1b[31mRecieved string:\033[0m %s\n", buffer);
		// free(buffer);
		// sp_free_event_set(p->event_set_rx);
		// sp_free_event_set(p->event_set_tx);
		// check(sp_close(p->port));
		// sp_free_port(p->port);
		return 0;
	}

#ifdef __cplusplus
}
#endif // __cplusplus