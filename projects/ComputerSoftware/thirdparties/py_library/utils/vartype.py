def str_list_to_byte_list(str_list: list[str]):
    """
    Convert a list of string of hexadecimal values into a list of 16-bit integers in big-endian format.

    Args:
        string (str): A list of string containing hexadecimal values where each two characters 
                      represent a byte (e.g., ["0A0B", "0C0D"] for two bytes).

    Returns:
        list[int]: A list of 16-bit integers where each pair of hexadecimal characters 
                   is combined into a 16-bit value in little-endian format.
                   For example, "0A0B" becomes 0x0A0B.
    """
    counter = 0
    byte_list = []
    for i in range(int(len(str_list) / 2)):
        byte_list.append(i)
        byte_list[i] = int(str_list[counter + 1], base=16)
        byte_list[i] = byte_list[i] * 0x100 + int(str_list[counter], base=16)
        counter += 2

    return byte_list
