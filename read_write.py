from libserial_interface import *
import logging
import threading
import time

format = "\n%(message)s"
logging.basicConfig(format=format, level=logging.INFO, datefmt="%H:%M:%S")

# create a pointer to the device we are using
device_write = ctypes.c_void_p()
device_read = ctypes.c_void_p()
# tell the C library to set the value of this pointer to an allocated space on the heap.
si_allocate_device(ctypes.byref(device_read))
si_allocate_device(ctypes.byref(device_write))

# tell the C library to use that pointer to open a COM/tty port and store 
# the data needed to access it in the device structure
si_open_channel(125000, c_char_p(b"/dev/ttyACM0"), device_write, SI_WRITE)
si_open_channel(125000, c_char_p(b"/dev/ttyACM0"), device_read, SI_READ)

# now that we have the comunication channel open we can do what we want.

STOP_FLAG_READ = False

def read_threaded(device_read):
    logging.info("Read Thread Started.")
    while(not STOP_FLAG_READ):
        read = sip_read_line(device_read)
        logging.info(read)

x = threading.Thread(target=read_threaded, args=(device_read,))
x.start()
# here is an example of a serial monitor.
while (True):
    to_write = input("To write: ")
    sip_write_line(device_write, to_write)
sip_close(device)
