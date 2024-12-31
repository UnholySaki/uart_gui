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
    input_box = ttk.Entry(root, width=50)
    input_box.grid(row=0, column=0, padx=10, pady=10)

    # Create a display box
    display_box = tk.Text(root, width=50, height=10, state=tk.DISABLED)
    display_box.grid(row=1, column=0, padx=10, pady=10)

    # Create a button to display the input text
    display_button = ttk.Button(
        root, text="Display", command=lambda: display_text(input_box, display_box))
    display_button.grid(row=0, column=1, padx=10, pady=5)
    # Create a button to open a file
    open_button = ttk.Button(
        root, text="Open File", command=lambda: open_file(display_box))
    open_button.grid(row=2, column=0, padx=10, pady=5)

    # Create a button to save the content to a file
    save_button = ttk.Button(
        root, text="Save File", command=lambda: save_file(display_box))
    save_button.grid(row=2, column=1, padx=10, pady=5)

    return root

def display_text(input_box, display_box):
    input_text = input_box.get()
    display_box.config(state=tk.NORMAL)
    display_box.delete(1.0, tk.END)
    display_box.insert(tk.END, input_text)
    display_box.config(state=tk.DISABLED)


def open_file(display_box):
    file_path = filedialog.askopenfilename()
    if file_path:
        with open(file_path, 'r') as file:
            content = file.read()
            display_box.config(state=tk.NORMAL)
            display_box.delete(1.0, tk.END)
            display_box.insert(tk.END, content)
            display_box.config(state=tk.DISABLED)


def save_file(display_box):
    file_path = filedialog.asksaveasfilename(defaultextension=".txt",
                                             filetypes=[("Text files", "*.txt"), ("All files", "*.*")])
    if file_path:
        with open(file_path, 'w') as file:
            content = display_box.get(1.0, tk.END)
            file.write(content)
