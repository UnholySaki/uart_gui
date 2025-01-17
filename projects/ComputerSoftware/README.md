# UART GUI Project

This project provides a graphical user interface (GUI) for UART communication settings and file management. The GUI is built using Tkinter and includes functionalities for setting UART parameters, opening and saving files, and displaying file content.

## Project Structure

```
uart_gui/
├── projects/
│   ├── computer_src/
│   │   ├── gui.py
│   │   ├── gui_configs.py
│   │   ├── inc/
│   │   │   ├── py_library/
│   │   │   │   ├── sample/
│   │   │   │   │   ├── crc/
│   │   │   │   │   │   ├── inc/
│   │   │   │   │   │   │   ├── crc.py
│   │   │   │   │   │   ├── main.py
│   │   │   │   ├── utils/
│   │   │   │   │   ├── crc.py
│   │   ├── README.md
│   │   ├── main.py
│   │   ├── thread.py
│   │   ├── uart.py
```

## Files Description

- **gui.py**: Contains the main GUI implementation using Tkinter.
- **gui_configs.py**: Configuration file for the GUI.
- **main.py**: Entry point of the application.
- **thread.py**: Manages threading for the GUI and UART communication.
- **uart.py**: Contains the `UartSetting` class for managing UART settings.
- **inc/py_library/utils/crc.py**: Utility functions for CRC calculations.
- **inc/py_library/sample/crc/main.py**: Sample script demonstrating CRC calculations.
- **inc/py_library/sample/crc/inc/crc.py**: CRC calculation functions.
- **inc/py_library/sample/file_path/lib_path.py**: Demonstrates how to add relative paths.

## How to Run

1. **Install Dependencies**:
   Ensure you have Python installed. Install any required dependencies using pip:
   ```sh
   pip install -r requirements.txt
   ```

2. **Run the Application**:
   Navigate to the `computer_src` directory and run the `main.py` script:
   ```sh
   python main.py
   ```

3. **Using the GUI**:
   - Set the UART parameters (port and baudrate) in the UART settings section.
   - Open and display file content using the general input and display section.
   - Save file content or upload data as needed.

## License

This project is licensed under the MIT License.
