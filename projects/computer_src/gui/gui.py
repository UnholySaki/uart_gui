import tkinter as tk
from tkinter import ttk
from gui.gui_func import *
from gui.gui_configs import *

### Global variables
global uart_setting
uart_setting = UartSetting()


### Main function
def init_gui():
    ######### Overall Setup ##########
    # Create the main window
    root = tk.Tk()
    root.title("Tkinter Input and Display")
    root.geometry(f"{GuiConfig.ROOT_WIDTH}x{GuiConfig.ROOT_HEIGHT}")
    root.resizable(False, False)

    # Create a canvas for UART settings
    uart_canvas = tk.Canvas(root,
                            width=GuiConfig.UART_CV_WIDTH,
                            height=GuiConfig.UART_CV_HEIGHT)
    uart_canvas.grid(row=0,
                     column=0,
                     padx=GuiConfig.PADX,
                     pady=GuiConfig.PADY,
                     sticky="nsew")

    uart_frame = tk.Frame(uart_canvas)
    uart_canvas.create_window((0, 0), window=uart_frame, anchor="nw")

    # Create a canvas for general input and display
    general_canvas = tk.Canvas(root,
                               width=GuiConfig.GEN_CV_WIDTH,
                               height=GuiConfig.GEN_CV_HEIGHT)
    general_canvas.grid(row=0,
                        column=1,
                        padx=GuiConfig.PADX,
                        pady=GuiConfig.PADY,
                        sticky="nsew")

    app_frame = tk.Frame(general_canvas)
    general_canvas.create_window((0, 0), window=app_frame, anchor="nw")

    init_uart_frame(uart_frame)
    init_app_frame(app_frame)

    return root


def init_uart_frame(frame):
    # UART settings widgets
    serial_label = tk.Label(frame,
                            text="Serial Port",
                            width=GuiConfig.LABEL_WIDTH)
    serial_label.grid(row=0, column=0, pady=GuiConfig.PADY)

    port_entry = tk.Entry(frame, width=GuiConfig.INPUT_BOX_WIDTH)
    port_entry.grid(row=0, column=1, padx=GuiConfig.PADX, pady=GuiConfig.PADY)

    baudrate_label = tk.Label(frame,
                              text="Baudrate",
                              width=GuiConfig.LABEL_WIDTH)
    baudrate_label.grid(row=1, column=0, pady=GuiConfig.PADY)

    baudrate_entry = tk.Entry(frame, width=GuiConfig.INPUT_BOX_WIDTH)
    baudrate_entry.grid(row=1,
                        column=1,
                        padx=GuiConfig.PADX,
                        pady=GuiConfig.PADY)

    file_adr_label = tk.Label(frame,
                              text="Bin address",
                              width=GuiConfig.LABEL_WIDTH)
    file_adr_label.grid(row=2, column=0, pady=GuiConfig.PADY)

    file_adr_entry = tk.Entry(frame, width=GuiConfig.INPUT_BOX_WIDTH)
    file_adr_entry.grid(row=2,
                        column=1,
                        padx=GuiConfig.PADX,
                        pady=GuiConfig.PADY)

    info_box = tk.Text(frame,
                       width=GuiConfig.LABEL_WIDTH + GuiConfig.INPUT_BOX_WIDTH,
                       height=6,
                       state=tk.DISABLED)
    info_box.grid(row=3,
                  column=0,
                  columnspan=2,
                  padx=GuiConfig.PADX,
                  pady=GuiConfig.PADY)

    save_setting_btn = tk.Button(
        frame,
        width=GuiConfig.BTN_WIDTH,
        text="Save Settings",
        command=lambda: save_setting(uart_setting, port_entry, baudrate_entry,
                                     file_adr_entry, info_box),
    )
    save_setting_btn.grid(row=4,
                          column=0,
                          padx=GuiConfig.PADX,
                          pady=GuiConfig.PADY,
                          sticky=tk.S)

    refresh_btn = tk.Button(
        frame,
        width=GuiConfig.BTN_WIDTH,
        text="Refresh",
        command=lambda: update_field(uart_setting.get_cfg_file_addr(), info_box
                                     ),
    )
    refresh_btn.grid(row=4,
                     column=1,
                     padx=GuiConfig.PADX,
                     pady=GuiConfig.PADY,
                     sticky=tk.S)

    port_entry.insert(tk.END, uart_setting.get_cfg_port())
    baudrate_entry.insert(tk.END, str(uart_setting.get_cfg_baudrate()))
    file_adr_entry.insert(tk.END, uart_setting.get_cfg_file_addr())

    info_box.config(state=tk.NORMAL)
    info_box.insert(tk.END, "Setting\n")
    info_box.config(state=tk.DISABLED)


def init_app_frame(frame):
    # General input and display widgets
    file_address_box = tk.Entry(frame, width=40)
    file_address_box.grid(row=0,
                          column=0,
                          padx=GuiConfig.PADX,
                          pady=GuiConfig.PADY)

    open_file_btn = tk.Button(
        frame,
        width=GuiConfig.BTN_WIDTH,
        text="Open File",
        command=lambda: open_file(uart_setting, general_box),
    )
    open_file_btn.grid(row=0,
                       column=1,
                       padx=GuiConfig.PADX,
                       pady=GuiConfig.PADY)

    general_box = tk.Text(frame, height=12, state=tk.DISABLED)
    general_box.grid(row=1,
                     column=0,
                     columnspan=3,
                     padx=GuiConfig.PADX,
                     pady=GuiConfig.PADY)

    display_button = tk.Button(
        frame,
        width=GuiConfig.BTN_WIDTH,
        text="Display",
        command=lambda: update_field(file_address_box.get(), general_box),
    )
    display_button.grid(row=0,
                        column=2,
                        padx=GuiConfig.PADX,
                        pady=GuiConfig.PADY)

    send_to_uart_btn = tk.Button(frame,
                                 width=GuiConfig.BTN_WIDTH,
                                 text="Upload",
                                 command=lambda: upload_data(uart_setting))
    send_to_uart_btn.grid(row=2,
                          column=0,
                          padx=GuiConfig.PADX,
                          pady=GuiConfig.PADY)

    save_btn = tk.Button(
        frame,
        width=GuiConfig.BTN_WIDTH,
        text="Save File",
        command=lambda: save_file(general_box),
    )
    save_btn.grid(row=2, column=1, padx=GuiConfig.PADX, pady=GuiConfig.PADY)

    setting_btn = tk.Button(
        frame,
        width=GuiConfig.BTN_WIDTH,
        text="Settings",
        command=lambda: open_setting_window(),
    )
    setting_btn.grid(row=2, column=2, padx=GuiConfig.PADX, pady=GuiConfig.PADY)
