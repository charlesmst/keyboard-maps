
#include QMK_KEYBOARD_H

#ifdef MACCEL_ENABLE
    #include "maccel/maccel.h"
#endif

#ifndef PLOOPY_DRAGSCROLL_DIVISOR_H
#    define PLOOPY_DRAGSCROLL_DIVISOR_H 24.0
#endif
#ifndef PLOOPY_DRAGSCROLL_DIVISOR_V
#    define PLOOPY_DRAGSCROLL_DIVISOR_V 24.0
#endif


#define _TILING 1
#define _MOUSE 1
#define _NAV 1
#define _TILING_LEFT 3
#define TAP_LAYER_MOUSE
#ifdef TAP_LAYER_MOUSE
// Define a type for as many tap dance states as you need
typedef enum {
  TD_NONE,
  TD_UNKNOWN,
  TD_SINGLE_TAP,
  TD_SINGLE_HOLD,
  TD_DOUBLE_TAP,
  TD_TRIPLE_TAP
} td_state_t;

typedef struct {
  bool is_press_action;
  td_state_t state;
} td_tap_t;

enum {
  QUOT_LAYR, // Our custom tap dance key; add any other tap dance keys to this
             // enum
  DRAG,
  LAYER_CLICK,
  SNIPING_TD,
  ALT_LAYER_TD,
};

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
td_state_t cur_dance(tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(tap_dance_state_t *state, void *user_data);
void ql_reset(tap_dance_state_t *state, void *user_data);

void ql_drag_finished(tap_dance_state_t *state, void *user_data);
void ql_drag_reset(tap_dance_state_t *state, void *user_data);

void ql_sniping_finished(tap_dance_state_t *state, void *user_data);
void ql_sniping_reset(tap_dance_state_t *state, void *user_data);

void ql_layer_start(tap_dance_state_t *state, void *user_data);
void ql_layer_finished(tap_dance_state_t *state, void *user_data);
void ql_layer_reset(tap_dance_state_t *state, void *user_data);
#endif

bool  local_is_scroll_clicked    = false;
bool  local_is_drag_scroll       = false;
float local_scroll_accumulated_h = 0;
float local_scroll_accumulated_v = 0;

bool charybdis_get_pointer_dragscroll_enabled(void){
    return local_is_drag_scroll;
}

bool charybdis_get_pointer_sniping_enabled(void){
    return false;
}

void charybdis_set_pointer_dragscroll_enabled(bool enabled){
    local_is_drag_scroll = enabled;
}
void charybdis_set_pointer_sniping_enabled(bool enabled){
    //nothing to do
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (local_is_drag_scroll) {
        local_scroll_accumulated_h += (float)mouse_report.x / PLOOPY_DRAGSCROLL_DIVISOR_H;
        local_scroll_accumulated_v += (float)mouse_report.y / PLOOPY_DRAGSCROLL_DIVISOR_V;

        // Assign integer parts of accumulated scroll values to the mouse report
        mouse_report.h = (int8_t)local_scroll_accumulated_h;
#ifdef PLOOPY_DRAGSCROLL_INVERT
        mouse_report.v = -(int8_t)local_scroll_accumulated_v;
#else
        mouse_report.v = (int8_t)local_scroll_accumulated_v;
#endif

        // Update accumulated scroll values by subtracting the integer parts
        local_scroll_accumulated_h -= (int8_t)local_scroll_accumulated_h;
        local_scroll_accumulated_v -= (int8_t)local_scroll_accumulated_v;

        // Clear the X and Y values of the mouse report
        mouse_report.x = 0;
        mouse_report.y = 0;

        mouse_report.x = 0;
        mouse_report.y = 0;
    }


#ifdef MACCEL_ENABLE
    return pointing_device_task_maccel(mouse_report);
#else
    return mouse_report;
#endif

}
#include "os_detection.h"
enum custom_keycodes {
  USR_UNDO = SAFE_RANGE,
  USR_CUT,
  USR_COPY,
  USR_PASTE,
  USR_PRINT,
  PREVWIN,
  NEXTWIN,
  MISSION_CONTROL,
};

static bool alt_tab_registered = false;

bool is_apple(void) {
  os_variant_t host_os = detected_host_os();
  return host_os == OS_MACOS || host_os == OS_IOS;
}

void mission_control(void){
  uint16_t keycode_to_press = KC_NO;
  if(is_apple()){
      keycode_to_press = C(KC_UP);
  }else{
      keycode_to_press = G(KC_TAB);
  }
  register_code16(keycode_to_press);
  unregister_code16(keycode_to_press);
}
void process_platform_combo(uint16_t keycode, keyrecord_t *record) {
  uint16_t keycode_to_press = KC_NO;
  if (is_apple()) {
    switch (keycode) {
    case USR_UNDO:
      keycode_to_press = G(KC_Z);
      break;
    case USR_CUT:
      keycode_to_press = G(KC_X);
      break;
    case USR_COPY:
      keycode_to_press = G(KC_C);
      break;
    case USR_PASTE:
      keycode_to_press = G(KC_V);
      break;
    case USR_PRINT:
      keycode_to_press = SGUI(KC_4);
      break;
      // I use amethyst for this on mac, this is the shortcut
    case NEXTWIN:
      keycode_to_press = LSA(KC_D);
      break;
    case PREVWIN:
      keycode_to_press = LSA(KC_A);
      break;
    case MISSION_CONTROL:
      keycode_to_press = C(KC_UP);
      break;

    }

  } else {
    switch (keycode) {
    case USR_UNDO:
      keycode_to_press = C(KC_Z);
      break;
    case USR_CUT:
      keycode_to_press = C(KC_X);
      break;
    case USR_COPY:
      keycode_to_press = C(KC_C);
      break;
    case USR_PASTE:
      keycode_to_press = C(KC_V);
      break;
    case USR_PRINT:
      keycode_to_press = KC_PSCR;
      break;
    case NEXTWIN:
    case PREVWIN:
      if (record->event.pressed) {
        if (!alt_tab_registered && (IS_LAYER_ON(_TILING) || IS_LAYER_ON(_TILING_LEFT)) ) {
          register_code(KC_LALT);
          alt_tab_registered = true;
        }
      }
      keycode_to_press = keycode == NEXTWIN ? KC_TAB : S(KC_TAB);
      break;
    case MISSION_CONTROL:
      keycode_to_press = G(KC_TAB);
      break;

    }
  }
  if (record->event.pressed) {
    register_code16(keycode_to_press);
  } else {
    unregister_code16(keycode_to_press);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case USR_UNDO:
  case USR_CUT:
  case USR_COPY:
  case USR_PASTE:
  case USR_PRINT:
  case NEXTWIN:
  case PREVWIN:
  case MISSION_CONTROL:
    process_platform_combo(keycode, record);
    return false;
  }

  return true;
}

void matrix_scan_user(void) {
  if (alt_tab_registered) {
    if (IS_LAYER_OFF(_TILING) && IS_LAYER_OFF(_TILING_LEFT)) {
      unregister_code(KC_LALT);
      alt_tab_registered = false;
    }
  }

}

#ifdef TAP_LAYER_MOUSE
td_state_t cur_dance(tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed)
      return TD_SINGLE_TAP;
    else
      return TD_SINGLE_HOLD;
  } else if (state->count == 2)
    return TD_DOUBLE_TAP;
  else if (state->count == 3)
    return TD_TRIPLE_TAP;
  else
    return TD_UNKNOWN;
}


