from ctypes import *

lib = CDLL('./libCANBUS_INTERFACE.so')


print(lib.print_ports())