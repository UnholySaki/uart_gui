from py_library import *
import tkinter as tk
from tkinter import ttk



def display_text(input_box, display_box):
    input_text = input_box.get()
    display_box.config(state=tk.NORMAL)
    display_box.delete(1.0, tk.END)
    display_box.insert(tk.END, input_text)
    display_box.config(state=tk.DISABLED)

def init_gui():
    # Create the main window
    root = tk.Tk()
    root.title("Tkinter Input and Display")

    # Create an input box
    input_box = ttk.Entry(root, width=50)
    input_box.pack(pady=10)

    # Create a button to display the input text
    display_button = ttk.Button(root, text="Display", command=display_text)
    display_button.pack(pady=5)

    # Create a display box
    display_box = tk.Text(root, width=50, height=10, state=tk.DISABLED)
    display_box.pack(pady=10)
    
    return root
    
    
def main():
    #build a tkinter
    root = init_gui()
    
    root.mainloop()
    
    pass

if __name__ == '__main__':
    main()