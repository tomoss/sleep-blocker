import sys
import os

input_file  = sys.argv[1]
output_file = sys.argv[2]
var_name    = sys.argv[3]

os.makedirs(os.path.dirname(output_file), exist_ok=True)  # create directory if needed

with open(input_file, "rb") as f:
    data = f.read()

with open(output_file, "w") as f:
    f.write(f"#pragma once\n")
    f.write(f"#include <cstddef>\n")
    f.write(f"inline constexpr unsigned char {var_name}[] = {{\n")
    for i, byte in enumerate(data):
        f.write(f"0x{byte:02x},")
        if (i + 1) % 16 == 0:
            f.write("\n")
    f.write(f"\n}};\n")
    f.write(f"inline constexpr std::size_t {var_name}_len = {len(data)};\n")