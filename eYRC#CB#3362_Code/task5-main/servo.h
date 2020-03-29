/*
*
*Team Id: <3362>
* Author List: <Nikhil,Vansh,Ujjwal,Vivek>
* Filename: <servo.h>
* Theme: <Construct-O-Bot -- Specific to eYRC>
* Functions: <servo_1,servo_2,servo_3,servo_1_free,servo_2_free,servo_3_free,servo_neutral,pickup_from_left,pickup_from_right>
* 
*
*/
void servo_1(unsigned char degrees);
void servo_2(unsigned char degrees);
void servo_3(unsigned char degrees);
void servo_1_free(void);
void servo_2_free(void);
void servo_3_free(void);

/*
*
* Function Name: timer1_init
* Input: node
* Output: void
* Logic: uses timer to rotate servo
* Example Call: timer1_init();
*
*/


void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFC; //Counte.r high value to which OCR1xH value is to be compared with
 TCNT1L = 0x01;	//Counter low value to which OCR1xH value is to be compared with
 OCR1AH = 0x03;	//Output compare Register high value for servo 1
 OCR1AL = 0xFF;	//Output Compare Register low Value For servo 1
 OCR1BH = 0x03;	//Output compare Register high value for servo 2
 OCR1BL = 0xFF;	//Output Compare Register low Value For servo 2
 OCR1CH = 0x03;	//Output compare Register high value for servo 3
 OCR1CL = 0xFF;	//Output Compare Register low Value For servo 3
 ICR1H  = 0x03;	
 ICR1L  = 0xFF;
 TCCR1A = 0xAB; /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
 					For Overriding normal port functionality to OCRnA outputs.
				  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
 TCCR1C = 0x00;
 TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}

/*
*
* Function Name: servo1_pin_config
* Input: node
* Output: void
* Logic: Defines pins for servo1
* Example Call: servo1_pin_config(); 
*
*/
void servo1_pin_config (void)
{
 DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
 PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}




/*
*
* Function Name: servo2_pin_config
* Input: node
* Output: void
* Logic: Defines pins for servo2
* Example Call: servo2_pin_config();
*
*/
void servo2_pin_config (void)
{
 DDRB  = DDRB | 0x40;  //making PORTB 6 pin output
 PORTB = PORTB | 0x40; //setting PORTB 6 pin to logic 1
}


/*
*
* Function Name: servo3_pin_config
* Input: node
* Output: void
* Logic: Defines pins for servo3
* Example Call: servo3_pin_config();
*
*/
void servo3_pin_config (void)
{
 DDRB  = DDRB | 0x80;  //making PORTB 7 pin output
 PORTB = PORTB | 0x80; //setting PORTB 7 pin to logic 1
}


/*
*
* Function Name: servo4_pin_config
* Input: node
* Output: void
* Logic: Defines pins for servo4
* Example Call: servo4_pin_config();
*
*/
void servo4_pin_config(void)
{
    DDRB = DDRB | 0x08;      //making PORTB 4 pin output
    PORTB = PORTB | 0x08;    //setting PORTB 4 pin to logic 1
}
/*
*
* Function Name: servo_port_init
* Input: node
* Output: void
* Logic: Initialize the ports
* Example Call: servo_port_init();
*
*/
void servo_port_init(void)
{
 servo1_pin_config(); //Configure PORTB 5 pin for servo motor 1 operation
 servo2_pin_config(); //Configure PORTB 6 pin for servo motor 2 operation
 servo3_pin_config(); //Configure PORTB 7 pin for servo motor 3 operation
 servo4_pin_config(); //Configure PORTB 4 pin for servo motor 4 operation
}

/*
*
* Function Name: servo_1
* Input: unsigned char
* Output: void
* Logic: Function to rotate Servo 1 by a specified angle in the multiples of 1.86 degrees
* Example Call: servo_1();
*
*/
void servo_1(unsigned char degrees)
{
    float PositionPanServo = 0;
    PositionPanServo = ((float)degrees / 1.86) + 35.0;
    OCR1AH = 0x00;
    OCR1AL = (unsigned char) PositionPanServo;
}


/*
*
* Function Name: servo_2
* Input: unsigned char
* Output: void
* Logic: Function to rotate Servo 2 by a specified angle in the multiples of 1.86 degrees
* Example Call: servo_2();
*
*/
void servo_2(unsigned char degrees)
{
    float PositionTiltServo = 0;
    PositionTiltServo = ((float)degrees / 1.86) + 35.0;
    OCR1BH = 0x00;
    OCR1BL = (unsigned char) PositionTiltServo;
}

/*
*
* Function Name: servo_3
* Input: unsigned char
* Output: void
* Logic: Function to rotate Servo 3 by a specified angle in the multiples of 1.86 degrees
* Example Call: servo_3();
*
*/
void servo_3(unsigned char degrees)
{
    float PositionServo = 0;
    PositionServo = ((float)degrees / 1.86) + 35.0;
    OCR1CH = 0x00;
    OCR1CL = (unsigned char) PositionServo;
}

/*
*
* Function Name: servo_1_free
* Input: void
* Output: void
* Logic: servo_free functions unlocks the servo motors from the any angle
         and make them free by giving 100% duty cycle at the PWM. This function can be used to
         reduce the power consumption of the motor if it is holding load against the gravity.
* Example Call: servo_1_free();
*
*/

void servo_1_free (void) //makes servo 1 free rotating
{
    OCR1AH = 0x03;
    OCR1AL = 0xFF; //Servo 1 off
}

