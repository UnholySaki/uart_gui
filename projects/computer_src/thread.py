import time
import threading

import sys
import os

sys.path.append(os.path.abspath("../inc"))

from gui import *



def gui_thread_func():
    root = init_gui()
    root.mainloop()


def uart_thread_func():
    global stop_uart_thread
    
    stop_uart_thread = False
    while not stop_uart_thread:
        print("hello")
        time.sleep(1)


def initialize_threads():
    # Run the Tkinter main loop in a separate thread
    gui_thread = threading.Thread(target=gui_thread_func)
    gui_thread.start()

    uart_thread = threading.Thread(target=uart_thread_func)
    # uart_thread.start()
