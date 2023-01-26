from libserial_interface import *

device = ctypes.c_void_p()

si_allocate_device(ctypes.byref(device))
si_open_channel(125000, c_char_p(b"/dev/ttyACM0"), device, SI_READ_WRITE)

si_flush_buffer(device, SI_BOTH)

buffer = c_void_p()
si_allocate_buffer(ctypes.byref(buffer),  ctypes.c_int(1),  ctypes.c_int(32))
sip_write_line(device, "SET VOLTAGE 3222")
sip_write_line(device, "GET VOLTAGE")

# si_read_until(device, c_char(b"\n"), ctypes.byref(buffer))
# print(cast(buffer, c_char_p).value.decode("utf-8"))

print(sip_read_line(device))

sip_close(device)
si_free_lib(device)