/*
*
* Function Name: servo_2_free
* Input: void
* Output: void
* Logic: servo_free functions unlocks the servo motors from the any angle
         and make them free by giving 100% duty cycle at the PWM. This function can be used to
         reduce the power consumption of the motor if it is holding load against the gravity.
* Example Call: servo_2_free();
*
*/

void servo_2_free (void) //makes servo 2 free rotating
{
    OCR1BH = 0x03;
    OCR1BL = 0xFF; //Servo 2 off
}

/*
*
* Function Name: servo_3_free
* Input: void
* Output: void
* Logic: servo_free functions unlocks the servo motors from the any angle
         and make them free by giving 100% duty cycle at the PWM. This function can be used to
         reduce the power consumption of the motor if it is holding load against the gravity.
* Example Call: servo_3_free();
*
*/
void servo_3_free (void) //makes servo 3 free rotating
{
    OCR1CH = 0x03;
    OCR1CL = 0xFF; //Servo 3 off
}
/*
*
* Function Name: pickup1
* Input: void
* Output: void
* Logic: Moves arm downwards to pick block from forward
* Example Call: pickup1();
*
*/
void pickup1()
{
	servo_2(100);
	_delay_ms(500);
	servo_1(20);
}
/*
*
* Function Name: pickup1
* Input: void
* Output: void
* Logic: Moves arm downwards to pick block from back
* Example Call: pickup1back();
*
*/
void pickup1back()
{
	servo_3(110);
	_delay_ms(800);
	servo_1(180);
}
/*
*
* Function Name: pickup2
* Input: void
* Output: void
* Logic: Grabs box from forward and Moves arm  upwards
* Example Call: pickup2();
*
*/
void pickup2()
{

	servo_2(0);
	_delay_ms(2000);
	servo_1(90);

}
/*
*
* Function Name: pickup2back()
* Input: void
* Output: void
* Logic: Grabs box from back and Moves arm  upwards
* Example Call: pickup2back();
*
*/
void pickup2back()
{

	servo_3(0);
	_delay_ms(2000);
	servo_1(90);

}
/*
*
* Function Name:  place_both_low_rise
* Input: void
* Output: void
* Logic: Drops both the boxes inside the low rise house
* Example Call:  place_both_low_rise();
*
*/
void place_both_low_rise(void)
{
	servo_1(50);
	_delay_ms(500);
	servo_2(110);
	
	
	_delay_ms(500);
	back();
	_delay_ms(120);
	stop();
	servo_3(150);
	
	_delay_ms(1000);
	servo_neutral();
	_delay_ms(500);
}
/*
*
* Function Name:  place_low_rise_front
* Input: void
* Output: void
* Logic: Drops front arm box inside the low rise house
* Example Call:  place_low_rise_front();
*
*/
void place_low_rise_front(void)
{
	servo_1(50);
	_delay_ms(500);
	servo_2(110);
	_delay_ms(1000);
	servo_neutral();

	
	
}
/*
*
* Function Name:  place_low_rise_back
* Input: void
* Output: void
* Logic: Drops back arm box inside the low rise house
* Example Call:  place_low_rise_back();
*
*/
void place_low_rise_back(void)
{
	servo_1(50);
	_delay_ms(500);
	servo_3(150);
	_delay_ms(1000);
	servo_neutral();

	
}
/*
*
* Function Name:  place_high_rise_front
* Input: void
* Output: void
* Logic: Drops front arm box inside the high rise house
* Example Call:  place_high_rise_front();
*
*/
void place_high_rise_front(void)
{
	servo_1(95);
	_delay_ms(500);
	servo_2(110);
	_delay_ms(1000);
	servo_neutral();
}
/*
*
* Function Name:  place_high_rise_back
* Input: void
* Output: void
* Logic: Drops back arm box inside the high rise house
* Example Call:  place_high_rise_back();
*
*/
void place_high_rise_back(void)
{
	servo_1(95);
	_delay_ms(500);
	servo_3(110);
	_delay_ms(1000);
	servo_neutral();
}
/*
*
* Function Name: servo_neutral
* Input: void
* Output: void
* Logic: arm gets to neutral position
* Example Call: servo_neutral();
*
*/
void servo_neutral()
{
	servo_1(90);
	servo_2(0);
	servo_3(0);
}
/*
*
* Function Name: pickup_from_front
* Input: void
* Output: void
* Logic: Bot picks up block from front
* Example Call: pickup_from_front();
*
*/
void pickup_from_front()
{
	align();
	forward();
	//_delay_ms(20);
	//stop();
	//pickup_from_front();
	align();
	forward_for(3);
	back();
	_delay_ms(40);
	stop();
	pickup1();
	_delay_ms(500);
	//speed3=1;
	align();
	velocity(250,250);
	//forward_for(5);
	forward();
	_delay_ms(180);
	stop();
	speed=0;
	pickup2();
	_delay_ms(1000);
	forward();
	_delay_ms(50);
}
/*
*
* Function Name: pickup_from_back
* Input: void
* Output: void
* Logic: Bot picks up block from back
* Example Call: pickup_from_back();
*
*/
void pickup_from_back()
{
	align();
	forward_for(15);
	stop();
	turn_around();
	back();
	_delay_ms(50);
	int f=0;
	while(f!=5){
		align();
		f++;
	}
	forward_for(1);
	back();
	_delay_ms(40);
	stop();
	pickup1back();
	_delay_ms(1000);
	back();
	_delay_ms(80);
	stop();
	pickup2back();
	_delay_ms(1000);
}