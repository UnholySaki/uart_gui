import tkinter as tk
from tkinter import ttk
from tkinter import filedialog
from uart import *

import sys
import os

sys.path.append(os.path.abspath("../inc"))

global uart_setting
uart_setting = UartSetting()

INPUT_BOX_WIDTH = 24
LABEL_WIDTH = 6
PADX = 10
PADY = 5

UART_CV_WIDTH = int((INPUT_BOX_WIDTH + LABEL_WIDTH + PADX / 10) * 10)
UART_CV_HEIGHT = 300

GEN_CV_WIDTH = 500
GEN_CV_HEIGHT = UART_CV_HEIGHT

ROOT_WIDTH = UART_CV_WIDTH + GEN_CV_WIDTH + 2*PADX
ROOT_HEIGHT = UART_CV_HEIGHT + PADY

def init_gui():
    ######### Overall Setup ##########
    # Create the main window
    root = tk.Tk()
    root.title("Tkinter Input and Display")
    root.geometry(f"{ROOT_WIDTH}x{ROOT_HEIGHT}")
    root.resizable(False, False)

    # Create a canvas for UART settings
    uart_canvas = tk.Canvas(root, width=UART_CV_WIDTH, height=UART_CV_HEIGHT)
    uart_canvas.grid(row=0, column=0, padx=PADX, pady=PADY, sticky="nsew")

    uart_frame = tk.Frame(uart_canvas)
    uart_canvas.create_window((0, 0), window=uart_frame, anchor="nw")

    # Create a canvas for general input and display
    general_canvas = tk.Canvas(root, width=GEN_CV_WIDTH, height=GEN_CV_HEIGHT)
    general_canvas.grid(row=0, column=1, padx=PADX, pady=PADY, sticky="nsew")

    general_frame = tk.Frame(general_canvas)
    general_canvas.create_window((0, 0), window=general_frame, anchor="nw")

    ######### UART window Setup ##########
    # UART settings widgets
    serial_label = tk.Label(uart_frame, text="Serial Port", width=LABEL_WIDTH)
    serial_label.grid(row=0, column=0, padx=PADX, pady=PADY, sticky=tk.W)

    port_entry = tk.Entry(uart_frame, width=INPUT_BOX_WIDTH)
    port_entry.grid(row=0, column=1, padx=PADX, pady=PADY, sticky=tk.E)

    baudrate_label = tk.Label(uart_frame, text="Baudrate", width=LABEL_WIDTH)
    baudrate_label.grid(row=1, column=0, padx=PADX, pady=PADY, sticky=tk.W)

    baudrate_entry = tk.Entry(uart_frame, width=INPUT_BOX_WIDTH)
    baudrate_entry.grid(row=1, column=1, padx=PADX, pady=PADY, sticky=tk.E)

    info_box = tk.Text(
        uart_frame, width=LABEL_WIDTH + INPUT_BOX_WIDTH, height=4, state=tk.DISABLED
    )
    info_box.grid(row=2, column=0, columnspan=2, padx=PADX, pady=PADY)

    save_setting_btn = tk.Button(
        uart_frame,
        text="Save Settings",
        command=lambda: save_setting(port_entry, baudrate_entry, info_box),
    )
    save_setting_btn.grid(row=3, column=0, columnspan=2, padx=10, pady=10, sticky=tk.S)

    ######### General window Setup ##########
    # General input and display widgets
    file_address_box = tk.Entry(general_frame, width=40)
    file_address_box.grid(row=0, column=0, padx=10, pady=5)

    open_file_btn = tk.Button(
        general_frame, text="Open File", command=lambda: open_file(general_box)
    )
    open_file_btn.grid(row=0, column=1, padx=10, pady=5)

    general_box = tk.Text(general_frame, height=8, state=tk.DISABLED)
    general_box.grid(row=1, column=0, columnspan=3, padx=10, pady=10)

    display_button = tk.Button(
        general_frame,
        text="Display",
        command=lambda: update_field(file_address_box.get(), general_box),
    )
    display_button.grid(row=0, column=2, padx=10, pady=5)

    save_btn = tk.Button(
        general_frame, text="Save File", command=lambda: save_file(general_box)
    )
    save_btn.grid(row=2, column=0, padx=10, pady=5)

    setting_btn = tk.Button(
        general_frame, text="Settings", command=lambda: open_setting_window()
    )
    setting_btn.grid(row=2, column=1, padx=10, pady=5)

    ######### Update content #########
    # Update the UART setting
    port_entry.insert(tk.END, uart_setting.get_port())
    baudrate_entry.insert(tk.END, str(uart_setting.get_baudrate()))

    info_box.config(state=tk.NORMAL)
    info_box.insert(tk.END, "Setting\n")
    info_box.config(state=tk.DISABLED)

    return root


def open_setting_window():
    setting_root = tk.Toplevel()
    setting_root.title("Setting")
    setting_root.geometry("300x200")

    setting_root.mainloop()


def save_setting(port, baudrate, display_field):
    uart_setting.set_port(port.get())
    uart_setting.set_baudrate(int(baudrate.get()))
    new_setting = "New setting:\n"
    new_setting += "Port: " + uart_setting.get_port() + "\n"
    new_setting += "Baudrate: " + str(uart_setting.get_baudrate())
    update_field(new_setting, display_field)


def update_field(text, dbox):
    dbox.config(state=tk.NORMAL)
    dbox.delete(1.0, tk.END)
    dbox.insert(tk.END, text)
    dbox.config(state=tk.DISABLED)


def open_file(display_box):
    file_path = filedialog.askopenfilename()
    if file_path:
        with open(file_path, "r") as file:
            content = file.read()
            display_box.config(state=tk.NORMAL)
            display_box.delete(1.0, tk.END)
            display_box.insert(tk.END, content)
            display_box.config(state=tk.DISABLED)


def save_file(display_box):
    file_path = filedialog.asksaveasfilename(
        defaultextension=".txt",
        filetypes=[("Text files", "*.txt"), ("All files", "*.*")],
    )
    if file_path:
        with open(file_path, "w") as file:
            content = display_box.get(1.0, tk.END)
            file.write(content)
