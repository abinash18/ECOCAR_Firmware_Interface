---
marp: true
---

# libSERIAL_INTERFACE.so
by Abi

---
# Use 
- used to communicate to serial devices.

- python wrapper to be used in quick prototyping.

---

# Development

- I used wsl (had problems with virtual box and multipass not accepting usb passthrough)

- I used cmake as a build system which is versitile and can cross compile to different archetectures.

---

# troubleshooting

- it is easy to debug using gdb pure C code

- but it is extreamly difficult to debug a already compiled C shared object when using its functions in python.

- i just used print statements and if it segfaulted before my print i just moved it line by line up.

---

# Lesson learned c is better.

---

Notes: 
- My troubles were finding a suitable low level c library for direct writing to com ports. or writing my own *SOLVED*

- best dev enviorment: i tried multipass, virtualbox, wsl2 and found wsl2 is best for every linux dev. becuase it dosnt crash if there isnt a com device connected.

- writing to the teensy was buggy. because of the way strings were encoded in c to bytes. also cool thing i found in c if yu define a char array like char* l = {'H', 'I'} and another as char* l2 = {'B', 'Y', 'E'} and if you print the first using printf("%s", l) then something cool happens.

- debugging was another issue, i had to use print statements and check line by line which one caused a segfault.
