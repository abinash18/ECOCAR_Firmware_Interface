from ctypes import *
import ctypes

lib = ctypes.CDLL('./build/canbus-interface/libCANBUS_INTERFACE.so')


lib.print_ports()

open_channel_func = lib.open_channel
open_channel_func.restype = ctypes.POINTER(ctypes.c_long)
open_channel_func.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_char)]

# Open channel on port
_port = input("Enter the port: ")
#port = c_char(b"/dev/ttyS0")
device = open_channel_func(125000, c_char_p("/dev/ttyS0"))



free_cbi_func = lib.free_cbi

free_cbi_func()
