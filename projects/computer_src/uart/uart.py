import serial
import os
from common.ret_code import RET_CODE

BAUDRATE_LIST = [
    110, 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400,
    460800, 921600, 1000000, 1500000, 2000000, 2500000, 3000000, 3500000,
    4000000
]


class UartSetting(RET_CODE):

    def __init__(self):
        self.port = "/dev/ttyUSB0"
        self.baudrate = 115200
        self.file_address = "../../bin/uart.bin"

        ###### SET Functions ######

    def set_cfg_port(self, value):
        if not isinstance(value, str):
            # raise ValueError("Invalid port")
            return RET_CODE.ERR_INVALID
        if not (value.startswith("/dev/") or value.startswith("COM")):
            # raise ValueError("Invalid port")
            return RET_CODE.ERR_INVALID

        self.port = value
        return RET_CODE.SUCCESS

    def set_cfg_baudrate(self, value):
        if value not in BAUDRATE_LIST:
            # raise ValueError("Inval id baudrate")
            # Default baudrate
            self.baudrate = 115200
            return RET_CODE.ERR_INVALID

        self.baudrate = value
        return RET_CODE.SUCCESS

    def set_cfg_file_addr(self, address: str):
        if not isinstance(address, str):
            # raise ValueError("Invalid address")
            return RET_CODE.ERR_INVALID
        if not os.path.exists(address):
            # raise FileNotFoundError("File not found")
            return RET_CODE.ERR_FNF

        self.file_address = address
        return RET_CODE.SUCCESS

    ####### GET Functions #####
    def get_cfg_baudrate(self):
        if self.baudrate is None:
            raise ValueError("Baudrate is not set")
        return self.baudrate

    def get_cfg_port(self):
        return self.port

    def get_cfg_file_addr(self):
        return self.file_address
    
    ####### Utility functions ######
    def send(self, uart_port: serial.Serial, data):
        match type(data):
            case int():
                uart_port.write(bytes([data]))

            case str():
                pass

            case _:
                uart_port.write(bytes([data]))

    def read_resp(self, uart_port: serial.Serial):
        pass


def uart_send(uart_port: serial.Serial, data) -> None:
    """
    Send data to the specified COM port and introduce a delay.

    Args:
        data_to_send (int): The data byte to be sent to the COM port.
    """
    match type(data):
        case int():
            uart_port.write(bytes([data]))

        case str():
            pass

        case _:
            uart_port.write(bytes([data]))


def uart_read_resp(uart_port) -> bool:
    """
    Check if the transmission was successful by reading the MCU's response.

    Args:
        COM (object): The COM port object used for communication.

    Returns:
        bool: True if the response indicates success, False otherwise.
    """

    return True
    delay_tick(WAIT_FOR_RES)
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
        if uart_read_resp(uart_port):
            read_success_flag = 1  # return to main app right away
            return True
        else:
            return False

    else:
        match msp_res:
            case RespondCmd.RES_DOWNLOAD_INPROG:
                print(f"Response: please wait: {msp_res}")
                # response indicates please wait, add ticks

                while not uart_read_resp(uart_port):
                    print("res code = 3 -> Waiting")

                return True

            case RespondCmd.RES_WRITE_DATA_FAIL:
                print(
                    f"Response: fail: {msp_res}. Re-send data, tries: {error_count}"
                )
                error_count += 1
                if error_count == NUMBER_OF_ATTEMPTS:
                    print("Exceed number of attempts")
                    exit()
                return False

            case RespondCmd.RES_WRITE_DATA_SUCCESS:
                error_count = 0
                return True

            case RespondCmd.RES_CRC_SUCCESS:
                error_count = 0
                print("Response: CRC success")
                return True

            case RespondCmd.RES_CRC_FAIL:
                print("Response: CRC Fail!!!")
                return False

            case RespondCmd.RES_ERASE_COMPLETE:
                error_count = 0
                print("Response: Erase complete")
                return True

            case RespondCmd.RES_APP_FLASH_CLEAN:
                error_count = 0
                print("Response: Memory is clean")
                return True

            case RespondCmd.RES_APP_FLASH_DIRTY:
                print("Response: Memory is NOT erased")
                return False

            case RespondCmd.RES_ENTER_BLD_SUCCESS:
                print("Response: Enter Bootloader success")
                return True

            case RespondCmd.RES_ENTER_BLD_FAIL:
                print("Response: Enter Bootloader fail")
                return False

            case RespondCmd.RES_EXIT_BLD_SUCCESS:
                print("Response: Exit Bootloader success")
                return True

            case RespondCmd.RES_EXIT_BLD_FAIL:
                print("Response: Exit Bootloader fail")
                return False

            case _:
                print("Response: Receive unknown response: ", msp_res)
                error_count += 1
                if error_count == NUMBER_OF_ATTEMPTS:
                    print("Reach maximum number of retries. Exiting...")
                    exit()
