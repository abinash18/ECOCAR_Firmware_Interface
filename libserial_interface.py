from ctypes import *
import ctypes

serial = ctypes.CDLL('./build/serial-interface/libSERIAL_INTERFACE.so')

SI_READ = 1
SI_WRITE = 2
SI_READ_WRITE = 3


SI_INPUT = 1
SI_OUTPUT = 2
SI_BOTH = 3

MAX_BUFFER_SIZE = 32

si_print_ports = serial.si_print_ports

si_allocate_buffer = serial.si_allocate_buffer
si_allocate_buffer.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int]

si_free_buffer = serial.si_free_buffer
si_free_buffer.argtypes = [ctypes.c_void_p]

si_allocate_device = serial.si_allocate_device
si_allocate_device.argtypes = [ctypes.c_void_p]

si_get_string = serial.si_get_string
si_get_string.argtypes = [ctypes.c_void_p]
si_get_string.restype = ctypes.c_char_p

si_open_channel = serial.si_open_channel
si_open_channel.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_void_p, ctypes.c_int]

si_read_until = serial.si_read_until
si_read_until.argtypes = [ctypes.c_void_p, ctypes.c_char, ctypes.c_void_p]

si_write_line = serial.si_write_line
si_write_line.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_bool]
si_write_line.restype = ctypes.c_int
# Open channel on port

si_flush_buffer = serial.si_flush_buffer
si_flush_buffer.argtypes = [ctypes.c_void_p, ctypes.c_int]

si_free_lib = serial.si_free_lib
si_free_lib.argtypes = [ctypes.c_void_p]

si_close = serial.si_close
si_close.argtypes = [ctypes.c_void_p]
"""

"""
def sip_get_string(string_buffer):
    _result = si_get_string(string_buffer)
    result = cast(_result, c_char_p).value
    si_free_buffer(_result)
    return result

"""
Wrapper function to write a line to the device.
"""
def sip_write_line(device, line):
    si_write_line(device, c_char_p(line.encode('utf-8')), ctypes.c_int(len(line)), ctypes.c_bool(True))
    
"""
Reads until the first newline. Manages memory, frees buffer and stores line in python managed memory.
"""
def sip_read_line(device):
    buffer = ctypes.c_void_p()
    si_allocate_buffer(ctypes.byref(buffer), MAX_BUFFER_SIZE, 1)
    si_read_until(device, ctypes.c_char(b"\n"), ctypes.byref(buffer))
    result = cast(buffer, c_char_p).value.decode("utf-8")
    si_free_buffer(ctypes.byref(buffer))
    return result

"""
Closes the port and frees the device.
"""
def sip_close(device):
    si_close(device)
    si_free_buffer(device)
