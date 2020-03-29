/*
*
*Team Id: <3362>
* Author List: <Nikhil,Vansh,Ujjwal,Vivek>
* Filename: <lfr.h>
* Theme: <Construct-O-Bot -- Specific to eYRC>
* Functions: <forward_wls,left_turn_wls,right_turn_wls,zigzag,Wall,forward_for,turn_around,forward,left,right,back ,soft_left,sofr_right>
* Global Variables: <threshold, wallZag,speed,zig,last_error,error>
*
*/

int threshold = 120;               //threshold to detect black line
int error=0;                       //detects the position of bot
int wallZag = 0;                   // 1 if bot is between wall
int speed = 0;                     // used to inc or dec speed of bot
int zig =0;                        // 1 if bot is following zigzag line
int last_error=0;


void forward_for(int);
void right_turn_wls(void);
void left_turn_wls(void);
void zigzag(void);
void Wall(void);




/*
*
* Function Name: motion_pin_config
* Input: void
* Output: void
* Logic: Motion pin configuration.
* Example Call: motion_pin_config();
*
*/

void motion_pin_config (void)
{
 DDRA = DDRA | 0x0F; //set direction of the PORTA 3 to PORTA 0 pins as output
 PORTA = PORTA & 0xF0; // set initial value of the PORTA 3 to PORTA 0 pins to logic 0
 DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
 PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM
}

/*
*
* Function Name: motion_set
* Input: void
* Output: void
* Logic: Rotates Motor in a given direction.
* Example Call: motion_set();
*
*/
void motion_set (unsigned char Direction)
{
 unsigned char PortARestore = 0;

 Direction &= 0x0F;             // removing upper nibbel as it is not needed
 PortARestore = PORTA;          // reading the PORTA's original status
 PortARestore &= 0xF0;          // setting lower direction nibbel to 0
 PortARestore |= Direction;     // adding lower nibbel for direction command and restoring the PORTA status
 PORTA = PortARestore;          // setting the command to the port
}

/*
*
* Function Name: timer5_init
* Input: none
* Output: void
* Logic: uses timer to rotate motor
* Example Call: timer5_init();
*
*/

