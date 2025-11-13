/*
 * score.c
 *
 * Created: 30.10.2025 13:22:55
 *  Author: marcjpo
 */ 

#include "score.h"
#include "../timer/timer.h"
#include "../time/time.h"
#include "../ir/ir.h"
#include "../can/can.h"
#include <sam.h>

uint8_t score_initialized = 0;

uint8_t score;
uint64_t last_time;

void score_init(void) {
	if (!score_initialized) {
		ir_init();
		timer_init();
		//time_init();
		score = 0;
		last_time = totalSeconds(time_now());
		score_initialized = 1;
	}

}

void score_reset() {
	score = 0;
}

uint8_t score_handle() {
	double now = totalSeconds(time_now());
	if (now > last_time + 1.) {
		score++;
		printf("score=%d\r\n", score);
		//TODO: Send the score to Node 1
		last_time = now;
	}
	if (ir_get_state()) {
		if (ir_get_state()) {
			const CanMsg game_over_sig = (CanMsg){
				.id = 5,
				.length = 1,
				.byte[0] = score,
			};
			printf("s\r\n");
			can_tx(game_over_sig);
			pwm_set_pulse_width(0, 1200);
			score_reset();
			return 0; // stop playing
		}

	}
	return 1; // keep playing
}

uint8_t get_score() {
	return score;
}