#define error and succesful codes here


class RET_CODE:
    SUCCESS = 0  # Successful operation
    ERR = -2  # General error
    ERR_FNF = -1  # File not found
    ERR_INVALID = -3  # Invalid value
    ERR_INVALID_PORT = -4  # Invalid port
    ERR_INVALID_DATATYPE = -5  # Invalid data type
