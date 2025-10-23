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


void pwm_init(uint32_t channel, uint32_t freq, float duty_cycle) {
	PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));
    PWM->PWM_DIS = (1 << channel);

	// 3. Reset channel configuration
	PWM->PWM_CH_NUM[channel].PWM_CMR = 0;

	// 4. Select clock and calculate prescaler
	// We’ll use CLKA = MCK / (PREA * DIVA)
	// Configure CLKA to target frequency range

	// Example: use PREA = 0 (divide by 1), DIVA = 42 -> CLKA = 2 MHz
	// (You can adjust for your application)
	PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42); // CLKA = 84MHz / 42 = 2MHz

	// 5. Set channel to use CLKA
	PWM->PWM_CH_NUM[channel].PWM_CMR = PWM_CMR_CPRE_CLKA;

	// 6. Compute period and duty in counts
	uint32_t pwm_clk = F_CPU / 42; // 2 MHz from above
	uint32_t period = pwm_clk / freq;
	uint32_t duty = (uint32_t)(period * (duty_cycle / 100.0f));

	// 7. Set period and duty cycle
	PWM->PWM_CH_NUM[channel].PWM_CPRD = period;
	PWM->PWM_CH_NUM[channel].PWM_CDTY = duty;

	// 8. Enable output on corresponding pin (depends on channel)
	// Example for PWM channel 0 → PB0, peripheral B
	// Configure this in your main code or here:
	if (channel == 0) {
		PIOB->PIO_PDR = PIO_PB12;       // Disable GPIO on PB10
		PIOB->PIO_ABSR |= PIO_PB12;     // Select peripheral B (PWMH0)
		PIOB->PIO_PUDR = PIO_PB12;      // Disable pull-up
	} else if (channel == 1) {
		PIOB->PIO_PDR = PIO_PB13;       // Disable GPIO on PB21
		PIOB->PIO_ABSR |= PIO_PB13;     // Select peripheral B (PWMH1)
		PIOB->PIO_PUDR = PIO_PB13;      // Disable pull-up	
	}


	// 9. Enable PWM channel
	pwm_start(channel);
}

void pwm_start(uint32_t channel){
	PWM->PWM_ENA = (1 << channel);
}

void pwm_stop(uint32_t channel){
	PWM->PWM_DIS = (1 << channel);
}

void pwm_set_pulse_width(uint32_t channel, float pulse_width){
	//if (pulse_width < 0) pulse_width = 0;
	//if (pulse_width > 100) pulse_width = 100;


	// Update duty using update register (CDTYUPD)
	PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = pulse_width;

}