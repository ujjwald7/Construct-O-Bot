/*
*
*Team Id: <3362>
* Author List: <Nikhil,Vansh,Ujjwal,Vivek>
* Filename: <lcd.h>
* Theme: <Construct-O-Bot -- Specific to eYRC>
* Functions: <lcd_reset,lcd_init,lcd_wr_command,lcd_wr_char,lcd_print,lcd_home>
* Global Variables: <>
*
*/
#define RS 0
#define RW 1
#define EN 2
#define lcd_port PORTC

#define sbit(reg,bit)	reg |= (1<<bit)			// Macro defined for Setting a bit of any register.
#define cbit(reg,bit)	reg &= ~(1<<bit)		// Macro defined for Clearing a bit of any register.

void lcd_reset();
void lcd_init();
void lcd_wr_command(unsigned char);
void lcd_wr_char(char);
void lcd_line1();
void lcd_line2();
void lcd_string(char*);

unsigned int temp;
unsigned int unit;
unsigned int tens;
unsigned int hundred;
unsigned int thousand;
unsigned int million;

/*
*
* Function Name: lcd_port_config
* Input: node
* Output: void
* Logic: Function to configure LCD port
* Example Call: lcd_port_config();
*
*/
void lcd_port_config (void)
{
 DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 PORTC = PORTC & 0x08; // all the LCD pins are set to logic 0 except PORTC 7
}

/*
*
* Function Name: lcd_set_4bit
* Input: node
* Output: void
* Logic: Function to Reset LCD
* Example Call: lcd_set_4bit();
*
*/

void lcd_set_4bit()
{
	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x30;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x30;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x30;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x20;				//Sending 2 to initialise LCD 4-bit mode
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin


}

/*
*
* Function Name: lcd_init
* Input: node
* Output: void
* Logic: Function to Initialize LCD
* Example Call: lcd_init();
*
*/

void lcd_init()
{
	lcd_set_4bit();
	_delay_ms(1);

	lcd_wr_command(0x28);			//LCD 4-bit mode and 2 lines.
	lcd_wr_command(0x01);
	lcd_wr_command(0x06);
	lcd_wr_command(0x0E);
	lcd_wr_command(0x80);

}

/*
*
* Function Name: lcd_wr_command
* Input: unsigned char
* Output: void
* Logic: Function to Write Command on LCD
* Example Call: lcd_wr_command(left);
*
*/

void lcd_wr_command(unsigned char cmd)
{
	unsigned char temp;
	temp = cmd;
	temp = temp & 0xF0;
	lcd_port &= 0x0F;
	lcd_port |= temp;
	cbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);

	cmd = cmd & 0x0F;
	cmd = cmd<<4;
	lcd_port &= 0x0F;
	lcd_port |= cmd;
	cbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);
}

/*
*
* Function Name: lcd_wr_char
* Input: char
* Output: void
* Logic: Function to Write Data on LCD
* Example Call: lcd_wr_char('A');
*
*/

void lcd_wr_char(char letter)
{
	char temp;
	temp = letter;
	temp = (temp & 0xF0);
	lcd_port &= 0x0F;
	lcd_port |= temp;
	sbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);

	letter = letter & 0x0F;
	letter = letter<<4;
	lcd_port &= 0x0F;
	lcd_port |= letter;
	sbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);
}

/*
*
* Function Name: lcd_home
* Input: void
* Output: void
* Logic: Function to bring cursor at home position
* Example Call: lcd_home();
*
*/

void lcd_home()
{
	lcd_wr_command(0x80);
}

/*
*
* Function Name: lcd_string
* Input: char
* Output: void
* Logic: Function to Print String on LCD
* Example Call: lcd_string('AbC');
*
*/

void lcd_string(char *str)
{
	while(*str != '\0')
	{
		lcd_wr_char(*str);
		str++;
	}
}

/*
*
* Function Name: lcd_cursor
* Input: char,char
* Output: void
* Logic: Position the LCD cursor at "row", "column".
* Example Call: lcd_cursor(1,3);
*
*/


void lcd_cursor (char row, char column)
{
	switch (row) {
		case 1: lcd_wr_command (0x80 + column - 1); break;
		case 2: lcd_wr_command (0xc0 + column - 1); break;
		case 3: lcd_wr_command (0x94 + column - 1); break;
		case 4: lcd_wr_command (0xd4 + column - 1); break;
		default: break;
	}
}
/*
*
* Function Name: lcd_print
* Input: char,char,unsignes int, int
* Output: void
* Logic: Function To Print Any input value upto the desired digit on LCD.
* Example Call: lcd_print(1,1,10,2);
*
*/
void lcd_print (char row, char coloumn, unsigned int value, int digits)
{
	unsigned char flag=0;
	if(row==0||coloumn==0)
	{
		lcd_home();
	}
	else
	{
		lcd_cursor(row,coloumn);
	}
	if(digits==5 || flag==1)
	{
		million=value/10000+48;
		lcd_wr_char(million);
		flag=1;
	}
	if(digits==4 || flag==1)
	{
		temp = value/1000;
		thousand = temp%10 + 48;
		lcd_wr_char(thousand);
		flag=1;
	}
	if(digits==3 || flag==1)
	{
		temp = value/100;
		hundred = temp%10 + 48;
		lcd_wr_char(hundred);
		flag=1;
	}
	if(digits==2 || flag==1)
	{
		temp = value/10;
		tens = temp%10 + 48;
		lcd_wr_char(tens);
		flag=1;
	}
	if(digits==1 || flag==1)
	{
		unit = value%10 + 48;
		lcd_wr_char(unit);
	}
	if(digits>5)
	{
		lcd_wr_char('E');
	}

}



