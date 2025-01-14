from projects.computer_src.file_hanlder.send_uart import *
from projects.computer_src.file_hanlder.read_hex_file import *
from projects.computer_src.common.memory_map import *


blanking_address = "00002000"
blanking_size = "00002000"


def select_file() -> None:
    """
    Select and download a HEX file to the device.

    Procedure:
    1.
    2. Erase the device's memory to prepare it for downloading.
    3. Initiate the file download process based on the selected file.
    """

    file = input(
        "\nInput number: \
        \n1. File 1: MSPM0 MPPT app.hex\
        \n2. File 2: (TEST) HexFile.hex\
        \n3. File 3: (TEST) HexFile-mod.hex\
        \n4. File 4: MSPM0 Image header \
        \n5. File 5: STM32 application hex file \
        \n6. File 6: STM32 app blink led \
        \n7. File 7: MSPM0 app blink led \
        \n8. File 8: MSPM0 app blink led - 16 bytes \
        \nChoosing mode: ")
    match file:
        case "1":
            file_path = "Releases/app.hex"
            send_hex_file(file_path)

        case "2":
            send_hex_file("Releases/HexFile.hex")

        case "3":
            send_hex_file("Releases/HexFile-mod.hex")

        case "4":
            send_hex_file("Releases/page_info.hex", check_crc=True)
            
        case "5":
            send_hex_file("Releases/stm32_test.hex", check_crc=True)
            
        case "6":
            send_hex_file("Releases/app_led_blink.hex", check_crc=True)
            
        case "7":
            send_hex_file("Releases/MSP_M0_blink_led.hex", check_crc=True)
            
        case "8":
            send_hex_file("Releases/MSP_M0_blink_led_16.hex", check_crc=True)

        case _:
            print("Unknown command, try again")


def select_command() -> None:
    """
    Select a control command to execute.
    """
    while (True):
        command_mode = input(
            "\n1. Erase \
            \n2. Check Blanking \
            \n3. System reset \
            \n4. Get Bootloader version \
            \n5. Enter Bootloader \
            \n6. Exit Bootloader \
            \nReturning: write \"r\" to return\
            \nChoosing mode: ")
        match command_mode:
            case "1":
                is_erased = send_erase_cmd(UART_PORT, FLASH_APP_START, FLASH_APP_SIZE)
                if is_erased:
                    print("Flash erase success")
                else:
                    print("Fail to erase")
                    exit()

            case "2":
                blanking_adr = input(
                    "Input starting address Ex. \"00002000\": ")
                mem_len = input("Input memory size Ex. \"00001000\": ")
                is_image_clean = send_check_blanking_cmd(
                    UART_PORT, blanking_adr, mem_len)
                if (is_image_clean):  # clean
                    print("Erase target is clean")
                    pass
                else:
                    print("Erase target is dirty")

            case "3":
                send_data(UART_PORT, Rq_cmd.RQ_SYSTEM_RESET)

            case "4":
                bld_version = (get_bld_ver_cmd(UART_PORT, 4))
                print("Bootloader version is: ", bld_version)
            case "5":
                send_enter_bld_cmd(UART_PORT)
                is_enter_success = read_enter_bld_response(UART_PORT)

            case "6":
                send_exit_bld_cmd(UART_PORT)
                read_exit_bld_response(UART_PORT)

            case "r":
                break

            case _:
                print("Unknown command, returning")
                break


def main():
    """
        Main loop
    """
    while True:
        mode = input("\nSelect mode: \
                    \nMode 1: downloading \
                    \nMode 2: other specific tasks \
                    \nExiting: write \"e\" to exit\
                    \nChoosing mode: ")
        match mode:
            case "1":
                select_file()

            case "2":
                select_command()

            case "e":
                exit()

            case _:
                print("Unknown command, exiting")
                exit()


if __name__ == "__main__":
    main()
