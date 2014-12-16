#include "contiki.h"
#include "looci.h"
#include "utils.h"
#include "event-types.h"
#include "adc.h"

#include <stdint.h>
#include <stdio.h>

#ifdef LOOCI_COMPONENT_DEBUG
#include "debug.h"
#else
#include "nodebug.h"
#endif

#define LOOCI_COMPONENT_NAME potentio

struct state{
	looci_event_t* event;
};

#define LOOCI_NR_PROPERTIES 0
LOOCI_PROPERTIES();
COMPONENT_INTERFACES(RPC_REPLY); 
COMPONENT_RECEPTACLES(RPC_REQUEST); 
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
	printf("potentio received ev %u\n",event->type);
	if(event->type == RPC_REQUEST){
		ADCvalue = range_potentiometer(readADC(0));
		PUBLISH_EVENT(RPC_REPLY, &ADCvalue, 1);
	}
	return 1;
}



COMP_FUNCS_INIT //THIS LINE MUST BE PRESENT
COMP_FUNC_ACTIVATE(activate)
COMP_FUNC_EVENT(event)
COMP_FUNCS_END(NULL)//THIS LINE MUST BE PRESENT