static td_tap_t ql_tap_state_drag = {.is_press_action = true, .state = TD_NONE};

static bool was_drag = false;
static bool is_btn3 = false;

void ql_drag_start(tap_dance_state_t *state, void *user_data) {
  was_drag = charybdis_get_pointer_dragscroll_enabled();
  charybdis_set_pointer_dragscroll_enabled(true);
}
// Functions that control what our tap dance key does
void ql_drag_finished(tap_dance_state_t *state, void *user_data) {
  ql_tap_state_drag.state = cur_dance(state);
  switch (ql_tap_state_drag.state) {
  case TD_SINGLE_TAP:
    charybdis_set_pointer_dragscroll_enabled(!was_drag);
    break;
  case TD_SINGLE_HOLD:
    charybdis_set_pointer_dragscroll_enabled(true);
    break;
  case TD_DOUBLE_TAP:
    register_code(KC_BTN3);
    break;
  case TD_TRIPLE_TAP:
    is_btn3 = !is_btn3;
    if(is_btn3){
        register_code(KC_BTN3);
    }else{
        unregister_code(KC_BTN3);
    }
    break;
  default:
    break;
  }
}

void ql_drag_reset(tap_dance_state_t *state, void *user_data) {
  // If the key was held down and now is released then switch off the layer
  if (ql_tap_state_drag.state == TD_SINGLE_HOLD) {
    charybdis_set_pointer_dragscroll_enabled(false);
  }
  if (ql_tap_state_drag.state == TD_DOUBLE_TAP) {
    unregister_code(KC_BTN3);
    charybdis_set_pointer_dragscroll_enabled(false);
  }
  if (ql_tap_state_drag.state == TD_TRIPLE_TAP) {
    charybdis_set_pointer_dragscroll_enabled(false);
  }
  ql_tap_state_drag.state = TD_NONE;
}

static td_tap_t ql_tap_state_sniping = {.is_press_action = true,
                                        .state = TD_NONE};

static bool was_sniping = false;

