class RespondCmd():
    """
    Contains the respond signal
    """
    RES_EXIT_BLD_SUCCESS = ord('c')
    RES_EXIT_BLD_FAIL = ord('d')
    RES_ENTER_BLD_SUCCESS = ord('a')
    RES_ENTER_BLD_FAIL = ord('b')
    RES_ERASE_COMPLETE = ord('8')  # Erase memory success
    RES_CRC_FAIL = ord('7')  # Check crc fail
    RES_CRC_SUCCESS = ord('6')  # download image success
    RES_WRITE_DATA_SUCCESS = ord('5')  # download line success
    RES_WRITE_DATA_FAIL = ord('4')  # download line fail - Ask for line again
    RES_DOWNLOAD_INPROG = ord('3')  # downloading in progress - Ask to wait
    RES_APP_FLASH_CLEAN = ord('2')  # main application flash is clean
    RES_APP_FLASH_DIRTY = ord('1')  # main application flash is dirty


class RequestCmd():
    """
    ontains the request commands
    """
    RQ_DOWNLOAD = 0xF2
    RQ_ERASE = 0xF3
    RQ_CHECK_BLANKING = 0xF4
    RQ_CHECK_CRC_MODE = 0xF5
    RQ_SYSTEM_RESET = 0xF6
    RQ_GET_BLD_VER = 0xF7
    RQ_ENTER_BLD = 0xF8
    RQ_EXIT_BLD = 0xF9