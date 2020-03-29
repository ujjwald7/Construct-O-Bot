/*
*
*Team Id: <3362>
* Author List: <Nikhil,Vansh,Ujjwal,Vivek>
* Filename: <whiteline.h>
* Theme: <Construct-O-Bot -- Specific to eYRC>
* Functions: <white_enter_right(),white_enter_left(), white_leave_right(),white_leave_left(),align_white(),forward_by_node_white,
				right_turn_wls_white,black_white,left_turn_wls_white,forward_for_white>
*
*
*/

/*
*
* Function Name: white_enter_right
* Input: none
* Output: void
* Logic: bot will enter from right side and place the box in H5 .
* Example Call: white_enter_right();  
*
*/
void white_enter_right()
{
	right_turn_wls_white();
	black_white();
	forward_by_node_white(1);
	left_turn_wls_white();
	back();
	_delay_ms(40);
	stop();
	place_both_low_rise();
}
/*
*
* Function Name: white_enter_left
* Input: none
* Output: void
* Logic: bot will enter from left side and place the box in H5 .
* Example Call: white_enter_left();
*
*/
void white_enter_left()
{
	left_turn_wls_white();
	black_white();
	forward_by_node_white(1);
	left_turn_wls_white();
	back();
	_delay_ms(40);
	stop();
	place_both_low_rise();
}
/*
*
* Function Name: white_leave_right
* Input: none
* Output: void
* Logic: bot will leave from right side after placing the box in H5 .
* Example Call: white_leave_right();
*
*/
void white_exit_right()
{
	right_turn_wls_white();
	forward_by_node_white(1);
}
/*
*
* Function Name: white_leave_left
* Input: none
* Output: void
* Logic: bot will leave from left side after placing the box in H5 .
* Example Call: white_leave_left();
*
*/
void white_exit_left()
{
	left_turn_wls_white();
	forward_by_node_white(1);
}
/*
*
* Function Name: align_white
* Input: none
* Output: void
* Logic: aligns the middle sensor of the bot .
* Example Call: align_white();  //middle sensor at white line
*
*/
void align_white()
{
	if(ADC_Conversion(1)<threshold)
	{
		while(ADC_Conversion(2)>threshold)
		{
			soft_left();
			//_delay_ms(50);
		}
	}
	if(ADC_Conversion(3)<threshold)
	{
		while(ADC_Conversion(2)>threshold)
		{
			soft_right();
			//_delay_ms(50);
		}
	}
	return;
}
/*
*
* Function Name: white_line_follower
* Input: void
* Output: void
* Logic: Helps bot to stay on the white line
* Example Call:  white_line_follower();
*
*/
void white_line_follower()
{
	white_calculate_error();
	white_drive_motors();
}

/*
*
* Function Name: white_calculate_error
* Input: void
* Output: void
* Logic: set value of variable error
* Example Call:  white_calculate_error();
*
*/

void white_calculate_error()
{
	if(ADC_Conversion(2)<threshold)
	{
		error=0;
		//forward();
		//velocity(150,150);
	}

	if(ADC_Conversion(3)<threshold)
	{
		error=-1;



		//soft_left_turn();
		//velocity(130,50);
	}

	if(ADC_Conversion(1)<threshold)
	{
		error=1;
		//soft_right_turn();        //velocity(50,130);
	}

}
/*
*
* Function Name: forward_by_node_white()
* Input: node
* Output: void
* Logic: Uses white line sensors to go forward by the number of nodes specified
* Example Call: forward_by_node_white(2); //Goes forward by two nodes
*
*/
void forward_by_node_white(int node)
{
	int on_the_node=0;
	forward();
	_delay_ms(20);
	while(on_the_node < node)
	{
		white_line_follower();
		if (ADC_Conversion(2)>threshold && ADC_Conversion(1)<threshold && ADC_Conversion(3)<threshold)
		{
			forward_by_node(1);
			break;

		}
		if (ADC_Conversion(2)<threshold && (ADC_Conversion(1)<threshold || ADC_Conversion(3)<threshold && ADC_Conversion(2)<threshold))
		{
			forward();
			_delay_ms(10);
			on_the_node++;
		}


	}
	forward();
	_delay_ms(250);
}
/*
*
* Function Name: right_turn_wls_white()
* Input: void
* Output: void
* Logic: Uses white line sensors to turn right until white line is encountered
* Example Call: right_turn_wls_white(); //Turns right until white line is encountered
*/
void right_turn_wls_white(void)
{
	left();                 
	_delay_ms(50);                   //small initial rotation to initialise sensors
	while (ADC_Conversion(2) < threshold) //bot will rotate  left till centre sensor is on white line
	{
		left();
	}
	//_delay_ms(50);                //small delay to re-initialise sensors at the new position
	while (ADC_Conversion(1) > threshold) //bot will rotate left till left most sensor is on black
	{
		left();
	}
	//  soft_left();          //soft left for a short period to move bot out of the node
	//_delay_ms(100);



}
/*
*
* Function Name: left_turn_wls_white()
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until white line is encountered
* Example Call: left_turn_wls_white(); //Turns right until white line is encountered
*
*/
void left_turn_wls_white(void)
{

	right();
	_delay_ms(50);                //small initial rotation to initialise sensors
	while (ADC_Conversion(2) < threshold)      //bot will rotate  right till centre sensor is on white line
	{
		right();
	}
	//_delay_ms(50);                        //small delay to re-initialise sensors at the new position
	while (ADC_Conversion(3) > threshold)     //bot will rotate right till right most sensor is on black
	{
		right();
	}
	soft_right();                     //soft right for a short period to move bot out of the node
	_delay_ms(100);

}

/*
*
* Function Name: white_drive_motor
* Input: void
* Output: void
* Logic: motor response according to error
* Example Call:  white_drive_motors();
*
*/

void white_drive_motors()
{
	if (error == 0)
	{
		forward();
	}
	else if (error == 1)
	{
		velocity(150,250);
		left();

		//soft_left();
	}

	else if (error == -1)
	{
		velocity(250,150);
		right();
		//soft_right();
	}else if(error==2)
	{
		stop();
	}
}

/*
*
* Function Name: forward_for_white
* Input: int
* Output: void
* Logic: Uses white line sensors to go forward for a given factor n
* Example Call: forward_for_white(10); //Goes forward while count<10
*
*/
void forward_for_white(int count)
{
	forward();
	int n= count;
	_delay_ms(30);
	while (n!=0)
	{
		white_line_follower();
		_delay_ms(30);
	}
	stop();

}
/*
*
* Function Name: black_white()
* Input: int
* Output: void
* Logic: Uses white line sensors tranfer between black to white line
* Example Call: black_white() // runs on black line and stops as soon as white line is encountered
*
*/
void black_white(void)
{
	while(1){
		print_sensor(1,1,1);
		print_sensor(1,5,2);
		print_sensor(1,9,3);


		if (ADC_Conversion(1)>threshold && ADC_Conversion(2)<threshold && ADC_Conversion(3)>threshold)
		{
			stop();

			return;

		}else if (ADC_Conversion(1)>threshold && ADC_Conversion(2)>threshold && ADC_Conversion(3)<threshold)
		{
			stop();

			return;

		}else if (ADC_Conversion(1)<threshold && ADC_Conversion(2)>threshold && ADC_Conversion(3)>threshold)
		{
			stop();

			return;

			}else{
			forward();
			align();
		}
	}


	stop();
}

