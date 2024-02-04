#pragma once

#define DIODE_DIRECTION ROW2COL
#define SPLIT_LAYER_STATE_ENABLE
#define EE_HANDS
#define CHARYBDIS_CONFIG_SYNC
/* Trackball angle adjustment. */
#define ROTATIONAL_TRANSFORM_ANGLE -25

/* RGB settings. */
#ifdef RGB_MATRIX_ENABLE
#    define RGB_MATRIX_LED_COUNT 36
#    define RGB_MATRIX_SPLIT \
        { 18, 18 }
#endif

#define QUICK_TAP_TERM 0
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY


//In my bios it doesnt turn on sometimes
#define SPLIT_USB_TIMEOUT 4000
#define MOUSE_EXTENDED_REPORT
#define CHARYBDIS_DRAGSCROLL_REVERSE_Y
