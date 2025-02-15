#!/usr/bin/env python3

import sys

if __name__ == '__main__':
    with open("../rom/dvg.rom", "rb") as f:
        data = f.read()

    words = [data[i] + (data[i + 1] << 8) for i in range(0, len(data), 2)]

    with open(sys.argv[1], "w") as f:
        f.write("#include \"../../dvg_rom.h\"\n\n")
        f.write("uint16_t dvg_rom[] = {\n")
        f.write(", ".join(f"0x{word:04x}" for word in words))
        f.write("\n};\n")
