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
	uint8_t code;
	uint8_t digits_sampled;
	looci_event_t* event;
};

#define LOOCI_NR_PROPERTIES 0
LOOCI_PROPERTIES();
COMPONENT_INTERFACES(BUTTON_PRESSED, POTENTIO_REQUEST);
COMPONENT_RECEPTACLES(POTENTIO_READING);
LOOCI_COMPONENT("code reader", struct state);

static uint8_t activate(struct state* compState, void* data){
	SREG |= 10000000;
	EICRA = 0x03;
	EIMSK = 0x01;
	printf("button component activated\n");
	return 1;
}

static uint8_t event(struct state* compState, void* data){
	PRINT_LN("received ev %u",event->type);
	if(event->type == POTENTIO_READING){
		PRINT_LN("is ev %u",event->type);
		compState->code &= (event->payload << (compState->digits_sampled)); 

		if(compState->digits_sampled == 3)		
			PUBLISH_EVENT(BUTTON_PRESSED, compState->code, 1);
		}
		compState->digits_sampled += 1;
		compState->digits_sampled %= 4;
	}
	return 1;
}

ISR(INT0_vect){
	printf("button pressed!\n");
	PUBLISH_EVENT(POTENTIO_REQUEST);
}

void update_code(value){
	code = (code % 1000) * 10 + range_potentiometer(value);
}

COMP_FUNCS_INIT //THIS LINE MUST BE PRESENT
COMP_FUNC_ACTIVATE(activate)
COMP_FUNC_EVENT(event)
COMP_FUNCS_END(NULL)//THIS LINE MUST BE PRESENT
