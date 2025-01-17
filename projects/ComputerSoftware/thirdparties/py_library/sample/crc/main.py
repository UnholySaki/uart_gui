import sys
import os

sys.path.append(os.path.abspath("../../utils"))

from crc import *

def main():
    # Define the data
    data = "123456789"
    # Calculate the CRC
    crc = crc32(data)
    # Print the CRC
    print(f"CRC: {crc}")


if __name__ == "__main__":
    main()
