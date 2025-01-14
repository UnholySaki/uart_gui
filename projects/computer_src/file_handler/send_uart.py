import time
import serial


IMAGE_TAG = 0xD1B45E82
IMAGE_START = 0x00002000
IMAGE_SIZE = 0x0000068A
IMAGE_CRC = 0x1C18B4D5
IMAGE_JUMP_ADDR = 0x00002000

RQ_KEY = 0x84AE9B23
RES_KEY = 0x18E2B8AC

NUMBER_OF_ATTEMPTS = 20  # number of resend attempt if respond returns fail
DOWNLOADING_SPEED = 7500  # test successful with 5000, x2 -> 10000 just to be safe
READ_RES_SPEED = 10000
WAIT_FOR_RES = 50000  # make it large for flash downloading and verifying

error_count = 0

class Res_cmd():
    """
    Contains the respond signal
    """
    RES_EXIT_BLD_SUCCESS = ord('c')
    RES_EXIT_BLD_FAIL = ord('d')
    RES_ENTER_BLD_SUCCESS = ord('a')
    RES_ENTER_BLD_FAIL = ord('b')
    RES_ERASE_COMPLETE = ord('8')  # Erase memory success
    RES_CRC_FAIL = ord('7')  # Check crc fail
    RES_CRC_SUCCESS = ord('6')  # download image success
    RES_WRITE_DATA_SUCCESS = ord('5')  # download line success
    RES_WRITE_DATA_FAIL = ord('4')  # download line fail - Ask for line again
    RES_DOWNLOAD_INPROG = ord('3')  # downloading in progress - Ask to wait
    RES_APP_FLASH_CLEAN = ord('2')  # main application flash is clean
    RES_APP_FLASH_DIRTY = ord('1')  # main application flash is dirty


class Rq_cmd():
    """
    ontains the request commands
    """
    RQ_DOWNLOAD = 0xF2
    RQ_ERASE = 0xF3
    RQ_CHECK_BLANKING = 0xF4
    RQ_CHECK_CRC_MODE = 0xF5
    RQ_SYSTEM_RESET = 0xF6
    RQ_GET_BLD_VER = 0xF7
    RQ_ENTER_BLD = 0xF8
    RQ_EXIT_BLD = 0xF9
    pass


def send_data(uart_port: serial.Serial, data) -> None:
    """
    Send data to the specified COM port and introduce a delay.

    Args:
        COM (object): The COM port object used for communication.
        data_to_send (int): The data byte to be sent to the COM port.
    """
    uart_port.write(bytes([data]))

    ticks = 0
    while (ticks < DOWNLOADING_SPEED):
        ticks += 1


def is_sent_success(uart_port) -> bool:
    """
    Check if the transmission was successful by reading the MCU's response.

    Args:
        COM (object): The COM port object used for communication.

    Returns:
        bool: True if the response indicates success, False otherwise.
    """
    global error_count
    read_success_flag = 0

    msp_res = ''
    try:
        if read_success_flag != 1:
            # waiting for data to come
            timeout_counter = 0
            while not uart_port.in_waiting:
                # if dont have data to read yet, set timeout
                timeout_counter += 1
                if timeout_counter >= 1500000:
                    print("Reading UART timeout. Exiting...")
                    exit()

            msp_res = uart_port.read(uart_port.in_waiting)

            if len(msp_res) == 1:
                msp_res = ord(msp_res)
            else:
                msp_res = ord(chr(msp_res[len(msp_res) - 1]))

        else:
            return True

    except ValueError:
        ticks = 0
        while (ticks < READ_RES_SPEED):
            ticks += 1
        # if read catch NULL, try read again, if fail return fail values
        if is_sent_success(uart_port):
            read_success_flag = 1  # return to main app right away
            return True
        else:
            return False

    else:
        match msp_res:
            case Res_cmd.RES_DOWNLOAD_INPROG:
                print(
                    f"Response: please wait: {msp_res}")
                # response indicates please wait, add ticks

                while not is_sent_success(uart_port):
                    print("res code = 3 -> Waiting")

                return True

            case Res_cmd.RES_WRITE_DATA_FAIL:
                print(
                    f"Response: fail: {msp_res}. Re-send data, tries: {error_count}")
                error_count += 1
                if error_count == NUMBER_OF_ATTEMPTS:
                    print("Exceed number of attempts")
                    exit()
                return False

            case Res_cmd.RES_WRITE_DATA_SUCCESS:
                error_count = 0
                return True

            case Res_cmd.RES_CRC_SUCCESS:
                error_count = 0
                print("Response: CRC success")
                return True

            case Res_cmd.RES_CRC_FAIL:
                print("Response: CRC Fail!!!")
                return False

            case Res_cmd.RES_ERASE_COMPLETE:
                error_count = 0
                print("Response: Erase complete")
                return True

            case Res_cmd.RES_APP_FLASH_CLEAN:
                error_count = 0
                print("Response: Memory is clean")
                return True

            case Res_cmd.RES_APP_FLASH_DIRTY:
                print("Response: Memory is NOT erased")
                return False

            case Res_cmd.RES_ENTER_BLD_SUCCESS:
                print("Response: Enter Bootloader success")
                return True

            case Res_cmd.RES_ENTER_BLD_FAIL:
                print("Response: Enter Bootloader fail")
                return False

            case Res_cmd.RES_EXIT_BLD_SUCCESS:
                print("Response: Exit Bootloader success")
                return True

            case Res_cmd.RES_EXIT_BLD_FAIL:
                print("Response: Exit Bootloader fail")
                return False

            case _:
                print("Response: Receive unknown response: ", msp_res)
                error_count += 1
                if error_count == NUMBER_OF_ATTEMPTS:
                    print("Reach maximum number of retries. Exiting...")
                    exit()


