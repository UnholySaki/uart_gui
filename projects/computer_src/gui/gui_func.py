import tkinter as tk
from tkinter import filedialog
from tkinter import dialog

from uart.uart import *
from file_handler.read_hex_file import *


def open_setting_window():
    setting_root = tk.Toplevel()
    setting_root.title("Setting")
    setting_root.geometry("300x200")

    setting_root.mainloop()


def save_setting(uart_obj: UartSetting, port, baudrate, bin_adr,
                 display_field):
    port_value = port.get()
    if not port_value:
        err_popup()
        return
    uart_obj.set_cfg_port(port_value)

    baudrate_value = int(baudrate.get())
    if not baudrate_value:
        err_popup()
        return
    uart_obj.set_cfg_baudrate(baudrate_value)

    file_address = bin_adr.get()
    if not file_address:
        err_popup()
        return
    uart_obj.set_cfg_file_addr(file_address)

    new_setting = "New setting:\n"
    new_setting += "Port: " + uart_obj.get_cfg_port() + "\n"
    new_setting += "Baudrate: " + str(uart_obj.get_cfg_baudrate()) + "\n"
    new_setting += "Bin address: " + str(uart_obj.get_cfg_file_addr())
    update_field(new_setting, display_field)


def update_field(text, dbox):
    if not text:
        return
    dbox.config(state=tk.NORMAL)
    dbox.delete(1.0, tk.END)
    dbox.insert(tk.END, text)
    dbox.config(state=tk.DISABLED)


def open_file(uart_obj: UartSetting, dbox: tk.Entry):
    file_path = filedialog.askopenfilename()
    if file_path:
        with open(file_path, "r") as file:
            content = file.read()
            dbox.config(state=tk.NORMAL)
            dbox.delete(1.0, tk.END)
            dbox.insert(tk.END, content)
            dbox.config(state=tk.DISABLED)

        uart_obj.set_cfg_file_addr(file_path)


def save_file(display_box):
    file_path = filedialog.asksaveasfilename(
        defaultextension=".txt",
        filetypes=[("Text files", "*.txt"), ("All files", "*.*")],
    )
    if file_path:
        with open(file_path, "w") as file:
            content = display_box.get(1.0, tk.END)
            file.write(content)


def err_popup(title="Error", text="Please enter the file address"):
    dialog.Dialog(title=title,
                  text=text,
                  bitmap=dialog.DIALOG_ICON,
                  default=0,
                  strings=("OK", ))


def upload_data(uart_obj: UartSetting):
    # Open serial port
    try:
        uart_serial = serial.Serial(port=uart_obj.get_cfg_port(),
                                    baudrate=uart_obj.get_cfg_baudrate(),
                                    timeout=1)
    except serial.SerialException:
        err_popup("Error", "Port not exist")
        return
    
    upload_bin(uart_serial_port=uart_serial,
                  file_path="releases\Application.hex")
