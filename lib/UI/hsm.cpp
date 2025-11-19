/*
 * hsm.c
 *
 *  Created on: Jun 25, 2021
 *      Author: Biren
 */

#include "hsm.h"
//#include "esp_log.h"
#include "ui.h"

uint8_t screen_changed = 0;
hsm_t* current_screen_hsm = &ui.hsm[HSM_HOME];

static void hsm_enter(hsm_t* hsm, uint32_t event) {
	if (hsm == 0)
		return;
	if (hsm->enter)
		hsm->enter(event);
	hsm_enter(hsm->sub, event);
}
static void hsm_exit(hsm_t* hsm, uint32_t event) {
	if (hsm == 0)
		return;
	hsm_exit(hsm->sub, event);
	if (hsm->exit)
		hsm->exit(event);
}

hsm_t* hsm_run(hsm_t* hsm, uint32_t event) {
	if (hsm == 0)
		return 0;
	if (hsm->task) {
		hsm_t* active = hsm->task(event);
		if (hsm != active) {
			hsm_exit(hsm, event);
			hsm_enter(active, event);
			screen_changed = 1;
			current_screen_hsm = active;
			return active;
		}
	}
	hsm->sub = hsm_run(hsm->sub, event);
	return hsm;
}

hsm_t* get_current_sceen_hsm()
{
	return current_screen_hsm;
}
