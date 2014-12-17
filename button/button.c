#include "contiki.h"
#include "looci.h"
#include "timer_utils.h"
#include "event-types.h"

#include <stdint.h>
#include <stdio.h>

#ifdef LOOCI_COMPONENT_DEBUG
#include "debug.h"
#else
#include "nodebug.h"
#endif

#define CODE_READ 413
#define LOOCI_COMPONENT_NAME button

struct state{
	uint8_t code;
	uint8_t digits_sampled;
	uint8_t interrupt;
	struct etimer et;
};

int request = 0;

#define LOOCI_NR_PROPERTIES 0
LOOCI_PROPERTIES();
COMPONENT_INTERFACES(CODE_READ, RPC_REQUEST);
COMPONENT_RECEPTACLES(RPC_REPLY);
LOOCI_COMPONENT("button listener", struct state);

static void printTheCode(uint8_t);

static uint8_t activate(struct state* compState, void* data){
	SREG |= 10000000;
	EICRA = 0x03;
	EIMSK = 0x01;
	printf("button listener activated\n");
	ETIMER_SET(&compState->et, CLOCK_SECOND*0.5);
	return 1;
}

static uint8_t event(struct state* compState, core_looci_event_t* event){
	uint8_t offset;
	uint8_t andBits;
	printf("button received ev %u\n",event->type);
	if(event->type == RPC_REPLY){
		
		printf("Input: %d \n",*(event->payload));
		compState->code *= 4;
		compState->code |= (*(event->payload)-1);
				
		compState->digits_sampled += 1;

		if(compState->digits_sampled == 4){	
			PUBLISH_EVENT(CODE_READ, &(compState->code), 1);
			printTheCode(compState->code);
			compState->digits_sampled = 0;
		}
	}
	return 1;
}

static uint8_t time(struct state* compState, void* data){
	if(request==1){
		PUBLISH_EVENT(RPC_REQUEST, NULL, 0);
		request=0;
	}
	ETIMER_RESET(&compState->et);
	return 1;
}

ISR(INT0_vect){
	request=1;
}

static void printTheCode(uint8_t number) {
	int number1;
	int number2;
	int number3;
	int number4;
	char str[5];
    
	number1 = (3 & (number >> 6))+1;
	number2 = (3 & (number >> 4))+1;
	number3 = (3 & (number >> 2))+1;
	number4 = (3 & number)+1;
	
	printf("%d%d%d%d\n",number1,number2,number3,number4);
}

COMP_FUNCS_INIT //THIS LINE MUST BE PRESENT
COMP_FUNC_ACTIVATE(activate)
COMP_FUNC_EVENT(event)
COMP_FUNC_TIMER(time)
COMP_FUNCS_END(NULL)//THIS LINE MUST BE PRESENT
