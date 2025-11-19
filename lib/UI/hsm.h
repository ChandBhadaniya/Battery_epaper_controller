/*
 * hsm.h
 *
 *  Created on: Jun 25, 2021
 *      Author: Biren
 */

#ifndef UI_HSM_H_
#define UI_HSM_H_

// Configuration
#include "hV_Configuration.h"

typedef struct hsm hsm_t;
typedef void (*hsm_transition_fp)(uint32_t);
typedef hsm_t* (*hsm_task_fp)(uint32_t);

struct hsm {
	hsm_transition_fp enter;
	hsm_task_fp task;
	hsm_transition_fp exit;
	struct hsm* sub;
};

hsm_t* hsm_run(hsm_t* hsm, uint32_t event);
hsm_t* get_current_sceen_hsm();

#endif /* UI_HSM_H_ */
