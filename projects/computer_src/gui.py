import tkinter as tk
from tkinter import ttk
from tkinter import filedialog


import sys
import os

sys.path.append(os.path.abspath("../inc"))


def init_gui():
    # Create the main window
    root = tk.Tk()
    root.title("Tkinter Input and Display")

    # Create an input box
    input_box = tk.Entry(root, width=50)
    input_box.grid(row=0, column=0, padx=10, pady=10)

    # Create a display box
    general_box = tk.Text(root, width=30, height=10, state=tk.DISABLED)
    general_box.grid(row=1, column=0, columnspan=3, padx=10, pady=10)

    info_box = tk.Text(root, width=20, height=10, state=tk.DISABLED)
    info_box.grid(row=1, column=3, columnspan=2, padx=10, pady=10)

    # Create a button to display the input text
    display_button = tk.Button(
        root, text="Display", command=lambda: upd_general_box(input_box, general_box)
    )
    display_button.grid(row=0, column=1, padx=10, pady=5)

    # Create a button to open a file
    open_button = tk.Button(
        root, text="Open File", command=lambda: open_file(general_box)
    )
    open_button.grid(row=2, column=0, padx=10, pady=5)

    # Create a button to save the content to a file
    save_btn = tk.Button(root, text="Save File", command=lambda: save_file(general_box))
    save_btn.grid(row=2, column=1, padx=10, pady=5)

    setting_btn = tk.Button(root, text="Settings", command=lambda: open_setting_window())
    setting_btn.grid(row=2, column=2, padx=10, pady=5)

    return root


def open_setting_window():
    INPUT_BOX_WIDTH = 25
    LABEL_WIDTH = 10
    PADX = 10
    PADY = 5

    setting_root = tk.Toplevel()
    setting_root.title("Setting")
    setting_root.geometry("300x200")

    serial_label = tk.Label(setting_root, text="Serial Port", width=LABEL_WIDTH)
    serial_label.grid(row=0, column=0, padx=PADX, pady=PADY, sticky=tk.W)

    serial_entry = tk.Entry(setting_root, width=INPUT_BOX_WIDTH)
    serial_entry.grid(row=0, column=1, padx=PADX, pady=PADY, sticky=tk.E)

    baudrate_label = tk.Label(setting_root, text="Baudrate", width=LABEL_WIDTH)
    baudrate_label.grid(row=1, column=0, padx=PADX, pady=PADY, sticky=tk.W)

    baudrate_entry = tk.Entry(setting_root, width=INPUT_BOX_WIDTH)
    baudrate_entry.grid(row=1, column=1, padx=PADX, pady=PADY, sticky=tk.E)

    info_box = tk.Text(
        setting_root, width=LABEL_WIDTH + INPUT_BOX_WIDTH, height=2, state=tk.DISABLED
    )
    info_box.grid(row=2, column=0, columnspan=2, padx=PADX, pady=PADY)

    save_setting_btn = tk.Button(
        setting_root,
        text="Save Settings",
        command=lambda: upd_general_box(serial_entry, info_box),
    )
    save_setting_btn.grid(row=3, column=0, columnspan=2, padx=10, pady=10, sticky=tk.S)

    info_box.config(state=tk.NORMAL)
    info_box.insert(tk.END, "Setting")
    info_box.config(state=tk.DISABLED)

    setting_root.mainloop()


def save_setting():
    pass


def upd_general_box(inbox, dbox):
    input_text = inbox.get()
    dbox.config(state=tk.NORMAL)
    dbox.delete(1.0, tk.END)
    dbox.insert(tk.END, input_text)
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
