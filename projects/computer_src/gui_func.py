import tkinter as tk
from tkinter import filedialog
from uart import UartSetting


def open_setting_window():
    setting_root = tk.Toplevel()
    setting_root.title("Setting")
    setting_root.geometry("300x200")

    setting_root.mainloop()


def save_setting(uart_obj: UartSetting, port, baudrate, bin_adr,
                 display_field):
    uart_obj.set_port(port.get())
    uart_obj.set_baudrate(int(baudrate.get()))
    uart_obj.set_file_address(bin_adr.get())
    new_setting = "New setting:\n"
    new_setting += "Port: " + uart_obj.get_port() + "\n"
    new_setting += "Baudrate: " + str(uart_obj.get_baudrate()) + "\n"
    new_setting += "Bin address: " + str(uart_obj.get_file_address())
    update_field(new_setting, display_field)


def update_field(text, dbox):
    if not text:
        return
    dbox.config(state=tk.NORMAL)
    dbox.delete(1.0, tk.END)
    dbox.insert(tk.END, text)
    dbox.config(state=tk.DISABLED)


def open_file(uart_obj, dbox):
    file_path = filedialog.askopenfilename()
    if file_path:
        with open(file_path, "r") as file:
            content = file.read()
            dbox.config(state=tk.NORMAL)
            dbox.delete(1.0, tk.END)
            dbox.insert(tk.END, content)
            dbox.config(state=tk.DISABLED)

        uart_obj.set_file_address(file_path)


def save_file(display_box):
    file_path = filedialog.asksaveasfilename(
        defaultextension=".txt",
        filetypes=[("Text files", "*.txt"), ("All files", "*.*")],
    )
    if file_path:
        with open(file_path, "w") as file:
            content = display_box.get(1.0, tk.END)
            file.write(content)
