from ctypes import *
import ctypes

serial = ctypes.CDLL('./build/serial-interface/libSERIAL_INTERFACE.so')

SI_READ = 1
SI_WRITE = 2
SI_READ_WRITE = 3

serial.print_ports()

si_allocate_buffer = serial.si_allocate_buffer
si_allocate_buffer.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int]

si_free_buffer = serial.si_free_buffer
si_free_buffer.argtypes = [ctypes.c_void_p]

si_allocate_device = serial.si_allocate_device
si_allocate_device.argtypes = [ctypes.c_void_p]


si_open_channel = serial.si_open_channel
si_open_channel.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_void_p, ctypes.c_int]

si_read_until = serial.si_read_until
si_read_until.argtypes = [ctypes.c_void_p, ctypes.c_char, ctypes.c_void_p]

si_write_line = serial.si_write_line
si_write_line.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_bool]
si_write_line.restype = ctypes.c_int
# Open channel on port


# wrap hello to make sure the free is done
# def hello(name):
#     _result = hello.hello(name)
#     result = _result.value
#     libc.free(_result)
#     return result


# _port = input("Enter the port: ")
#port = c_char(b"/dev/ttyACM0")
device = ctypes.c_void_p()

si_allocate_device(ctypes.byref(device))
si_open_channel(125000, c_char_p(b"/dev/ttyACM0"), device, SI_READ_WRITE)


buffer = c_void_p()
si_allocate_buffer(ctypes.byref(buffer),  ctypes.c_int(1),  ctypes.c_int(32))

si_write_line(device, c_char_p(b"GET VOLTAGE"), ctypes.c_int(11), ctypes.c_bool(False))

si_read_until(device, c_char(b"\n"), ctypes.byref(buffer))

si_free_buffer(ctypes.byref(buffer))


# 
free_lib = serial.free_lib

free_lib(device)

