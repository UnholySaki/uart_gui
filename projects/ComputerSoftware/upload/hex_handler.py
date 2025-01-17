import time
import os
from upload.common import MemoryMap
from upload.hex_uart_cmd import *
import thirdparties.py_library.utils.crc as crc_32
from common.memory_map import *
from common.intel_hex_code import *

LINE_TO_READ = 9999  # define number of line to read and sent
KB = 1024  # size of KB


mem_map = MemoryMap()

def hex_upload_file(uart_serial_port: serial.Serial,
                    file_path: str,
                    check_crc: int = True) -> None:
    print("==============Upload start===================")
    # 1. Verify the bin file
    ret = hex_verify_file(file_path)
    if not ret:
        print("Error in verifying the file")
        exit()

    print("==============Verify complete===================")
    # 2. Send erase
    erase_len = mem_map.END_ADDR - mem_map.START_ADDR + mem_map.END_ADDR_OFFSET
    erase_len = int((erase_len + mem_map.SECTOR_SIZE - 1) /
                    mem_map.SECTOR_SIZE) * mem_map.SECTOR_SIZE

    success = hex_uart_send_erase_cmd(uart_serial_port, mem_map.START_ADDR,
                                      erase_len, False)
    if not success:
        print("Fail to erase before downloading")
        exit()

    print("==============Image is cleared===================")
    # 3. data is read and sent
    hex_file = open(file_path, 'rb')

    # read data
    for line in hex_file:  # using line loop for check EOF
        # read line and send it
        line_list = parse_hex_line(line)
        store_hex_line_data(line_list)
        hex_uart_send_line(uart_serial_port, line_list)

        # retry
        while True:
            delay_tick(WAIT_FOR_RES)
            if (uart_read_resp(uart_serial_port)):
                break
            else:
                print("resend line...")
                hex_uart_send_line(uart_serial_port, line_list)

    print("==============Upload success===================")
    # 4. CRC_check
    if not check_crc:
        hex_file.close()
        print("upload complete. Exiting")
        return

    ret = hex_uart_send_crc_ovr(uart_serial_port, hex_file)
    if ret < 0:
        print("[Fail] Check CRC whole image failed, retry for each sector")
        hex_uart_send_crc_sector(uart_serial_port, hex_file)
    print("==============Verify success===================")

    # 5. Exiting
    hex_file.close()
    print("==============Done uploading===================")


@staticmethod
def hex_verify_file(file_path: str) -> bool:
    """
    1. Check if the data is valid
    2. Determine the memory map
    """
    try:
        with open(file_path, 'rb') as hex_file:
            hex_file.seek(0, 0)  # return cursor to start of line
            for line in hex_file:
                line_list = parse_hex_line(line)

                # check start code
                if line_list[0] != ":":
                    print("error, first byte is not \":\"")
                    return False

                # check checksum
                checksum_data = str(line_list[1] + line_list[2] +
                                    line_list[3] + line_list[4])
                if not verify_checksum(checksum_data, line_list[5]):
                    print("checksum not pass, recheck the HEX file")
                    return False

                hex_det_mem_map(line_list)

    except FileNotFoundError:
        print(f"File not found: {file_path}")
        return False

    print(f"Memory Map:\
            \nSTART_ADDR={mem_map.START_ADDR},\
            \nEND_ADDR={mem_map.END_ADDR},\
            \nEND_ADDR_OFFSET={mem_map.END_ADDR_OFFSET},\
            \nmain_addr={mem_map.main_addr}")
    return True


@staticmethod
def hex_det_mem_map(line_list: list) -> None:
    """
    determine start and end address
    """
    num_of_bytes, addr, type, data = line_list[1:5]

    print(addr, int(addr, base=16))
    full_address = mem_map.main_addr + int(addr, base=16)
    match int(type, base=16):
        case RecordType.EXT_LIN_ADDR:
            # determine the start address
            if data != None:
                mem_map.main_addr = int(data, base=16) << 4
        case RecordType.DATA:
            if mem_map.is_first_read_attempt:
                mem_map.START_ADDR = full_address
                mem_map.is_first_read_attempt = False
                return

            if mem_map.START_ADDR >= full_address:
                mem_map.START_ADDR = full_address
            if mem_map.END_ADDR <= full_address:
                mem_map.END_ADDR = full_address
                mem_map.END_ADDR_OFFSET = int(num_of_bytes, 16)