def get_bld_ver_cmd(uart_port, num_byte: int = 1) -> bytearray:
    """
    Read response from MCU based on defined number of byte wants to be read..

    Args:
        COM (object): The COM port object used for communication.
        num_byte (int): Number of byte that want to be read

    Returns:
        bytearray: MCU response in byte array
    """

    send_data(uart_port, Rq_cmd.RQ_GET_BLD_VER)

    time.sleep(0.01)
    try:
        return uart_port.read(num_byte)

    except ValueError:
        print("catch value error")


def send_erase_cmd(uart_port, s_adr, size) -> bool:
    """
    Send erase command to MCU. Including starting address and size of memory to be erased.
    First send blanking command, if returns dirty, erase command is sent. Otherwise, 
    The erase command is dismissed.

    Notes: Data sent including:
        1 cmd byte
        4 address bytes
        4 memory size bytes
        1 checksum byte

    Args:
        COM (object): The COM port object used for communication.
        s_adr (int/str): Start address of the memory range that to be erased
        size (int/str): Size of data to be erased

    Returns:
        bool: True if MCU is erased successfully, or memory is already clean. \
            False if not getting erase successful response.
    """

    send_check_blanking_cmd(uart_port, s_adr, size)
    # Read response
    if is_sent_success(uart_port):
        return True
    else:
        print("Wait for erasing FLASH\n")
        send_data(uart_port, Rq_cmd.RQ_ERASE)

        if type(s_adr) == str:
            # send start address of memory
            for i in range(4):
                send_data(uart_port, int(s_adr[i*2:i*2+2], base=16))
            # send size of memory range that want to be erased
            for i in range(4):
                send_data(uart_port, int(size[i*2:i*2+2], base=16))

            data_str = s_adr + size
            checksum = calculate_checksum_2byte(data_str)
            send_data(uart_port, checksum)

        elif type(s_adr) == int:
            adr_list = [None]*4
            size_list = [None]*4

            # convert from int to int list
            adr_list[0] = s_adr >> 24
            adr_list[1] = s_adr >> 16 & 0xFF
            adr_list[2] = s_adr >> 8 & 0xFF
            adr_list[3] = s_adr & 0xFF

            size_list[0] = size >> 24
            size_list[1] = size >> 16 & 0xFF
            size_list[2] = size >> 8 & 0xFF
            size_list[3] = size & 0xFF

            # send byte element in list
            for e in adr_list:
                send_data(uart_port, e)
            for e in size_list:
                send_data(uart_port, e)

            # calculate checksum
            data_str = []
            for i in range(4):
                data_str.append(str(adr_list[i]))
            for i in range(4):
                data_str.append(str(size_list[i]))

            # send checksum
            checksum = calculate_checksum_1byte(data_str)
            send_data(uart_port, checksum)

        if is_sent_success(uart_port):
            return True
        else:   # False = checksum fail
            send_erase_cmd(uart_port, s_adr, size)
            # Wait until the device completes the erase operation
            counter = 0
            while (True):
                if (is_sent_success(uart_port)):
                    return True
                else:
                    print(f"Erased failed, resend. Try: {counter}")
                    send_erase_cmd(uart_port, s_adr, size)
                    counter += 1
                    if counter == 3:
                        return False


