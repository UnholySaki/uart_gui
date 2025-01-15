import serial

from common.uart_code import *
from upload.send_uart import *


def hex_uart_send_erase_cmd(uart_port: serial.Serial, s_adr, size,
                        is_check_resp: bool) -> bool:
    """
    Send erase command to MCU. Including starting address and size of memory to be erased.

    Args:
        COM (object): The COM port object used for communication.
        s_adr (int/str): Start address of the memory range that to be erased
        size (int/str): Size of data to be erased

    Returns:
        bool: True if MCU is erased successfully, or memory is already clean. \
            False if not getting erase successful response.
    """
    print("====================")
    print(f"[{__name__} -> {hex_uart_send_erase_cmd.__name__}]")
    print("start adr: ", s_adr)
    print("len: ", size)
    print("====================")

    hex_uart_send_cmd(RequestCmd.RQ_CHECK_BLANKING, uart_port, s_adr, size)

    if not is_check_resp:
        return True
    # read response back
    ret = hex_uart_read_resp(uart_port, s_adr, size)
    if ret == True:
        return True

    return False


def hex_uart_send_check_blank_cmd(uart_port: serial.Serial, s_adr, size,
                              is_check_resp: bool) -> bool:
    """
    Check blanking of memory range based on starting address and memory length

    Args:
        COM (object): The COM port object used for communication.
        s_adr (int/str): Start address of the memory range that to be checked
        size (int/str): Size of data to be checked

    Returns:
        bool: True if memory range is clean. False if it is dirty.
    """

    print("====================")
    print(f"[{__name__} -> {hex_uart_send_check_blank_cmd.__name__}]")
    print("start adr: ", s_adr)
    print("len: ", size)
    print("====================")

    hex_uart_send_cmd(RequestCmd.RQ_CHECK_BLANKING, uart_port, s_adr, size)

    # read response back
    if not is_check_resp:
        return True
    ret = hex_uart_read_resp(uart_port, s_adr, size)
    if ret == True:
        return True

    return False


@staticmethod
def hex_uart_send_cmd(cmd: RequestCmd, uart_port: serial.Serial, s_adr,
                  size) -> bool:
    """
    Prepare address and size lists for sending data.
    
    Notes: Data sent including:
        1 cmd byte
        4 address bytes
        4 memory size bytes
        1 checksum byte

    Args:
        s_adr (int/str): Start address of the memory range.
        size (int/str): Size of data.

    Returns:
        tuple: Two lists containing the address and size bytes.
    """
    adr_list = [None] * 4
    size_list = [None] * 4

    match s_adr:
        case str():
            for i in range(4):
                adr_list[i] = int(s_adr[i * 2:i * 2 + 2], base=16)
        case int():
            for i in range(4):
                adr_list[i] = (s_adr >> (24 - i * 8)) & 0xFF
        case _:
            print("Invalid data type")
            return False

    match size:
        case str():
            for i in range(4):
                size_list[i] = int(size[i * 2:i * 2 + 2], base=16)
        case int():
            for i in range(4):
                size_list[i] = (size >> (24 - i * 8)) & 0xFF
        case _:
            print("Invalid data type")
            return False

    # calculate checksum
    data_str = [str(e) for e in adr_list + size_list]
    checksum = calculate_checksum_1byte(data_str)

    # send command code
    uart_send(uart_port, RequestCmd.RQ_CHECK_BLANKING)

    # send byte element in list
    for e in adr_list + size_list:
        uart_send(uart_port, e)

    # send checksum
    uart_send(uart_port, checksum)


@staticmethod
def hex_uart_read_resp(cmd: RequestCmd, uart_port, s_adr, size):
    counter = 0
    while (True):
        if (is_sent_success(uart_port)):
            return True
        else:  # retry
            print(f"Erased failed, resend. Try: {counter}")
            match cmd:
                case RequestCmd.RQ_CHECK_BLANKING:
                    hex_uart_send_check_blank_cmd(uart_port, s_adr, size)
                case RequestCmd.RQ_ERASE:
                    hex_uart_send_erase_cmd(uart_port, s_adr, size)
                case _:
                    return False
            counter += 1
            if counter == NUMBER_OF_ATTEMPTS:
                return False