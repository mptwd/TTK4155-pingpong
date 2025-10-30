/*
 * score.c
 *
 * Created: 30.10.2025 13:22:55
 *  Author: marcjpo
 */ 

#include "score.h"
#include "../timer/timer.h"
#include "../ir/ir.h"
#include <sam.h>

uint8_t score;
uint32_t lockout_end;
uint8_t timer_active;
uint32_t pause_duration;

void score_init(uint32_t duration) {
	ir_init();
	timer_init();
	score = 0;
	lockout_end = 0;
	timer_active = 0;
	pause_duration = duration;
}

void score_handle() {
	uint32_t now = timer_get();
	if (timer_active) {
		if ((int32_t)(now - lockout_end) >= 0) timer_active = 0;
		else return;
	}
	if (!timer_active && ir_get_state()) {
		score++;
		timer_active = 1;
		lockout_end = now + pause_duration;
	}
}

uint8_t get_score() {
	return score;
}