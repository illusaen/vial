CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
SPLIT_KEYBOARD = yes        # Use shared split_common code
LTO_ENABLE = yes            # Use link time optimization for smaller firmware
QMK_SETTINGS = no

SRC += oneshot.c
SRC += swapper.c