@staticmethod
def store_hex_line_data(line_list: list) -> None:
    num_of_bytes, addr, type, data = line_list[1:5]

    cur_addr = mem_map.main_addr + int(addr, base=16)
    # write to memory bin
    match int(type, base=16):
        case RecordType.DATA:
            j = 0
            # First read attempt to initialize data address value
            for i in range(int(num_of_bytes, base=16)):
                mem_map.image_str[cur_addr - mem_map.START_ADDR +
                                  i] = data[j:j + 2]
                j += 2
        case _:
            pass

    return


@staticmethod
def hex_uart_send_line(u_port, line_list: list):
    """
    Algorithm to send hex line to UART COM

    Args:
        data_str (str): Individual line from the hex file.
    """
    data = 0
    counter = 0
    line_str = str(line_list[1] + line_list[2] + line_list[3] + line_list[4] +
                   line_list[5])
    # raise is_downloading flag
    uart_send(u_port, RequestCmd.RQ_DOWNLOAD)
    # send data
    for _ in range(int(len(line_str) / 2)):
        data = int(line_str[counter:counter + 2], base=16)
        counter += 2
        uart_send(u_port, data)


@staticmethod
def hex_uart_send_crc_ovr(u_port: serial.Serial) -> int:
    """
    check the whole image
    """
    crc_start_addr = mem_map.START_ADDR

    # print(mem_map.image_str)
    # build memory list with each element is a byte
    # convert from string to byte becuase crc32 check on bytes
    for element in mem_map.image_str:
        mem_map.image_byte_list.append(int(element, base=16))

    # check crc whole image first
    end_ptr = mem_map.END_ADDR - mem_map.START_ADDR + mem_map.END_ADDR_OFFSET
    image_crc = crc_32.crc32(mem_map.image_byte_list[:end_ptr])

    print("====================")
    print("start adr: ", mem_map.START_ADDR)
    print("end adr: ", mem_map.END_ADDR)
    print("end address offset: ", mem_map.END_ADDR_OFFSET)
    print("end pointer is: ", end_ptr)
    print("image crc: ", image_crc)
    print("====================")

    send_crc_cmd(u_port, crc_start_addr, end_ptr, image_crc)

    # if fail, check crc for each individual sector
    if uart_read_resp(u_port):
        return RET_CODE.CRC_PASS
    else:
        return RET_CODE.CRC_FAIL


def hex_uart_send_crc_sector(u_port: serial.Serial, hex_file) -> int:
    element_size = 1  # each element is 2 bytes
    chunk_size = int(KB / element_size)

    for crc_start_addr in range(mem_map.START_ADDR,
                                mem_map.END_ADDR + mem_map.END_ADDR_OFFSET,
                                chunk_size):
        end_ptr = crc_start_addr + chunk_size
        if end_ptr > mem_map.END_ADDR + mem_map.END_ADDR_OFFSET:
            end_ptr = mem_map.END_ADDR + mem_map.END_ADDR_OFFSET
        crc_len = end_ptr - crc_start_addr

        sector_crc = crc_32.crc32(
            mem_map.image_byte_list[crc_start_addr:crc_start_addr + crc_len])

        print("Prepare to send: ", crc_start_addr, crc_len)
        send_crc_cmd(u_port, crc_start_addr, crc_len, sector_crc)

        delay_tick(WAIT_FOR_RES)

        if uart_read_resp(u_port) == False:
            handle_crc_check_failure(u_port, hex_file, crc_start_addr, end_ptr,
                                     crc_start_addr, chunk_size)


def handle_crc_check_failure(u_port, hex_file, start_ptr, end_ptr,
                             crc_start_addr, chunk_size):
    time_out = 0

    hex_uart_send_erase_cmd(u_port, crc_start_addr, mem_map.SECTOR_SIZE)

    while True:
        hex_file.seek(0, 0)
        for line in hex_file:
            data_str = read_hex_sector(line, hex_file, start_ptr)
            if data_str:
                hex_uart_send_line(u_port, data_str)

        crc_len = end_ptr - start_ptr
        sector_crc = crc_32.crc32(mem_map.image_byte_list[start_ptr:start_ptr +
                                                          crc_len])

        send_crc_cmd(u_port, crc_start_addr, crc_len, sector_crc)
        delay_tick(WAIT_FOR_RES)

        if uart_read_resp(u_port):
            if is_reach_end:
                print("Read end, exiting")
                exit()
            start_ptr, end_ptr, is_reach_end = move_ptr(start_ptr, end_ptr, KB)
            time_out = 0
            break
        else:
            time_out += 1
        if time_out >= 10:
            print("CRC fails, exiting")
            exit()


