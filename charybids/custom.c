#include QMK_KEYBOARD_H

#define _TILING 6
#define _MOUSE 2
#define _NAV 4
#define _GAMING 1

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
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
  if (IS_LAYER_ON(_GAMING)) {
    state = update_tri_layer_state(state, 5, 3, 7);
  } else {
    state = update_tri_layer_state(state, 4, 3, 7);
  }
  return state;
}

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
      keycode_to_press = LSA(KC_W);
      break;
    case PREVWIN:
      keycode_to_press = LSA(KC_S);
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
        if (!alt_tab_registered && IS_LAYER_ON(_TILING)) {
          register_code(KC_LALT);
          alt_tab_registered = true;
        }
      }
      keycode_to_press = keycode == NEXTWIN ? KC_TAB : S(KC_TAB);
      break;
    case MISSION_CONTROL:
      keycode_to_press = KC_LGUI;
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
    if (IS_LAYER_OFF(_TILING)) {
      unregister_code(KC_LALT);
      alt_tab_registered = false;
    }
  }

  // Disable sniping and dragscroll if the mouse layer is off
  if ((charybdis_get_pointer_dragscroll_enabled() ||
       charybdis_get_pointer_sniping_enabled()) &&
      (IS_LAYER_OFF(_MOUSE) && IS_LAYER_OFF(_NAV))) {
    charybdis_set_pointer_dragscroll_enabled(false);
    charybdis_set_pointer_sniping_enabled(false);
  }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  uint8_t layer = get_highest_layer(layer_state);
  if (layer == 1 || layer == 5) {
    RGB_MATRIX_INDICATOR_SET_COLOR(20, 255, 0, 0);
    RGB_MATRIX_INDICATOR_SET_COLOR(8, 255, 0, 0);
    RGB_MATRIX_INDICATOR_SET_COLOR(4, 255, 0, 0);
    RGB_MATRIX_INDICATOR_SET_COLOR(7, 255, 0, 0);
    RGB_MATRIX_INDICATOR_SET_COLOR(10, 255, 0, 0);
    return false;
  }

  if (charybdis_get_pointer_dragscroll_enabled()) {
    RGB_MATRIX_INDICATOR_SET_COLOR(23, 255, 0, 0);
  }
  if (charybdis_get_pointer_sniping_enabled()) {
    RGB_MATRIX_INDICATOR_SET_COLOR(18, 255, 0, 0);
  }

  if (layer == 2) {

    RGB_MATRIX_INDICATOR_SET_COLOR(24, 0, 255, 0);
    RGB_MATRIX_INDICATOR_SET_COLOR(29, 0, 255, 0);
    RGB_MATRIX_INDICATOR_SET_COLOR(15, 255, 0, 0);
    return false;
  }

  return false;
}

#ifdef TAP_LAYER_MOUSE
// Determine the current tap dance state
//  void dance_layers(qk_tap_dance_state_t *state, void *user_data)
//  {
//    if (state->pressed)
//    {
//      layer_on(_MOUSE);
//    }
//    else
//    {
//      layer_off(_MOUSE);
//    }
//    switch (state->count)
//    {
//    case 1: //off all layers, just base layer on
//      if (!state->pressed)
//      {
//        if(IS_LAYER_ON(_MOUSE)){
//          layer_off(_MOUSE);
//        } else {
//          layer_on(_MOUSE);
//        }
//      }
//      break;
//    }
//  }
//  Determine the current tap dance state
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

// Initialize tap structure associated with example tap dance key
static td_tap_t ql_tap_state = {.is_press_action = true, .state = TD_NONE};

// Functions that control what our tap dance key does
void ql_finished(tap_dance_state_t *state, void *user_data) {
  ql_tap_state.state = cur_dance(state);
  switch (ql_tap_state.state) {
  case TD_SINGLE_TAP:
    // Check to see if the layer is already set
    if (layer_state_is(_MOUSE)) {
      // If already set, then switch it off
      layer_off(_MOUSE);
    } else {
      // If not already set, then switch the layer on
      layer_on(_MOUSE);
    }
    break;
  case TD_SINGLE_HOLD:
    layer_on(_MOUSE);
    break;
  case TD_DOUBLE_TAP:
    // Check to see if the layer is already set
    if (layer_state_is(_GAMING)) {
      // If already set, then switch it off
      layer_off(_GAMING);
    } else {
      // If not already set, then switch the layer on
      layer_on(_GAMING);
    }
    break;
  default:
    break;
  }
}

void ql_reset(tap_dance_state_t *state, void *user_data) {
  // If the key was held down and now is released then switch off the layer
  if (ql_tap_state.state == TD_SINGLE_HOLD) {
    layer_off(_MOUSE);
  }
  ql_tap_state.state = TD_NONE;
}

static td_tap_t ql_tap_state_drag = {.is_press_action = true, .state = TD_NONE};

static was_drag = false;

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
  ql_tap_state_drag.state = TD_NONE;
}

static td_tap_t ql_tap_state_sniping = {.is_press_action = true,
                                        .state = TD_NONE};

static was_sniping = false;

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
    layer_off(_GAMING);
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
    [QUOT_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset),
    [DRAG] = ACTION_TAP_DANCE_FN_ADVANCED(ql_drag_start, ql_drag_finished,
                                          ql_drag_reset),
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
#endif
