#pragma once

#define DIODE_DIRECTION ROW2COL

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
