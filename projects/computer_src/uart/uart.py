import sys
import os

sys.path.append(os.path.abspath(".."))

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
