#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#define VNkeyboard 22 /* Interrupt vector for Keyboard */ 

typedef	unsigned char	uint8;
typedef	unsigned short	uint16;
typedef	unsigned long	uint32;

typedef	char			int8;
typedef	short			int16;
typedef	long			int32;

// To Clear or Set single bits in a byte variable.
#define	b_SetBit(bit_ID, varID)		(varID |= (uint8)(1<<bit_ID))
#define	b_ClearBit(bit_ID, varID)	(varID &= ~(uint8)(1<<bit_ID))
#define	b_XorBit(bit_ID, varID)		(varID ^= (uint8)(1<<bit_ID))

// To Clear or Set single bits in a word(16-bit) variable.
#define	w_SetBit(bit_ID, varID)		(varID |= (uint16)(1<<bit_ID))
#define	w_ClearBit(bit_ID, varID)	(varID &= ~(uint16)(1<<bit_ID))
#define	w_XorBit(bit_ID, varID)		(varID ^= (uint16)(1<<bit_ID))

// To check single bit status in a given variable in ZERO PAGE
#define	b_CheckBit(varID, bit_ID)	((varID & (uint8)(1<<bit_ID))?1:0)
//#define	b_CheckBit(varID, bit_ID)	(varID & (muint8)(1<<bit_ID))
#define	w_CheckBit(varID, bit_ID) ((varID & (uint16)(1<<bit_ID))?1:0)

// To check single bit status in a given variable in ZERO PAGE
#define		DummyRead(varID)			{__asm lda varID;}

int led_no=0;

void init(void);


void main(void) {
  EnableInterrupts;			//enable global interrupts
  /* include your code here */
  init();					//calling the method that configures the ports and registers
  

  while(1)
  	{
  		__RESET_WATCHDOG(); 
  	}
  /* please make sure that you never leave main */
}


interrupt VNkeyboard void intKBI_SW()
{
	//i use a global variable to turn on specific LEDs depending on the variable value. I update the variable before finishing the
	//interrupt to have the variable ready for the next interrupt.
	
	if(led_no == 0){
		PTFD = 0x0;
		PTFD_PTFD0=1;
	}
	if(led_no == 1){
		PTFD = 0x0;
		PTFD_PTFD1=1;
		}
	if(led_no == 2){
		PTFD = 0x0;
		PTFD_PTFD2=1;
		}
	if(led_no == 3){
		PTFD = 0x0;
		PTFD_PTFD3=1;
		}
	if(led_no == 4){
		PTFD = 0x0;
		PTFD_PTFD4=1;
		}
	if(led_no == 5){
		PTFD = 0x0;
		PTFD_PTFD5=1;
		}
	if(led_no == 6){
		PTFD = 0x0;
		PTFD_PTFD6=1;
		}
	if(led_no == 7){
		PTFD = 0x0;
		PTFD_PTFD7=1;
		}
	
	// if last LED has been turned on, i "reset" the animation to start again
	if(led_no == 7){
		led_no = 0;
	}
	else{
		led_no += 1;
	}
	
	KBI1SC_KBACK = 1; /*acknowledge interrupt*/
}
void init(){
	SOPT = 0x00;   			//disable watchdog
	
	PTFDD = 0xFF;  			//set all port F pins as output
	PTFD = 0x0; 			//turn off all LEDs
	
							//For pushbutton switches we can use either PTC2 PTC6 PTD3 or PTD2
							// However only PTD3 and PTD2 can trigger interrupts.
							//we will set KBIP5 *port D2 pin*
	
	
	PTDDD = 0xA9; 			//set port D pin 2 as input
						// note: we could have set it as 0b11111011 as well
	
	PTDPE_PTDPE2 = 1; 		//enabled port D pin 2 pull ups

	KBIPE_KBIPE5 = 1; 		//enable KBPIE 5 pin
	
	KBI1SC_KBEDG5 = 0; 		//falling edge trigger  for the pin (pull up pin)
	
	KBI1SC = 0b11110010;	//bit7:4 = 0 => trigger on rising edge for all KBI ports
							//bit3 = 0   => no KBI interrupt pending
							//bit2 = 0   => write-only
							//bit1 = 1   => KBI generates hardware interrupt
							//bit0 = 0   => edge-only detection
						//note: I set bit0 as 0  -KBIMOD-  (in course we have set it as 1) to trigger interrupts only the moment 
						//i "click" and let go the button, not while having it not pressed at all.
	
	KBI1PE_KBIPE5 = 1;  //enable keyboard interrupt on the pin
	
}
