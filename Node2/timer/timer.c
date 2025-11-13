/*
 * timer.c
 *
 * Created: 23.10.2025 11:58:49
 *  Author: marcjpo
 */ 

#include "timer.h"
#include "sam.h"

#include <stdio.h>

#define F_CPU 84000000

uint8_t timer_initialized = 0;

/* ====> TIMERS <==== */
void timer_init() {
	if (!timer_initialized) {
		/* Init timer 0 (for score) */
		PMC->PMC_PCER1 = (1 << (ID_TC6 - 32));
		TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4;  // MCK/128
		TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // enable + reset
	
		/* Init timer 2 as quadrature decoder (decoding motor position) */
		PMC->PMC_PCER0 |= (1 << ID_TC2);   // enable clock for TC2
		TC2->TC_BMR = TC_BMR_QDEN          // enable quadrature decoder
					| TC_BMR_POSEN          // activate position
					| TC_BMR_EDGPHA         // actif edge on A and B
					| TC_BMR_MAXFILT(1);    // minimal filtering
		
		TC2->TC_CHANNEL[0].TC_CMR = 0;
		TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
		timer_initialized = 1;
	}
}

uint32_t timer_get(uint8_t timer) {
	if (timer == 0) {
		return TC0->TC_CHANNEL[0].TC_CV;
	} else if (timer == 1) {
		return TC1->TC_CHANNEL[0].TC_CV;
	} else if (timer == 2) {
		return TC2->TC_CHANNEL[0].TC_CV;
    }
	return 0;
}

uint8_t timer_get_dir() {
	return (TC2->TC_QISR & TC_QISR_DIR) ? 1 : 0;
}

/* ================== */


/* ====> PWM <==== */

void pwm_init(uint32_t channel, uint32_t freq) {
	//if (channel != 0 && channel != 1) return;
	PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));
	PWM->PWM_DIS = (1 << channel);

	// Reset channel configuration
	PWM->PWM_CH_NUM[channel].PWM_CMR = 0;

	// Two different PWM clocks are used.
	PWM->PWM_CLK = PWM_CLK_PREA(1) | PWM_CLK_DIVA(1) // CLKA = 84MHz / 2 = 42MHz
					| PWM_CLK_PREB(0) | PWM_CLK_DIVB(42); // CLKB = 84MHz / 42 = 2MHz

	if (channel == 1) {
		// Set channel to use CLKB
		PWM->PWM_CH_NUM[channel].PWM_CMR = PWM_CMR_CPRE_CLKB;

		// Compute period (CPRD)
		uint32_t pwm_clk = F_CPU / 42; // 2 MHz from above
		uint32_t period = pwm_clk / freq; // 2000000 / 5000 = 400. 2000000/50 = 40000 See page 1048 datasheet

		// Set period and duty cycle
		PWM->PWM_CH_NUM[channel].PWM_CPRD = period;
		PWM->PWM_CH_NUM[channel].PWM_CDTY = 0;
		
		PIOB->PIO_PDR = PIO_PB13;       // Disable GPIO on PB13
		PIOB->PIO_ABSR |= PIO_PB13;     // Select peripheral B (PWMH1)
	} else if (channel == 0) {
		// Set channel to use CLKA
		PWM->PWM_CH_NUM[channel].PWM_CMR = PWM_CMR_CPRE_CLKA;
		
		// Compute period (CPRD)
		uint32_t pwm_clk = F_CPU / 2; // 42 MHz from above
		uint32_t period = pwm_clk / freq; // 42 000 000 / 35 000 = 1200. See page 1048 datasheet
		
		// Set period and duty cycle
		PWM->PWM_CH_NUM[channel].PWM_CPRD = period;
		PWM->PWM_CH_NUM[channel].PWM_CDTY = 0;
		
		PIOB->PIO_PDR = PIO_PB12;       // Disable GPIO on PB12
		PIOB->PIO_ABSR |= PIO_PB12;     // Select peripheral B (PWMH0)
	}
	pwm_start(channel);
}

void pwm_start(uint32_t channel){
	PWM->PWM_ENA = (1 << channel);
}

void pwm_stop(uint32_t channel){
	PWM->PWM_DIS = (1 << channel);
}

void pwm_set_pulse_width(uint32_t channel, uint32_t pulse_width){
	PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = pulse_width; // Pulse width is supposed to be between 0 and PWM_CPRD
}

/* =============== */