@staticmethod
def move_ptr(s_adr: int, e_adr: int, sector_size) -> list:
    """
    Move the pointer between start and end addresses, ensuring the pointer does not exceed 
    the image's end address. Adjusts the pointers based on the sector size.

    Args:
        s_adr (int): The current start address pointer.
        e_adr (int): The current end address pointer.
        sector_size (int): The size of the sector to process.

    Returns:
        list: A list containing:
            - The updated start address (`s_adr`).
            - The updated end address (`e_adr`).
            - A boolean flag `is_reach_end` indicating if the pointer reached the image's end.
    """
    global START_ADR, END_ADR, END_ADR_OFFSET
    element_size = 1  # each element is 2 bytes
    chunk_size = int(sector_size / element_size)
    is_reach_end = False

    s_adr = e_adr
    e_adr = s_adr + chunk_size
    # check if end address reach end
    image_end_address = (int(END_ADR, 16) - int(START_ADR, 16))

    if (e_adr > image_end_address):
        e_adr = image_end_address + END_ADR_OFFSET
        is_reach_end = True

    print(s_adr, e_adr, image_end_address)
    return [s_adr, e_adr, is_reach_end]


@staticmethod
def read_hex_sector(current_line: str, hex_file: str, start_ptr: int) -> str:
    """
    Reads and processes a sector of hex data from the given file

    Args:
        current_line (str): The current line of the hex file being processed.
        hex_file (str): A file-like object representing the hex file to be read.
        start_ptr (int): The starting pointer in the sector, provided as an integer.

    Returns:
        str or None: Returns the data string if the current line is part of the target sector,
        otherwise returns `None` if the line does not belong to the sector.
    """

    global START_ADR, main_address, byte_len

    hex_file.seek(-len(current_line), 1)
    data_list = parse_hex_line(hex_file, len(current_line))

    start_code, byte_count, line_adr, record_type, datapart, checksum, eol = data_list

    data_string = byte_count + line_adr + record_type + datapart + checksum

    full_address = ""
    offset_address = 0
    if record_type == "04":
        main_address = datapart
    # if indicating data, check if current line is on the target sector
    elif record_type == "00":
        # First read attempt to initialize data address value
        full_address = main_address + line_adr

        offset_address = int(full_address, 16) - int(START_ADR, 16)

    if record_type != "01":
        byte_len = int(byte_count[:2], 16)
        # in case of start ptr in inside a hex line, not the beginning
        if ((offset_address < start_ptr and
             (offset_address + byte_len > start_ptr))
                or offset_address >= start_ptr):
            # limit to 1 sector
            if offset_address < (start_ptr + KB):
                return data_string

    elif record_type == "01":
        return data_string

    # if not in sector, return None
    return None


@staticmethod
def parse_hex_line(data: str) -> list:
    """
    Parse a hex line from the given file and return its components as a list.

    Args:
        data (str): The data string to be parsed.
        
    Returns:
        List[str]: A list containing the parsed components of the hex line:
            - list[0]: The start delimiter, always ":".
            - list[1]: The byte count of the data part.
            - list[2]: The address where the data is stored.
            - list[3]: The record type (e.g., data, end of file).
            - list[4]: The data part of the line.
            - list[5]: The checksum of the data.
            - list[6]: End of line characters (e.g., "\r\n").
    """
    len = int(data[1:3].decode(), base=16) * 2

    return [
        data[0:1].decode(),  # Read ":"
        data[1:3].decode(),  # Read byte count
        data[3:7].decode(),  # Read address
        data[7:9].decode(),  # Read record type
        data[9:9 +
             len].decode() if data[1:3].decode() != "00" else "",  # Read data
        data[9 + len:11 + len].decode(),  # Read checksum
        data[11 + len:].decode()  # Bypass /r/n
    ]
