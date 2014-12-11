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

#define LOOCI_NR_PROPERTIES 0
LOOCI_PROPERTIES();
COMPONENT_INTERFACES(POTENTIO_READING); 
COMPONENT_RECEPTACLES(ANY_EVENT)); 
LOOCI_COMPONENT("potentiometer sampler", struct state);
COMPONENT_THREAD( ev, data)
{
	COMPONENT_BEGIN(struct state,compState);
	compState->reading = 0;
	while(1) {
		LOOCI_WAIT_EVENT();
		// create dummy value when timer expires
		compState->reading = range_potentiometer(readADC(0));
		PUBLISH_EVENT(POTENTIO_READING, (uint8_t)&compState->reading,
		 sizeof(compState->reading));
	}
	COMPONENT_END();
}

int range_potentiometer(value){
	if (value <= 210)
		return 1;
	else if (value <= 500)
		return 2;
	else if (value <= 800)
		return 3;
	else
		return 4;
}