def send_check_blanking_cmd(uart_port, s_adr, size):
    """
    Check blanking of memory range based on starting address and memory length

    Notes: Data sent including:
        1 cmd byte
        4 address bytes
        4 memory size bytes
        1 checksum byte

    Args:
        COM (object): The COM port object used for communication.
        s_adr (int/str): Start address of the memory range that to be checked
        size (int/str): Size of data to be checked

    Returns:
        bool: True if memory range is clean. False if it is dirty.
    """
    print("====================")
    print("start adr: ", s_adr)
    print("len: ", size)
    print("====================")

    send_data(uart_port, Rq_cmd.RQ_CHECK_BLANKING)

    if type(s_adr) == str:
        # send start address of memory
        for i in range(4):
            send_data(uart_port, int(s_adr[i*2:i*2+2], base=16))
        # send size of memory range that want to be erased
        for i in range(4):
            send_data(uart_port, int(size[i*2:i*2+2], base=16))

        data_str = s_adr + size
        checksum = calculate_checksum_2byte(data_str)
        send_data(uart_port, checksum)

    elif type(s_adr) == int:
        adr_list = [None]*4
        size_list = [None]*4

        # convert from int to int list
        adr_list[0] = s_adr >> 24
        adr_list[1] = s_adr >> 16 & 0xFF
        adr_list[2] = s_adr >> 8 & 0xFF
        adr_list[3] = s_adr & 0xFF

        size_list[0] = size >> 24
        size_list[1] = size >> 16 & 0xFF
        size_list[2] = size >> 8 & 0xFF
        size_list[3] = size & 0xFF

        # send byte element in list
        for e in adr_list:
            send_data(uart_port, e)
        for e in size_list:
            send_data(uart_port, e)

        # calculate checksum
        data_str = []
        for i in range(4):
            data_str.append(str(adr_list[i]))
        for i in range(4):
            data_str.append(str(size_list[i]))

        # send checksum
        checksum = calculate_checksum_1byte(data_str)
        send_data(uart_port, checksum)


def send_crc_cmd(uart_port, s_adr, size,  crc: int) -> None:
    """
    Check crc of memory range based on starting address and memory length

    Notes: Data sent including:
        1 cmd byte
        4 address bytes
        4 memory size bytes
        4 crc bytes (crc32)
        1checksum byte

    Args:
        COM (object): The COM port object used for communication.
        s_adr (int/str): Start address of the memory range that to be checked
        size (int/str): Size of data to be checked
        crc (int): Crc value to be checked
    """
    # send commmand
    send_data(uart_port, Rq_cmd.RQ_CHECK_CRC_MODE)

    # prepare to send data
    if type(s_adr) == str:
        # send start address of memory
        for i in range(4):
            send_data(uart_port, int(s_adr[i*2:i*2+2], base=16))
        # send size of memory range that want to be erased
        for i in range(4):
            send_data(uart_port, int(size[i*2:i*2+2], base=16))
        for i in range(4):
            send_data(uart_port, int(crc[i*2:i*2+2], base=16))

        # send checksum
        data_str = s_adr + size + crc
        checksum = calculate_checksum_2byte(data_str)
        send_data(uart_port, checksum)

    # send size of memory range that want to be check
    if type(s_adr) == int:
        adr_list = [None]*4
        size_list = [None]*4
        crc_list = [None]*4

        # convert from int to int list
        adr_list[0] = s_adr >> 24
        adr_list[1] = s_adr >> 16 & 0xFF
        adr_list[2] = s_adr >> 8 & 0xFF
        adr_list[3] = s_adr & 0xFF

        size_list[0] = size >> 24
        size_list[1] = size >> 16 & 0xFF
        size_list[2] = size >> 8 & 0xFF
        size_list[3] = size & 0xFF

        crc_list[0] = crc >> 24
        crc_list[1] = crc >> 16 & 0xFF
        crc_list[2] = crc >> 8 & 0xFF
        crc_list[3] = crc & 0xFF

        # send byte element in list
        for e in adr_list:
            send_data(uart_port, e)
        for e in size_list:
            send_data(uart_port, e)
        for e in crc_list:
            send_data(uart_port, e)

        # calculate checksum
        data_str = []
        for i in range(4):
            data_str.append(str(adr_list[i]))
        for i in range(4):
            data_str.append(str(size_list[i]))
        for i in range(4):
            data_str.append(str(crc_list[i]))

        checksum = calculate_checksum_1byte(data_str)
        send_data(uart_port, checksum)


