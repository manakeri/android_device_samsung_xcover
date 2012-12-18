#include <linux/input.h>

#include "recovery_ui.h"
#include "common.h"
#include "extendedcommands.h"

int device_toggle_display(volatile char* key_pressed, int key_code) {
	if (ui_get_showing_back_button()) {
		return 0;
	}
	return get_allow_toggle_display() && (key_code == 115 || key_code == 114);
}

int device_handle_key(int key_code, int visible) {
    if (visible) {
        switch (key_code) {
            case 115:
                return HIGHLIGHT_UP;

            case 114:
                return HIGHLIGHT_DOWN;

            case 139:
                return SELECT_ITEM;

            case 158:
                if (!ui_root_menu) {
                    return GO_BACK;
                }            
                if (!get_allow_toggle_display())
                    return GO_BACK;
        }
    }

    return NO_ACTION;
}