void timer5_init()
{
    TCCR5B = 0x00;  //Stop
    TCNT5H = 0xFF;  //Counter higher 8-bit value to which OCR5xH value is compared with
    TCNT5L = 0x01;  //Counter lower 8-bit value to which OCR5xH value is compared with
    OCR5AH = 0x00;  //Output compare register high value for Left Motor
    OCR5AL = 0xFF;  //Output compare register low value for Left Motor
    OCR5BH = 0x00;  //Output compare register high value for Right Motor
    OCR5BL = 0xFF;  //Output compare register low value for Right Motor
    OCR5CH = 0x00;  //Output compare register high value for Motor C1
    OCR5CL = 0xFF;  //Output compare register low value for Motor C1
    TCCR5A = 0xA9;  /*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
                      For Overriding normal port functionality to OCRnA outputs.
                      {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/

    TCCR5B = 0x0B;  //WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

/*
*
* Function Name: velocity
* Input: unsigned char, unsigned char
* Output: void
* Logic: sets speed of wheels
* Example Call: velocity();
*
*/
void velocity (unsigned char left_motor, unsigned char right_motor)
{
    OCR5AL = (unsigned char)left_motor;
    OCR5BL = (unsigned char)right_motor;
}
/*
*
* Function Name: forward
* Input: void
* Output: void
* Logic: both wheels forward
* Example Call: forward();
*
*/

void forward (void) 
{
    if (speed==0)
    {
         velocity(250,250);
    }else{
        velocity(180,180);
    }

  /*if(wallZag>0)
  {
      velocity(100,100);

  }*/
  //velocity(100,100);
  motion_set(0x06);
}

/*
*
* Function Name: back
* Input: void
* Output: void
* Logic: both wheels backward 
* Example Call: back();
*
*/
void back (void) 
{
    velocity(100,100);
  motion_set(0x09);
}
/*
*
* Function Name: left
* Input: void
* Output: void
* Logic: Left wheel backward, Right wheel forward
* Example Call: left();
*
*/
void left (void) 
{
  //velocity(0,200);
  if (speed!=0)
  {
      //velocity(0,155);
  }
  motion_set(0x05);

}

/*
*
* Function Name: right
* Input: void
* Output: void
* Logic: Left wheel forward, Right wheel backward
* Example Call: right();
*
*/
void right (void) 
{
  //velocity(0,200);
  if (speed!=0)
  {
     // velocity(155,0);
  }
  motion_set(0x0A);
}

/*
*
* Function Name: soft_left
* Input: void
* Output: void
* Logic: Left wheel stationary, Right wheel forward
* Example Call:  soft_left();
*
*/
void soft_left (void) 
{
 //velocity(200,100);
 velocity(0,250);
 motion_set(0x04);
}

/*
*
* Function Name: soft_right
* Input: void
* Output: void
* Logic: Left wheel forward, Right wheel is stationary
* Example Call:  soft_right();
*
*/
void soft_right (void) 
{
 //velocity(200,120);
 velocity(250,0);
 motion_set(0x02);
}

/*
*
* Function Name: stop
* Input: void
* Output: void
* Logic: hard stop
* Example Call:  stop();
*
*/
void stop (void) 
{
  velocity(0,0);
  motion_set(0x00);
}
/*
*
* Function Name: line_follower
* Input: void
* Output: void
* Logic: Helps bot to stay on the black line
* Example Call:  line_follower();
*
*/
void line_follower()
{
    calculate_error();
    drive_motors();
}
/*
*
* Function Name: calculate_error
* Input: void
* Output: void
* Logic: set value of variable error
* Example Call:  calculate_error();
*
*/
void calculate_error()
{
    print_sensor(1,1,1);
    print_sensor(1,5,2);
    print_sensor(1,9,3);
    if(ADC_Conversion(2)>threshold)
    {
        error=0;
        //forward();
        //velocity(150,150);
    }

    if(ADC_Conversion(3)>threshold)
    {
        error=-1;
        last_error=1;
        //soft_left_turn();
        //velocity(130,50);
    }

    if(ADC_Conversion(1)>threshold)
    {
        error=1;
        last_error=-1;
        //soft_right_turn();
        //velocity(50,130);
    }

    if((ADC_Conversion(3) < threshold) && (ADC_Conversion(2) < threshold) && (ADC_Conversion(1) < threshold))
    {
        error=error;
       if(wallZag>0 && Sharp_GP2D12_estimation(ADC_Conversion(9))<200 &&Sharp_GP2D12_estimation(ADC_Conversion(9))<200)
       {
	       proximity();
       }
       
       if (zig>0 && last_error!=0)
       {
	       if(last_error==-1)
	       {
		       error=1;
	       }
	       else
	       {
		       error=-1;
	       }
       }
    }

    if((ADC_Conversion(3) > threshold) && (ADC_Conversion(2) > threshold) && (ADC_Conversion(1) > threshold))
    {
        error=0;
    }
}
/*
*
* Function Name: drive_motor
* Input: void
* Output: void
* Logic: motor response according to error
* Example Call:  drive_motors();
*
*/
void drive_motors()
{
    if (error == 0)
    {
        forward();
    }
    else if (error == -1)
    {
        left();
    }
    else if (error == 1)
    {
        right();
    }
   
}
/*
*
* Function Name: right_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn right until black line is encountered
* Example Call: right_turn_wls(); //Turns right until black line is encountered
*/


void right_turn_wls(void)
{
    left();                 //small initial rotation to initialise sensors
    while (ADC_Conversion(2) == threshold) //bot will rotate  left till centre sensor is on black line
    {
        left();
    }
    //_delay_ms(50);                //small delay to re-initialise sensors at the new position
    while (ADC_Conversion(3) != threshold) //bot will rotate left till left most sensor is on white
    {
        left();
    }
    soft_left();          //soft left for a short period to move bot out of the node
    _delay_ms(100);

}

/*
*
* Function Name: left_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered
* Example Call: left_turn_wls(); //Turns right until black line is encountered
*
*/
void left_turn_wls(void)
{

    right();                //small initial rotation to initialise sensors
    while (ADC_Conversion(2) == threshold)      //bot will rotate  right till centre sensor is on black line
    {
        right();
    }
    //_delay_ms(50);                        //small delay to re-initialise sensors at the new position
    while (ADC_Conversion(1) != threshold)     //bot will rotate right till right most sensor is on white
    {
        right();
    }
    soft_right();                     //soft right for a short period to move bot out of the node
    _delay_ms(100);

}
/*
*
* Function Name: forward_by_node
* Input: node
* Output: void
* Logic: Uses white line sensors to go forward by the number of nodes specified
* Example Call: forward_by_node(2); //Goes forward by two nodes
*
*/
void forward_by_node(unsigned char node)
{
    int on_the_node=0;
    forward();
    _delay_ms(20);
    while(on_the_node != node)
    {
        line_follower();
        //distance_sharp=sharp_sensor_distance();
        /*if (distance_sharp < 80 )
        {
            stop();
            sound_on();
            break;
        }*/
        if (ADC_Conversion(3)> threshold && ADC_Conversion(2)>threshold && ADC_Conversion(1)>threshold)  //node detected
        {
            while(ADC_Conversion(3)> threshold && ADC_Conversion(2)>threshold && ADC_Conversion(1)>threshold)
            {
                forward();
            }
            _delay_ms(150);
            on_the_node++;

        }
    }
    forward();
    _delay_ms(10); //axis

}

/*
*
* Function Name: proximity
* Input: node
* Output: void
* Logic: Uses sharp sensors to pass between two parallel walls
* Example Call: proximity; //Passes through two parallel walls
*
*/
void proximity()
{
	forward();
	_delay_ms(1000);
	//speed1=1;
	velocity(100,100);

	while (ADC_Conversion(1)<threshold && ADC_Conversion(2)<threshold && ADC_Conversion(3)<threshold)
	{

		int ld = Sharp_GP2D12_estimation(ADC_Conversion(9));
		int rd = Sharp_GP2D12_estimation(ADC_Conversion(10));
		lcd_print(1,1,rd,3);
		lcd_print(1,9,ld,3);

		if (rd>=105 && ld<=95)
		{

			soft_left();
			}else if(ld>=110 && rd<=95){

			soft_right();
			}else{
			forward();
			_delay_ms(20);
		}
		velocity(230,230);
		//speed1=0;

	}

	//speed=0;
	wallZag=0;
}
/*
*
* Function Name: wall
* Input: none
* Output: void
* Logic: Uses sharp  and white lines sensors  to go between two nodes which have wall in between them
* Example Call: wall;  //follows line and Passes through two parallel walls between two nodes
*
*/
void wall()
{
	wallZag++;
	forward_by_node(1);
}
/*
*
* Function Name: turn_around
* Input: none
* Output: void
* Logic: Changes the direction on movement to opposite side.
* Example Call: turn_around;  //turn 180 degree
*
*/
void turn_around(void){
	forward();
	_delay_ms(10);
	right();
	_delay_ms(100);
	int cnt=0;
	while (ADC_Conversion(2)>threshold)
	{
		right();
	}

	while(ADC_Conversion(2)<threshold || cnt<30)
	{
		right();
		cnt++;
	}
}
/*
*
* Function Name: align
* Input: none
* Output: void
* Logic: aligns the middle sensor of the bot .
* Example Call: align();  //middle sensor at black line
*
*/
void align()
{
	speed=1;
	if(ADC_Conversion(1)>=threshold&&ADC_Conversion(2)<threshold)
	{
		while(ADC_Conversion(2)<threshold)
		{
			soft_left();
		}
	}
	else if(ADC_Conversion(3)>=threshold&&ADC_Conversion(2)<threshold)
	{
		while(ADC_Conversion(2)<threshold)
		{
			soft_right();
		}
	}
	else
	{
		forward();
	}
	speed=0;
	return;
}
/*
*
* Function Name: forward_for
* Input: int
* Output: void
* Logic: Uses white line sensors to go forward for a given factor n
* Example Call: forward_for(10); //Goes forward while count<10
*
*/
void forward_for(int n)
{
    velocity(150,150);
    forward();
    _delay_ms(30);
    while (n!=0)
    {
        line_follower();
        _delay_ms(30);
        n--;
    }
    stop();
    velocity(255,255);



}
/*
*
* Function Name: zigzag
* Input: void
* Output: void
* Logic: goes forward through a zig zag line
* Example Call: zigzag(); 
*
*/
void zigzag()
{
    //speed++;
   // forward_for(10);
    zig++;
    forward_by_node(1);
    zig--;
    //speed--;

}
