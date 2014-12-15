#include "contiki.h"
#include "looci.h"
#include "utils.h"
#include "timer_utils.h"
#include <avr/pgmspace.h>
#include "sensors.h"
#include "event-types.h"
//#include "button_events.h"
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

#define POTENTIO_READING 500
#define POTENTIO_REQUEST 501
#define BUTTON_PRESSED 413
#define LOOCI_COMPONENT_NAME potentio


/*
static uint8_t activate(struct state*, void*);
static uint8_t event(struct state*, core_looci_event_t*);
static uint8_t range_potentiometer(int);
*/

struct state{
	looci_event_t* event;
};

//For receiving events, see ~/looci/looci2-contiki/components/tutorialComponents/tempFilter/tempFilter.c

#define LOOCI_NR_PROPERTIES 0
LOOCI_PROPERTIES();
COMPONENT_INTERFACES(POTENTIO_READING); 
COMPONENT_RECEPTACLES(ANY_EVENT); 
LOOCI_COMPONENT("potentiometer reader", struct state);

static uint8_t range_potentiometer(int value){
	if (value <= 210)
		return 1;
	else if (value <= 500)
		return 2;
	else if (value <= 800)
		return 3;
	else
		return 4;
}

static uint8_t activate(struct state* compState, void* data){
	printf("potentiometer component activated\n");
	return 1;
}

static uint8_t event(struct state* compState, core_looci_event_t* event){
	uint8_t ADCvalue;
	PRINT_LN("received ev %u",event->type);
	if(event->type == POTENTIO_REQUEST){
		ADCvalue = range_potentiometer(readADC(0));
		PUBLISH_EVENT(POTENTIO_READING, &ADCvalue, 1);
	}
	return 1;
}



COMP_FUNCS_INIT //THIS LINE MUST BE PRESENT
COMP_FUNC_ACTIVATE(activate)
COMP_FUNC_EVENT(event)
COMP_FUNCS_END(NULL)//THIS LINE MUST BE PRESENT