void ql_sniping_start(tap_dance_state_t *state, void *user_data) {
  was_sniping = charybdis_get_pointer_sniping_enabled();
  charybdis_set_pointer_sniping_enabled(true);
}
// Functions that control what our tap dance key does
void ql_sniping_finished(tap_dance_state_t *state, void *user_data) {
  ql_tap_state_sniping.state = cur_dance(state);
  switch (ql_tap_state_sniping.state) {
  case TD_SINGLE_TAP:
    charybdis_set_pointer_sniping_enabled(!was_sniping);
    break;
  case TD_SINGLE_HOLD:
    charybdis_set_pointer_sniping_enabled(true);
    break;
  case TD_DOUBLE_TAP:
    break;
  default:
    break;
  }
}

void ql_sniping_reset(tap_dance_state_t *state, void *user_data) {
  // If the key was held down and now is released then switch off the layer
  if (ql_tap_state_sniping.state == TD_SINGLE_HOLD) {
    charybdis_set_pointer_sniping_enabled(false);
  }
  ql_tap_state_sniping.state = TD_NONE;
}
/* ------------------------------------------------ */
static td_tap_t ql_tap_state_alt = {.is_press_action = true, .state = TD_NONE};

void ql_alt_start(tap_dance_state_t *state, void *user_data) {
  register_code(KC_LALT);
}
// Functions that control what our tap dance key does
void ql_alt_finished(tap_dance_state_t *state, void *user_data) {
  ql_tap_state_alt.state = cur_dance(state);
  switch (ql_tap_state_alt.state) {
  case TD_TRIPLE_TAP:
  case TD_DOUBLE_TAP:
    // layer_off(_GAMING);
    break;
  default:
    break;
  }
}

void ql_alt_reset(tap_dance_state_t *state, void *user_data) {
  unregister_code(KC_LALT);
  ql_tap_state_alt.state = TD_NONE;
}

// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
    [DRAG] = ACTION_TAP_DANCE_FN_ADVANCED(ql_drag_start, ql_drag_finished,
                                          ql_drag_reset),
    [LAYER_CLICK] = ACTION_TAP_DANCE_FN_ADVANCED(ql_layer_start, ql_layer_finished,
                                          ql_layer_reset),
    [SNIPING_TD] = ACTION_TAP_DANCE_FN_ADVANCED(
        ql_sniping_start, ql_sniping_finished, ql_sniping_reset),
    [ALT_LAYER_TD] = ACTION_TAP_DANCE_FN_ADVANCED(ql_alt_start, ql_alt_finished,
                                                  ql_alt_reset)};

// Set a long-ish tapping term for tap-dance keys
// uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
//             return 275;
//         default:
//             return TAPPING_TERM;
//     }
// ---------------------------------------------------------


static td_tap_t ql_tap_state_layer = {.is_press_action = true,
                                        .state = TD_NONE};


void ql_layer_start(tap_dance_state_t *state, void *user_data) {
  bool is_left = IS_LAYER_ON(2);
  if(is_left){
    layer_on(3);
  } else {
    layer_on(1);
  }
}
// Functions that control what our tap dance key does
void ql_layer_finished(tap_dance_state_t *state, void *user_data) {
  ql_tap_state_layer.state = cur_dance(state);
  switch (ql_tap_state_layer.state) {
  case TD_SINGLE_TAP:
    mission_control();
    break;
  case TD_SINGLE_HOLD:
  case TD_DOUBLE_TAP:
    break;
  default:
    break;
  }
}

void ql_layer_reset(tap_dance_state_t *state, void *user_data) {
  bool is_left = IS_LAYER_ON(2);
  if(is_left){
    layer_off(3);
  } else {
    layer_off(1);
  }
  ql_tap_state_layer.state = TD_NONE;
}



#endif

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  // Capture all mod-tap keycodes.
  case QK_MOD_TAP ... QK_MOD_TAP_MAX:
    if (keycode == RSFT_T(KC_ENT)) {
      // aka enable IGNORE_MOD_TAP_INTERRUPT for LCTL_T(KC_A).
      //
      // Enable HOLD_ON_OTHER_KEY_PRESS for every other mod-tap keycode.
      return true;
    } else {
      // Disable HOLD_ON_OTHER_KEY_PRESS for LCTL_T(KC_A)
      return false;
    }
  default:
    return false;
  }
}

const uint16_t PROGMEM test_combo1[] = {KC_BTN4, KC_BTN5, COMBO_END};
combo_t key_combos[] = {
    COMBO(test_combo1, TG(2)),
};
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KC_BTN4, KC_BTN5, TD(DRAG), KC_BTN2, KC_BTN1, TD(LAYER_CLICK)),
    [1] = LAYOUT( PREVWIN, NEXTWIN, MISSION_CONTROL, USR_PASTE, USR_COPY, TD(LAYER_CLICK)),
    [2] = LAYOUT( KC_BTN2,TD(DRAG),KC_BTN5, KC_BTN4, TD(LAYER_CLICK),KC_BTN1),
    [3] = LAYOUT( USR_PASTE,MISSION_CONTROL,NEXTWIN,PREVWIN ,TD(LAYER_CLICK),USR_COPY)
};
