#include "other/tempo.h"

#define tempo_message_counter_max 96

u32 tempo_counter = 0;
u8 tempo_listen = 0;
u32 tempo_timer = 0;
u8 tempo_message_counter = 0;
u32 tempo_bar = 2000;

void tempo_start() {
	tempo_counter = 0;
	tempo_listen = 1;
	tempo_timer = 0;
	tempo_message_counter = 0;
}

void tempo_midi() {
	if (tempo_listen) {
		u8 tempo_message_beat = tempo_message_counter_max >> 2;
		if ((++tempo_message_counter % tempo_message_beat) == tempo_message_beat) {
			tempo_bar = tempo_timer << 2;
			tempo_timer = 0;
		}
		if (tempo_message_counter == tempo_message_counter_max) {
			tempo_counter = 0;
			tempo_message_counter = 0;
		}
	}
}

void tempo_stop() {
	tempo_listen = 0;
}

void tempo_tick() {
	if (++tempo_counter >= tempo_bar) {
		tempo_counter = 0;
	}

	u8 palette_using = (mode == mode_performance)? palette_selected : palette_novation;
	u8 flash_state, pulse_state;
	u32 t;

	switch (global_timer % 4) {
		case 0:
			flash_state = (tempo_counter % (tempo_bar >> 2)) < (tempo_bar >> 3);
			for (u8 i = 1; i < 50; i++) {
				if (flash_screen[i]) {
					hal_plot_led(TYPEPAD, i, palette_value(palette_using, flash_screen[i], 0) * flash_state, palette_value(palette_using, flash_screen[i], 1) * flash_state, palette_value(palette_using, flash_screen[i], 2) * flash_state);
				}
			}
			break;

		case 1:
			flash_state = (tempo_counter % (tempo_bar >> 2)) < (tempo_bar >> 3);
			for (u8 i = 50; i < 99; i++) {
				if (flash_screen[i]) {
					hal_plot_led(TYPEPAD, i, palette_value(palette_using, flash_screen[i], 0) * flash_state, palette_value(palette_using, flash_screen[i], 1) * flash_state, palette_value(palette_using, flash_screen[i], 2) * flash_state);
				}
			}
			if (flash_screen[99]) {
				hal_plot_led(TYPESETUP, 0, palette_value(palette_using, flash_screen[99], 0) * flash_state, palette_value(palette_using, flash_screen[99], 1) * flash_state, palette_value(palette_using, flash_screen[99], 2) * flash_state);	
			}
			break;

		case 2:
			t = tempo_counter % (tempo_bar >> 1);
			pulse_state = (t < (tempo_bar >> 3))? (15 * tempo_bar + 384 * t) / tempo_bar : (237 * tempo_bar - 384 * t) / (3 * tempo_bar);
			for (u8 i = 1; i < 50; i++) {
				if (pulse_screen[i]) {
					hal_plot_led(TYPEPAD, i, (palette_value(palette_using, pulse_screen[i], 0) * pulse_state) / 63, (palette_value(palette_using, pulse_screen[i], 1) * pulse_state) / 63, (palette_value(palette_using, pulse_screen[i], 2) * pulse_state) / 63);
				}
			}
			break;

		case 3:
			t = tempo_counter % (tempo_bar >> 1);
			pulse_state = (t < (tempo_bar >> 3))? (15 * tempo_bar + 384 * t) / tempo_bar : (237 * tempo_bar - 384 * t) / (3 * tempo_bar);
			for (u8 i = 50; i < 99; i++) {
				if (pulse_screen[i]) {
					hal_plot_led(TYPEPAD, i, (palette_value(palette_using, pulse_screen[i], 0) * pulse_state) / 63, (palette_value(palette_using, pulse_screen[i], 1) * pulse_state) / 63, (palette_value(palette_using, pulse_screen[i], 2) * pulse_state) / 63);
				}
			}
			if (pulse_screen[99]) {
				hal_plot_led(TYPESETUP, 0, (palette_value(palette_using, pulse_screen[99], 0) * pulse_state) / 63, (palette_value(palette_using, pulse_screen[99], 1) * pulse_state) / 63, (palette_value(palette_using, pulse_screen[99], 2) * pulse_state) / 63);
			}
			break;
	}
}