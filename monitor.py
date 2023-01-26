from libserial_interface import *

# create a pointer to the device we are using
device = ctypes.c_void_p()

# tell the C library to set the value of this pointer to an allocated space on the heap.
si_allocate_device(ctypes.byref(device))

# tell the C library to use that pointer to open a COM/tty port and store 
# the data needed to access it in the device structure
si_open_channel(125000, c_char_p(b"/dev/ttyACM0"), device, SI_READ)

# now that we have the comunication channel open we can do what we want.

# here is an example of a serial monitor.
i = 0
while (True):
    print(i)
    print("Read Line: " + sip_read_line(device))
    i+= 1
    
sip_close(device)