## General
This is bootloader for stm32f103 that use UART communication to upload,
The computer program is placed in projects/computer_src directory

## Requirements
- STM32F103 microcontroller
- UART communication interface
- Computer with a terminal program (e.g., PuTTY, Tera Term)
- STM32 development environment (e.g., STM32CubeIDE)

## Installation
1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/stm32_uart_bootloader.git
    ```
2. Navigate to the project directory:
    ```sh
    cd stm32_uart_bootloader
    ```
3. Open the project in STM32CubeIDE and build it.

## Usage
1. Connect the STM32F103 microcontroller to your computer via UART.
2. Open a terminal program and connect to the appropriate COM port with the correct baud rate.
3. Reset the microcontroller to enter bootloader mode.
4. Use the terminal program to send the firmware file to the microcontroller.

## Contributing
1. Fork the repository.
2. Create a new branch:
    ```sh
    git checkout -b feature-branch
    ```
3. Make your changes and commit them:
    ```sh
    git commit -m "Description of changes"
    ```
4. Push to the branch:
    ```sh
    git push origin feature-branch
    ```
5. Open a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.