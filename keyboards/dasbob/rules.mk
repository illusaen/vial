OLED_ENABLE = yes
OLED_DRIVER = ssd1306
SERIAL_DRIVER = vendor
AUDIO_DRIVER = pwm_hardware
TAP_DANCE_ENABLE = no
KEY_OVERRIDE_ENABLE = no

SRC += oneshot.c
SRC += swapper.c