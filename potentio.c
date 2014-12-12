#include "contiki.h"
#include "looci.h"
#include "utils.h"
#include "timer_utils.h"
#include <avr/pgmspace.h>
#include "sensors.h"
#include "event-types.h"
#include "button_events.h"
#include "adc.h"

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
	int reading;
	looci_event_t* event;
};

//For receiving events, see ~/looci/looci2-contiki/components/tutorialComponents/tempFilter/tempFilter.c

#define LOOCI_NR_PROPERTIES 0
LOOCI_PROPERTIES();
COMPONENT_INTERFACES(POTENTIO_READING); 
COMPONENT_RECEPTACLES(ANY_EVENT)); 
LOOCI_COMPONENT("potentiometer sampler", struct state);

static uint8_t activate(struct state* compState, void* data){
	printf("potentiometer component activated\n");
	return 1;
}

static uint8_t event(struct state* compState, core_looci_event_t* event){
	PRINT_LN("received ev %u",event->type);
	if(event->type == POTENTIO_READING){
		PRINT_LN("is ev %u",event->type);
		PRINT_LN("publish ev %u",event->type);
		PUBLISH_EVENT(POTENTIO_READING, range_potentiometer(readADC(0)), 1);
	}
	return 1;
}

uint8_t range_potentiometer(value){
	if (value <= 210)
		return 1;
	else if (value <= 500)
		return 2;
	else if (value <= 800)
		return 3;
	else
		return 4;
}

COMP_FUNCS_INIT //THIS LINE MUST BE PRESENT
COMP_FUNC_ACTIVATE(activate)
COMP_FUNC_EVENT(event)
COMP_FUNCS_END(NULL)//THIS LINE MUST BE PRESENT
