/*
*
*Team Id: <3362>
* Author List: <Nikhil,Vansh,Ujjwal,Vivek>
* Filename: <adc.h>
* Theme: <Construct-O-Bot -- Specific to eYRC>
* Functions: <ADC_Conversion,Sharp_GP2D12_estimation,print_sensor>
* Global Variables: <ADC_Value>
*
*/
unsigned char ADC_Value;



/*
*
* Function Name: adc_pin_config
* Input: void
* Output: void
* Logic: ADC pin configuration.
* Example Call: adc_pin_config();
*
*/
void adc_pin_config (void)
{
	DDRF = 0x00; //set PORTF direction as input
	PORTF = 0x00; //set PORTF pins floating
	DDRK = 0x00; //set PORTK direction as input
	PORTK = 0x00; //set PORTK pins floating
}
/*
*
* Function Name: adc_init
* Input: void
* Output: void
* Logic:Function to Initialize ADC.
* Example Call: adc_init();
*
*/
void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;      //MUX5 = 0
	ADMUX = 0x20;       //Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;      //ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

/*
*
* Function Name:  ADC_Conversion
* Input: unsigned char
* Output: void
* Logic:This Function accepts the Channel Number and returns the corresponding Analog Value.
* Example Call: ADC_Conversion(1); //Gives ADC value of left_sensor
*
*/

unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;     //Set start conversion bit
	while((ADCSRA&0x10)==0);    //Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}

/*
*
* Function Name:  Sharp_GP2D12_estimation
* Input: unsigned char
* Output: void
* Logic: This Function calculates the actual distance in millimeters(mm) from the input 
         analog value of Sharp Sensor.
* Example Call: Sharp_GP2D12_estimation(ADC_Conversion(9)); //Gives distance in mm from left sharp sensor
*
*/


unsigned int Sharp_GP2D12_estimation(unsigned char adc_reading)
{
	float distance;
	unsigned int distanceInt;
	distance = (int)(10.00*(2799.6*(1.00/(pow(adc_reading,1.1546)))));
	distanceInt = (int)distance;
	if(distanceInt>800)
	{
		distanceInt=800;
	}
	return distanceInt;
}
/*
*
* Function Name:   print_sensor
* Input: char , char, unsigned char
* Output: void
* Logic: Print ADC_Value on lcd.
* Example Call: print_sensor(1,1,1);  //print adc value of left_sensor on 1st row and 1st coloumn
*
*/
void print_sensor(char row, char coloumn,unsigned char channel)
{
	ADC_Value = ADC_Conversion(channel);
	lcd_print(row, coloumn, ADC_Value, 3);
}

