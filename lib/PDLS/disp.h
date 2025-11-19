
#ifndef DISP_H_
#define DISP_H_

#include "common_def.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t is_scd4x_sensor_present;

void disp_init(void);
void disp_loop(void);
void setup_text_example();
void loop_example();

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif //DISP_H_