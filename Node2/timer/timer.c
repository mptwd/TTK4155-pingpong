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

void timer_init() {
	//PMC->PMC_PCER0 |= (1 << ID_TC0);   // enable clock for TC0
	PMC->PMC_PCER1 = (1 << (ID_TC6 - 32));
	TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4;  // MCK/128
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // enable + reset
	
	PMC->PMC_PCER0 |= (1 << ID_TC2);   // enable clock for TC2
	TC2->TC_BMR = TC_BMR_QDEN          // activer quadrature decoder
				| TC_BMR_POSEN          // activer position
				| TC_BMR_EDGPHA         // front actif sur A et B
				| TC_BMR_MAXFILT(1);    // filtrage minimal
				
	PIOC->PIO_PDR = PIO_PC25;       // Disable GPIO on PB10
	PIOC->PIO_ABSR |= PIO_PC25;     // Select peripheral B (PWMH0)
	PIOA->PIO_PDR = PIO_PA29;       // Disable GPIO on PB10
	PIOA->PIO_ABSR |= PIO_PA29;     // Select peripheral B (PWMH0)
		
	TC2->TC_CHANNEL[0].TC_CMR = 0;
	TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
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


void pwm_init(uint32_t channel, uint32_t freq) {
	PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));
    PWM->PWM_DIS = (1 << channel);

	// Reset channel configuration
	PWM->PWM_CH_NUM[channel].PWM_CMR = 0;

	// Select clock and calculate prescaler
	// We’ll use CLKA = MCK / (PREA * DIVA)
	// Configure CLKA to target frequency range
	
	PWM->PWM_CLK = PWM_CLK_PREA(1) | PWM_CLK_DIVA(1) // CLKA = 84MHz / 4 = 21MHz
					| PWM_CLK_PREB(0) | PWM_CLK_DIVB(42); // CLKB = 84MHz / 42 = 2MHz

	if (channel == 1) {
		// Set channel to use CLKA
		PWM->PWM_CH_NUM[channel].PWM_CMR = PWM_CMR_CPRE_CLKB;

		// Compute period and duty in counts
		uint32_t pwm_clk = F_CPU / 42; // 2 MHz from above
		uint32_t period = pwm_clk / freq; // 2000000 / 5000 = 400. 2000000/50 = 40000 See page 1048 datasheet

		// Set period and duty cycle
		PWM->PWM_CH_NUM[channel].PWM_CPRD = period;
		PWM->PWM_CH_NUM[channel].PWM_CDTY = 0;
		
		PIOB->PIO_PDR = PIO_PB13;       // Disable GPIO on PB13
		PIOB->PIO_ABSR |= PIO_PB13;     // Select peripheral B (PWMH1)
	} else if (channel == 0) {
		PWM->PWM_CH_NUM[channel].PWM_CMR = PWM_CMR_CPRE_CLKA;
		uint32_t pwm_clk = F_CPU / 2; // 42 MHz from above
		uint32_t period = pwm_clk / freq; // 42 000 000 / 35 000 = 1200. See page 1048 datasheet
		printf("%d\r\n", period);
		// Set period and duty cycle
		PWM->PWM_CH_NUM[channel].PWM_CPRD = period;
		PWM->PWM_CH_NUM[channel].PWM_CDTY = 0;
		
		PIOB->PIO_PDR = PIO_PB12;       // Disable GPIO on PB12
		PIOB->PIO_ABSR |= PIO_PB12;     // Select peripheral B (PWMH0)
	}
	
	// Enable PWM channel
	pwm_start(channel);
}

void pwm_start(uint32_t channel){
	PWM->PWM_ENA = (1 << channel);
}

void pwm_stop(uint32_t channel){
	PWM->PWM_DIS = (1 << channel);
}

void pwm_set_pulse_width(uint32_t channel, uint32_t pulse_width){
	// Update duty using update register (CDTYUPD)
	PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = pulse_width;

}
