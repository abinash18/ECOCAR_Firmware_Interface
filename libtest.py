from ctypes import *
import ctypes

serial = ctypes.CDLL('./build/canbus-interface/libCANBUS_INTERFACE.so')

SI_READ = 1
SI_WRITE = 2
SI_READ_WRITE = 3

si_allocate_device = serial.si_allocate_device
si_allocate_device.argtypes = [ctypes.c_void_p]

#serial.print_ports()
si_open_channel = serial.si_open_channel
si_open_channel.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_void_p, ctypes.c_int]
# Open channel on port
# _port = input("Enter the port: ")
#port = c_char(b"/dev/ttyACM0")
device = ctypes.c_void_p()

si_allocate_device(device)
si_open_channel(125000, c_char_p(b"/dev/ttyACM0"), device, SI_READ_WRITE)
# si_read_until = serial.si_read_until
# si_read_until.argtypes = [ctypes.c_void_p, ctypes.c_char, ctypes.POINTER(c_char_p)]
# 
# buffer = create_string_buffer(32+1)
# 
# si_read_until(device, '\n', buffer)
# 
free_lib = serial.free_lib

free_lib(device)
