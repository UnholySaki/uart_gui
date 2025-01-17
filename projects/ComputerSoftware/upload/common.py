


class MemoryMap():
    START_ADDR = 0
    END_ADDR = 0
    END_ADDR_OFFSET = 0
    main_addr = 0
    cur_addr = 0
    FLASH_SIZE = 16 * 1024
    SECTOR_SIZE = 1024
    image_str = ["FF"] * FLASH_SIZE
    image_byte_list = []
    is_first_read_attempt = True
