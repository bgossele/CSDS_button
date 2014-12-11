#include "contiki.h"
#include "looci.h"
#include "utils.h"
#include "timer_utils.h"
#include <avr/pgmspace.h>
#include "sensors.h"
#include "event-types.h"
#include "button-events.h"

#ifdef CONTIKI_TARGET_AVR_RAVEN
#include "raven-msg.h"
#endif

#include <stdint.h>
#include <stdio.h>

#ifdef LOOCI_COMPONENT_DEBUG
#include "debug.h"
#else
#include "nodebug.h"
#endif

struct state{
	int code;
	int digits_sampled;
	looci_event_t* event;
};

#define LOOCI_NR_PROPERTIES 0
LOOCI_PROPERTIES();
COMPONENT_INTERFACES(BUTTON_PRESSED, POTENTIO_REQUEST); //Button_pressed, potentio_request
COMPONENT_RECEPTACLES(POTENTIO_READING); //Potentio_reading
LOOCI_COMPONENT("code reader", struct state);
COMPONENT_THREAD( ev, data)
{
	COMPONENT_BEGIN(struct state,compState);
	compState->code = 0;
	ETIMER_SET(&compState->et, CLOCK_SECOND * 10);
	SREG |= 10000000;
	EICRA = 0x03;
	EIMSK = 0x01;

	while(1) {
		LOOCI_WAIT_EVENT(compState->event,
			ETIMER_EXPIRED(compState->et));
		// create dummy value when timer expires
		compState->digits_sampled += 1;
		PUBLISH_EVENT(BUTTON_PRESSED, (uint8_t)&compState->light,
		 sizeof(compState->light));
	}
	COMPONENT_END();
}

ISR(INT0_vect){
	printf("button pressed!\n");
	PUBLISH_EVENT(POTENTIO_REQUEST);
	printf("new code = %d\n", code);
}

void update_code(value){
	code = (code % 1000) * 10 + range_potentiometer(value);
}
