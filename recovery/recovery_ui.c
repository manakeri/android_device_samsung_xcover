#include <linux/input.h>
#include "recovery_ui.h"
#include "common.h"
#include "extendedcommands.h"

char* MENU_HEADERS[] = { "Use vol keys to highlight and menu to select.",
                         "",
                         NULL };

char* MENU_ITEMS[] = { "reboot system now",
                       "apply update from sdcard",
                       "wipe data/factory reset",
                       "wipe cache partition",
                       "install zip from sdcard",
                       "backup and restore",
                       "mounts and storage",
                       "advanced",
                       "power off",
                       NULL };

int device_recovery_start() {
    return 0;
}

int device_reboot_now(volatile char* key_pressed, int key_code) {
    return 0;
}

int device_perform_action(int which) {
    return which;
}

int device_wipe_data() {
    return 0;
}

int device_toggle_display(volatile char* key_pressed, int key_code) {
	if (ui_get_showing_back_button()) {
		return 0;
		//return get_allow_toggle_display() && (key_code == KEY_HOME || key_code == KEY_MENU || key_code == KEY_END);
	}
	return get_allow_toggle_display() && (key_code == 115 || key_code == 114);
//	return 1;
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
                if (!get_allow_toggle_display())
                    return GO_BACK;
        }
    }

    return NO_ACTION;
}

