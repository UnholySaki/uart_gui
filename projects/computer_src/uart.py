import sys
import os
sys.path.append(os.path.abspath("../inc"))



class UartSetting():
    def __init__(self):
        self.port = "/dev/ttyUSB0"
        self.baudrate = 115200

    def get_baudrate(self):
        return self.baudrate

    def set_baudrate(self, value):
        if value in [9600, 19200, 38400, 57600, 115200]:  # Example validation
            self.baudrate = value
        else:
            raise ValueError("Invalid baudrate value")

    def get_port(self):
        return self.port

    def set_port(self, value):
        if isinstance(value, str):
            if value.startswith("/dev/") or value.startwith("COM"):
                self.port = value
                return
        raise ValueError("Invalid port value")
    