#include <stdio.h>
#include <stdbool.h>
#include "system.h"
#include "sys/alt_irq.h"

unsigned char seven_seg_table[] ={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07,
									0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };

volatile int edge_capture;	//volatile => means it's value can change outside main program flow (due to interrupts) used to pass context to the ISRs

volatile int * key_ptr = (int *) KEY_BASE;
volatile int * timer_ptr = (int *) TIMER_BASE;
volatile int * sw_ptr = (int *) SW_BASE;
volatile int * hex_ptr = (int *) HEX_BASE ;

volatile int switch_value ; //To store the current state of the switch

//functions declaration :

//Interrupt functions
void handle_key_interrupts(void* context, alt_u32 id);
void handle_timer_interrupts(void* context, alt_u32 id);
void handle_switch_interrupts(void* context, alt_u32 id);

//Displaying functions
void display_time(int heure , int minute);
void display_chrono(int minute , int seconde);

unsigned int milisec = 0;

unsigned int seconde = 0;
unsigned int minute = 0;
unsigned int heure = 0;

unsigned int secondech = 0;
unsigned int minutech = 0;

bool isTime = true ;	//At first we would be displaying time
bool adjust_time = false;
bool start_chrono = false ;

bool isHour = true; //this boolean used to adjust time mode to know if we're working with hours or minutes
//---------------------------------------------------------------------------------------------------------------------
//Initializing interrupts functions:

void init_key(){
	void* edge_capture_ptr = (void*) &edge_capture;
	* (key_ptr + 2) = 0xf; // Masque de l'interruption "1111"
	 alt_irq_register( KEY_IRQ, edge_capture_ptr,handle_key_interrupts);	//Registers an ISR (Interrupt service routine) for a specific interrupt source.
}

void init_sw() {
    void* edge_capture_ptr = (void*) &edge_capture; // Pointer to edge capture variable
    *(sw_ptr + 2) = 0x3; // Enable interrupts for only SW0 and SW1
    alt_irq_register(SW_IRQ, edge_capture_ptr, handle_switch_interrupts); // Register ISR
}

void init_timer(){
	void* edge_capture_ptr = (void*) &edge_capture;
	* (key_ptr + 1) = 0x7; // Start the timer
	 alt_irq_register( TIMER_IRQ, edge_capture_ptr,handle_timer_interrupts);
}

//---------------------------------------------------------------------------------------------------------------------
//Interrupt functions implementation :

void handle_key_interrupts(void* context, alt_u32 id){
	int press = * (key_ptr +3); // read register of peripheral

	if(press&0x1){
		printf("Program should be reseted ! \n");

	}
	//adjusting time with key1(to increase) , key2(to decrease) and key3(to move from hour to minute and vice versa)
	else if(adjust_time & isTime){
		if(press&0x8){
			isHour = !isHour ;
		}
		if(press&0x2){
			if(isHour){
				heure++ ;
				if(heure == 24) heure = 0 ;
			}
			else{
				minute++;
			}
		}
		if(press&0x4){
			if(isHour){
				heure-- ;
			}
			else{
				minute--;
			}
		}
	}
	//chronometer mode to start with key1 , pause with key2 and stop with key3
	else if(!isTime){
		if (press&0x2){
			printf("Chronometer started !!\n");
			start_chrono = true ;
		}
		else if (press&0x4){
			printf("Chronometer paused !!\n");
			start_chrono = false ;
		}
		else if (press&0x8){
			printf("Chronometer stopped !!\n");
			start_chrono = false ;
			secondech = 0 ;
			minutech = 0 ;
		}
	}

	* (key_ptr + 3) =0;  // effacer le registre du periph.
}

void handle_switch_interrupts(void* context, alt_u32 id) {
	switch_value = *(sw_ptr + 3); // Read the edge capture register

    // Process the interrupt (e.g., log switch states)
    printf("Switch interrupt! Edge capture: 0x%X\n", switch_value);

    if(switch_value&0x1){
    	isTime = !isTime ;
    }
    else if(switch_value&0x2){
    	adjust_time = !adjust_time ;
    }
    // Clear the edge capture register
    *(sw_ptr + 3) = 0;
}

void handle_timer_interrupts(void* context, alt_u32 id){
	*(timer_ptr) = 0;	//clear the timer interrupt flag
	milisec = milisec + 1;
	if (milisec== 1000){
		milisec=0;
		seconde ++;
		if(start_chrono){
			secondech ++ ;
		}

		//Incrementing time :
		if (seconde==60){
			seconde=0;
			minute ++;
			if (minute==60){
				minute=0;
				heure++;
				if(heure==24){
					heure=0;
				}
			}
		}

		//Incrementing chronometer:
		if(secondech == 60){
			secondech=0;
			minutech ++ ;
			if(minutech==60){
				minutech=0 ;
			}
		}

		printf("heure actuelle  %d : %d \n", heure,minute);
		printf("chrono actuelle  %d : %d\n", minutech,secondech);

		if(isTime){
			display_time(heure,minute);
		}
		else{
			display_chrono(minutech,secondech);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
//Display functions implementation :

void display_chrono(int minute , int seconde){
	int m2 = minute/10 ;
	int m1 = minute%10 ;
	int s2 = seconde/10 ;
	int s1 = seconde%10 ;

	* hex_ptr = seven_seg_table[s1] + (seven_seg_table[s2] << 8 ) + (seven_seg_table[m1] << 16 ) +(seven_seg_table[m2] << 24 ) ;
}

void display_time(int heure , int minute){
	int h2 = heure/10 ;
	int h1 = heure%10 ;
	int m2 = minute/10 ;
	int m1 = minute%10 ;

	* hex_ptr = seven_seg_table[m1] + (seven_seg_table[m2] << 8 ) + (seven_seg_table[h1] << 16 ) +(seven_seg_table[h2] << 24 ) ;
}

//---------------------------------------------------------------------------------------------------------------------
//main function :

int main()
{
  printf("Welcome to your DE1 Smartwatch");
  init_key();
  init_timer();
  init_sw();
  while(1){}
  return 0;
}