def send_enter_bld_cmd(uart_port, rq_key: int = RQ_KEY) -> None:
    """
    Send enter bootloader flag

    Notes: Data sent including:
        1 cmd byte
        4 request key bytes
        1checksum byte

    Args:
        COM (object): The COM port object used for communication.
        rq_key (int): Request key
    """
    send_data(uart_port, Rq_cmd.RQ_ENTER_BLD)

    # convert from int to int list
    if type(rq_key == int):
        rq_key_list = [None]*4

        rq_key_list[0] = rq_key >> 24
        rq_key_list[1] = rq_key >> 16 & 0xFF
        rq_key_list[2] = rq_key >> 8 & 0xFF
        rq_key_list[3] = rq_key & 0xFF

        # send key bytes
        for e in rq_key_list:
            send_data(uart_port, e)

    # calculate checksum
    data_str = []
    for i in range(4):
        data_str.append(str(rq_key_list[i]))

    # Send checksum
    checksum = calculate_checksum_1byte(data_str)
    send_data(uart_port, checksum)


def read_enter_bld_response(uart_port):
    """
    Read response after sending enter bootloader command

    Returns:
        bool: True if enter successful, False otherwise
    """
    if is_sent_success(uart_port):
        return True
    else:   # False = checksum fail
        send_enter_bld_cmd(uart_port)
        # resend the command
        num_try = 0
        while (True):
            if (is_sent_success(uart_port)):
                return True
            else:
                print(f"Enter failed, resend. Try: {num_try}")
                send_enter_bld_cmd(uart_port)
                num_try += 1
                if num_try == 3:
                    return False


def send_exit_bld_cmd(uart_port, res_key: int = RES_KEY) -> None:
    """
    Send exit bootloader flag

    Notes: Data sent including:
        1 cmd byte
        4 response key bytes
        1checksum byte

    Args:
        COM (object): The COM port object used for communication.
        response_key (int): response key
    """
    # send first byte command
    send_data(uart_port, Rq_cmd.RQ_EXIT_BLD)

    # convert from int to int list
    if type(res_key == int):
        res_key_list = [None]*4

        res_key_list[0] = res_key >> 24
        res_key_list[1] = res_key >> 16 & 0xFF
        res_key_list[2] = res_key >> 8 & 0xFF
        res_key_list[3] = res_key & 0xFF

        # send key bytes
        for e in res_key_list:
            send_data(uart_port, e)

    # calculate checksum
    data_str = []
    for i in range(4):
        data_str.append(str(res_key_list[i]))

    # Send checksum
    checksum = calculate_checksum_1byte(data_str)
    send_data(uart_port, checksum)


def read_exit_bld_response(uart_port) -> bool:
    """
    Read response after sending exit bootloader command

    Returns:
        bool: True if exit successful, False otherwise
    """
    if is_sent_success(uart_port):
        return True
    else:   # False = checksum fail
        # resend the command
        send_exit_bld_cmd(uart_port)
        num_try = 0
        while (True):
            if (is_sent_success(uart_port)):
                return True
            else:
                print(f"Enter failed, resend. Try: {num_try}")
                send_exit_bld_cmd(uart_port)
                num_try += 1
                if num_try == 3:
                    return False


def calculate_checksum_1byte(data_str_list):
    """
    Calculate checksum for data string that contains 1 byte per element
    """
    total_checksum = 0

    # half of len because read 2 chars at a time
    for e in data_str_list:
        total_checksum += int(e, base=10)

    # %256 for return to 0 if reach 256
    calc_checksum = (256-total_checksum % 256) % 256
    return calc_checksum


def calculate_checksum_2byte(data_str):
    """
    Calculate checksum for data string that contains 2 byte per element
    """
    j = 0
    total_checksum = 0

    # half of len because read 2 chars at a time
    for _ in range(int(len(data_str)/2)):
        total_checksum += int(data_str[j:j+2], base=16)
        j += 2

    # %256 for return to 0 if reach 256
    calc_checksum = (256-total_checksum % 256) % 256
    return calc_checksum


def verify_checksum(data_str: str, ref_cs: str) -> bool:
    """
    Verify the checksum of a given data string against a reference checksum.

    Args:
        data_str (str): The data string for which the checksum is to be verified. It should be in hexadecimal format.
        ref_cs (str): The reference checksum to compare against. It should be in hexadecimal format.

    Returns:
        bool: True if the calculated checksum matches the reference checksum, False otherwise.
    """

    calc_checksum = calculate_checksum_2byte(data_str)

    if (calc_checksum == int(ref_cs, base=16)):
        # if success, do nothing, else exit the application
        return True
    else:
        print("\nData string is: ", data_str, len(data_str)/2)
        return False


def delay_tick(time: int) -> None:
    """
    Simple delay function that simulates a delay for a specified number of ticks.

    Args:
        time (int): The number of ticks to delay.
    """
    tick = 0
    while (tick <= time):
        tick += 1