import array
import json

# Load JSON data
with open('./ascii_freq.json', 'r') as f:
    data = json.load(f)

freq_array = ['0.0'] * 128

# Populate frequencies for ASCII codes present in JSON
for entry in data:
    char_code = entry["Char"]
    freq = entry["Freq"]
    # Replace the corresponding index with the frequency value
    freq_array[char_code] = str(freq)

# Create C array format
c_array_str = f'#ifndef ASCII_FREQ_H\n\
#define ASCII_FREQ_H\n\
double ascii_freq[128] = {{ { ", ".join(freq_array) } }};\n\
#endif'

# Save to a .h file
with open('./ascii_freq.h', 'w') as f:
    f.write(c_array_str)

print("C array definition saved to ascii_freq.h.")