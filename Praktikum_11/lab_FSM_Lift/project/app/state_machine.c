/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module state_machine.
 * --
 * -- $Id: state_machine.c 5526 2022-01-18 07:26:31Z ruan $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>

/* user includes */
#include "state_machine.h"
#include "action_handler.h"
#include "timer.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define SAFETY_DURATION      150u       // 150 * 10ms = 1.5s
#define SIGNAL_DURATION      100u       // 100 * 10ms = 1s

#define TEXT_F0_OPENED       "F0_OPENED"
#define TEXT_F0_CLOSED       "F0_CLOSED"
#define TEXT_F1_OPENED       "F1_OPENED"
#define TEXT_F1_CLOSED       "F1_CLOSED"
#define TEXT_MOVING_UP       "MOVING_UP"
#define TEXT_MOVING_DOWN     "MOVING_DOWN"

/// STUDENTS: To be programmed




/// END: To be programmed


/* -- Type definitions
 * ------------------------------------------------------------------------- */

// definition of FSM states
typedef enum {
    /* task 4.1 */
    F0_OPENED,
    F0_CLOSED,
    
    /* task 4.2 */
    F1_OPENED,
    F1_CLOSED,
    MOVING_UP,
    MOVING_DOWN,

    /// STUDENTS: To be programmed
		F0_DELAYED_START,
		F1_DELAYED_START,
	
    /// END: To be programmed
   
} state_t;


/* Module-wide variables & constants
 * ------------------------------------------------------------------------- */

// current FSM state 
static state_t current_state = F0_CLOSED;


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void fsm_init(void)
{
    action_handler_init();
    ah_show_exception(NORMAL, "");
    
    /* go to initial state & do initial actions */
    
    /// STUDENTS: To be programmed

	current_state = F0_CLOSED;
	ah_show_state(TEXT_F0_CLOSED);
	
	


    /// END: To be programmed
}


/*
 * See header file
 */
void fsm_handle_event(event_t event)
{
    /// STUDENTS: To be programmed
switch (current_state) {  // switch state depending on the current state of the system
 		case F0_CLOSED: // if f0 is closed
 			switch (event) {  // switch case for the interrupts that need handling.
 				case EV_DOOR0_OPEN_REQ: // open interrupt
					timer_stop();
 					ah_door(DOOR_OPEN);  // open door
					ah_show_state(TEXT_F0_OPENED);
 					current_state = F0_OPENED;
					break;
				
				case EV_BUTTON_F1:  // press button f1 interrupt
					ah_door(DOOR_LOCK);  // lock the door again
					ah_show_state("MOVING_UP_DELAY");
					timer_start(SAFETY_DURATION);
					current_state = F0_DELAYED_START;
					break;
				
 			default: 
				break;
			
		} break;
			
		
		case F0_OPENED:  // if door f0 is open
			switch (event) {  // switch case for the interrupts that need handling.
 				case EV_DOOR0_CLOSE_REQ:  // close interrupt
 					ah_door(DOOR_CLOSE);  // close door
					ah_show_state(TEXT_F0_CLOSED);
 					current_state = F0_CLOSED;
	 				break;
				
 			default:
				break;
			
		} break;
			
		case MOVING_UP:  // case when the lift is moving up
 			switch (event) {  // switch case for the interrupts that need handling.
 				case EV_F1_REACHED:  // interrupt f1 reached
 					ah_motor(MOTOR_OFF);  // turn motor off
 					ah_door(DOOR_UNLOCK);  // unlock the doors
					ah_show_state(TEXT_F1_CLOSED);
					current_state = F1_CLOSED;
 				break;
				
 			default:
				break;
			
		} break;
			
		
		case F1_CLOSED:  //case when f1 is closed
 			switch (event) {  // switch case for the interrupts that need handling.
 				case EV_DOOR1_OPEN_REQ: // dopen door interrupt
 					ah_door(DOOR_OPEN);
					ah_show_state(TEXT_F1_OPENED);
					current_state = F1_OPENED;
 				break;
				
				case EV_BUTTON_F0:  // button f0 press interrupt
					ah_door(DOOR_LOCK);
					ah_show_state("F1_DELAYED_START");
					timer_start(SAFETY_DURATION);
					current_state = F1_DELAYED_START;
				break;
				
 			default:
				break;
			
		} break;
			
		
		case F1_OPENED:  // case when f1 is open
 			switch (event) {  // switch case for the interrupts that need handling.
 				case EV_DOOR1_CLOSE_REQ:  // close door interrupt
 					ah_door(DOOR_CLOSE);
					ah_show_state(TEXT_F1_CLOSED);
					current_state = F1_CLOSED;
 				break;
				
 			default:
				break;
			
		} break;
			
		case MOVING_DOWN:  // case when lift is moving down
 			switch (event) {  // switch case for the interrupts that need handling.
 				case EV_F0_REACHED:
					ah_motor(MOTOR_OFF);
 					ah_door(DOOR_UNLOCK);
					ah_show_state(TEXT_F0_CLOSED);
					current_state = F0_CLOSED;
					ah_signal(SIGNAL_OFF);
 				break;
				
 			default:
				break;
			
		} break;
		
		case F0_DELAYED_START:  // case start delayed from f0
 			switch (event) {  // switch case for the interrupts that need handling.
 				case EV_TIMEOUT: // time out interrupt
					timer_stop();
					ah_motor(MOTOR_UP);
					ah_show_state(TEXT_MOVING_UP);
					current_state = MOVING_UP;
 				break;
				
 			default:
				break;
			
		} break;
 		
		case F1_DELAYED_START:  // case start delayed from f1
 			switch (event) {  // switch case for the interrupts that need handling.
 				case EV_TIMEOUT:  // time out interrupt
					timer_stop();
					ah_motor(MOTOR_DOWN);
					ah_show_state(TEXT_MOVING_DOWN);
					current_state = MOVING_DOWN;
 				break;
				
 			default:
				break;
			
		} break;

	}

    /// END: To be programmed
 }
