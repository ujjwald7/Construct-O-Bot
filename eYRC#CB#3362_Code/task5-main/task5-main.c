/*
*
*Team Id: <3362>
* Filename: <task5-main.c>
* Theme: <Construct-O-Bot -- Specific to eYRC>
* Functions: <path,init_device,port_init>
* Global Variables: <>
*
*/

#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "adc.h"
#include "lfr.h"
#include "buzzer.h"
#include "servo.h"
#include "whiteline.h"
#include<stdio.h>
#include<math.h>
#define IN 99			//Initialising The initial distance between all edges as 99 before using Djikstra's Algorithm
#define N 34			//The Number Of Nodes In the Graph 
								//Refer To the "Node_map" image attached along for more node numbering details
#define number 10		//The number of blocks to be picked up and placed

int pickup[10] = {27,28,25,29,26,20,30,22,21,24};			//Pickup_Arraya
int drop_off[10] = {18,33,18,33,31,32,34,31,32,34};			//DropOff Array

int front_arm=1,back_arm=1;		//In Our Bidirectional Arm Pickup mechanism
					//front_arm = 1 , implies that it is free to use 
					//front_arm = 0, implies that it is holding a block

//house configurations 1st column is "1" if it is a low rise house else 0
//similarly, 2nd column is "1" if it is a high rise house else 0


int house_config[5][2] ={{31,1},      //HOUSE H3
						{32,0},      //HOUSE H1
						{34,1},      //HOUSE H2
						{33,0},      //HOUSE H4
						{18,0} };    //HOUSE H5

int path[1000];		//Array To store path
int path_size=0;	//to keep a count of the path size


/*
*
* Function Name: port_init
* Input: node
* Output: void
* Logic: initalizes ports
* Example Call: port_init();
*
*/

void port_init()
{
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();
	
	//buzzer_pin_config();
	
}

/*
*
* Function Name: init_devices
* Input: node
* Output: void
* Logic: initalizes devices
* Example Call: init_devices();
*
*/

void init_devices (void)
{
	cli(); //Clears the global interrupts
	port_init();
	adc_init();
	timer5_init();
	timer1_init();
	servo_port_init();
	buzzer_port_init();

	//buzzer_port_init();
	sei(); //Enables the global interrupts
}

/*
*
* Function Name: dijsktra
* Input: int cost[][N+1],int source,int target
* Output: int distance
* Logic: returns the shortest distance length from the current position to the next position
* Example Call: dijsktra(cost,0,10);   //returns shortest distance from 0 to 10th node
*
*/

int dijsktra(int cost[][N+1],int source,int target)
{
	int dist[N+1],prev[N+1],selected[N+1]={0},i,m,min,start,d,j;
	for(i=1;i< N+1;i++)						//initialise all edge distances as MAX
	{
		dist[i] = IN;			
		prev[i] = -1;
	}		
	start = source;
	selected[start]=1;
	dist[start] = 0;
	while(selected[target] ==0)
	{
		min = IN;
		m = 0;
		for(i=1;i< N+1;i++)
		{
			d = dist[start] +cost[start][i];
			if(d< dist[i]&&selected[i]==0)			
			{
				dist[i] = d;
				prev[i] = start;
			}
			if(min>dist[i] && selected[i]==0)		
			{
				min = dist[i];
				m = i;
			}
		}
		start = m;
		selected[start] = 1;
	}
	return dist[target];
}

/*
*
* Function Name: path_travel
* Input: int cost[][N+1],int source,int target
* Output: void
* Logic: stores the path size
* Example Call: path(cost,0,10);   //stores shortest path from 0 to 10th node
*
*/


void path_travel(int cost[][N+1], int source,int target)
{
	int dist[N+1],prev[N+1],selected[N+1]={0},i,m,min,start,d,j;
	for(i=1;i< N+1;i++)
	{
		dist[i] = IN;
		prev[i] = -1;
	}
	start = source;
	selected[start]=1;
	dist[start] = 0;
	while(selected[target] ==0)
	{
		min = IN;
		m = 0;
		for(i=1;i< N+1;i++)
		{
			d = dist[start] +cost[start][i];
			if(d< dist[i]&&selected[i]==0)
			{
				dist[i] = d;
				prev[i] = start;
			}
			if(min>dist[i] && selected[i]==0)
			{
				min = dist[i];
				m = i;
			}
		}
		start = m;
		selected[start] = 1;
	}
	start = target;
	while(start != -1)				//storing the path and incrementing patth_size
	{
		path[path_size++] = start;
		start = prev[start];
	}
	for(int i=path_size-1;i>=0;i--)
	{
		printf("%d ",path[i]);
	}
	path_traveller(path,path_size-1);
	path_size=0;
}

/*
*
* Function Name: path_traveller
* Input: int path[],int index
* Output: path index where the BOT has to travel next
* Logic: returns the next index where the BOT has to travel using recursive calls
* Example Call: path_traveller(path,path_size);   //where path_size gives the current node the BOT has to travel to
*
*/


//contains all the travel commands as returned by path_travel

int path_traveller(int path[],int index)
{
	if(index==-1){
		return;
	}
	if(path[index]==2)
	{
		if(path[index+1]==1){
			forward_by_node(1);
		}
		else if(path[index+1]==4){
			left_turn_wls();
			forward_by_node(1);
		}
		else if(path[index+1]==3){
			right_turn_wls();
			forward_by_node(1);
		}
	}
	if(path[index]==3)
	{
		if(path[index+1]==2){
			left_turn_wls();
			forward_by_node(1);
		}
		else{
			forward_by_node(1);
		}
	}
	if(path[index]==4)
	{
		if(path[index+1]==2){
			right_turn_wls();
			forward_by_node(1);
		}
		else{
			forward_by_node(1);
		}
	}
	if(path[index]==6||path[index]==5||path[index]==10||path[index]==9||path[index]==14||path[index]==13)
	{
		forward_by_node(1);
	}
	if(path[index]==28||path[index]==32||path[index]==24||path[index]==31||path[index]==19||path[index]==30||path[index]==26||path[index]==21)
	{
		if(path[index+2]==8||path[index+2]==10||path[index+2]==12||path[index+2]==14||path[index+2]==15||path[index+2]==11||path[index+2]==16||path[index+2]==7)
		{
			right_turn_wls();
			if(path[index]==32||path[index]==31)
			{
				if(house_config[path[index]][0]==1){
					if(front_arm==0){
						place_high_rise_front();
						front_arm=1;
					}
					else{
						place_high_rise_back();
						back_arm=1;
					}
				}
				else if(house_config[path[index]][0]==0)
				{
					if(front_arm==1){
						place_low_rise_front();
						front_arm=1;
					}
					else{
						place_low_rise_back();
						back_arm=0;
					}
				}
			}
			else if(front_arm==1){
				pickup_from_front();
				front_arm=0;
			}
			else{
				pickup_from_back();
				back_arm=0;
			}
		}
		if(path[index+2]==4||path[index+2]==6||path[index+2]==8||path[index+2]==10||path[index+2]==12||path[index+2]==3||path[index+2]==7||path[index+2]==11)
		{
			left_turn_wls();
			if(path[index]==32||path[index]==31)
			{
				if(house_config[path[index]][0]==1){
					if(front_arm==0){
						place_high_rise_front();
						front_arm=1;
					}
					else{
						place_high_rise_back();
						back_arm=1;
					}
				}
				else if(house_config[path[index]][0]==0)
				{
					if(front_arm==1){
						place_low_rise_front();
						front_arm=0;
					}
					else{
						place_low_rise_back();
						back_arm=0;
					}
				}
			}
			else if(front_arm==1){
				pickup_from_front();
				front_arm=0;
			}
			else{
				pickup_from_back();
				back_arm=0;
			}
		}
	}
	if(path[index]==27||path[index]==23||path[index]==20||path[index]==29||path[index]==34||path[index]==25||path[index]==33||path[index]==22)
	{
		if(path[index+2]==8||path[index+2]==12||path[index+2]==15||path[index+2]==16||path[index+2]==13||path[index+2]==11||path[index+2]==9||path[index+2]==7)
		{
			left_turn_wls();
			if(path[index]==33||path[index]==34)
			{
				if(house_config[path[index]][0]==1){
					if(front_arm==0){
						place_high_rise_front();
						front_arm=1;
					}
					else{
						place_high_rise_back();
						back_arm=1;
					}
				}
				else if(house_config[path[index]][0]==0)
				{
					if(front_arm==1){
						place_low_rise_front();
						front_arm=0;
					}
					else{
						place_low_rise_back();
						back_arm=0;
					}
				}
			}
			else if(front_arm==1){
				pickup_from_front();
				front_arm=0;
			}
			else{
				pickup_from_back();
				back_arm=0;
			}
		}
		if(path[index+2]==4||path[index+2]==12||path[index+2]==8||path[index+2]==3||path[index+2]==5||path[index+2]==7||path[index+2]==9||path[index+2]==11)
		{
			right_turn_wls();
			if(path[index]==33||path[index]==34)
			{
				if(house_config[path[index]][0]==1){
					if(front_arm==0){
						place_high_rise_front();
						front_arm=1;
					}
					else{
						place_high_rise_back();
						back_arm=1;
					}
				}
				else if(house_config[path[index]][0]==0)
				{
					if(front_arm==1){
						place_low_rise_front();
						front_arm=0;
					}
					else{
						place_low_rise_back();
						back_arm=0;
					}
				}
			}
			else if(front_arm==1){
				pickup_from_front();
				front_arm=0;
			}
			else{
				pickup_from_back();
				back_arm=0;
			}
		}
	}
	if(path[index]==8||path[index==7])
	{
		if((path[index]==8&&path[index+1]==7)||(path[index]==7&&path[index+1]==8)){
			wall();
		}
		else
		forward_by_node(1);
	}
	if(path[index]==11||path[index]==12)
	{
		if((path[index]==12&&path[index+1]==11)||(path[index]==11&&path[index+1]==12)){
			zigzag();
		}
		else
		forward_by_node(1);
	}
	if(path[index]==15||path[index]==16)
	{
		if(path[index-2]==18)
		{
			if(path[index]==15){
				white_enter_right();
			}
			else
			white_enter_left();
			index-=2;
		}
		else
		forward_by_node(1);
	}
	if(path[index]==18)
	{
		if(path[index-2]==15)
		{
			white_exit_left();
		}
		else{
			white_exit_right();
		}
		index-=2;
	}
	return path_traveller(path,index-1);			//RECURSIVE CALL
}

/*
*
*Function Name : initialise_graph
* Input : void
* Output : void
* Logic : stores nodes of graph,SORT'S THE PICKUP AND DROP_OFF ARRAY using Dijkstra's Algorithm to traverse the bot in minimum time
* Example Call : initialise_graph(); //stores nodes of graph and uses Djikstra's Algorithm to traverse the bot in minimum time
*
*/

void initialise_graph()
{
	int cost[N+1][N+1],i,j,w,ch,co;
	int src, dst;
	for(i=1;i< N+1;i++)			//initialising all initial node-to-node distances as infinity
	for(j=1;j< N+1;j++)
	cost[i][j] = IN;
	int source = 1, dest = 1;		//initialising the source and destination
	
	//Initialising the adjacency matrix;
	cost[1][2]=cost[2][1]=1;
	cost[2][3]=cost[3][2]=1;
	cost[2][4]=cost[4][2]=1;
	cost[4][6]=cost[6][4]=1;
	cost[6][28]=cost[28][6]=1;
	cost[6][27]=cost[27][6]=1;
	cost[6][8]=cost[8][6]=1;
	cost[8][32]=cost[32][8]=1;
	cost[8][7]=cost[7][8]=1;
	cost[8][10]=cost[10][8]=1;
	cost[10][24]=cost[24][10]=1;
	cost[10][23]=cost[23][10]=1;
	cost[10][12]=cost[12][10]=1;
	cost[12][31]=cost[31][12]=1;
	cost[12][11]=cost[11][12]=1;
	cost[12][14]=cost[14][12]=1;
	cost[14][19]=cost[19][14]=1;
	cost[14][20]=cost[20][14]=1;
	cost[14][15]=cost[15][14]=1;
	cost[15][17]=cost[17][15]=1;
	cost[17][18]=cost[18][17]=1;
	cost[17][16]=cost[16][17]=1;
	cost[16][13]=cost[13][16]=1;
	cost[13][21]=cost[21][13]=1;
	cost[13][22]=cost[22][13]=1;
	cost[13][11]=cost[11][13]=1;
	cost[11][33]=cost[33][11]=1;
	cost[11][9]=cost[9][11]=1;
	cost[9][26]=cost[26][9]=1;
	cost[9][25]=cost[25][9]=1;
	cost[9][7]=cost[7][9]=1;
	cost[7][34]=cost[34][7]=1;
	cost[7][5]=cost[5][7]=1;
	cost[5][29]=cost[29][5]=1;
	cost[5][30]=cost[30][5]=1;
	cost[5][3]=cost[3][5]=1;
	int starting_point=0;		//a counter to search and prioritize the white_node pickup's
	for(int i=0;i<number;i++)
	{
		for(int j=i+1;j<number;j++)
		{
			if(drop_off[j]==18)             //white house priority
			{
				int temp;
				temp=pickup[j];
				pickup[j]=pickup[i];
				pickup[i]=temp;
				temp=drop_off[j];
				drop_off[j]=drop_off[i];
				drop_off[i]=temp;
				starting_point++;
			}
		}
	}

	for (int i = starting_point; i < number; i++)
	{
		if (i == 0)
		{
			source = 1;
		}
		else {
			source = drop_off[i - 1];
		}
		int minm = IN;
		for (int j = i; j < number; j++)
		{
			int distance = dijsktra(cost,source,pickup[j]) + dijsktra(cost,pickup[j],drop_off[j]);
			if (distance < minm)
			{
				minm = distance;
				src = j;
				dst = j;
			}
		}
		int temp;			//swap pickup and drop_off index's if a block is found to be nearest
		temp=pickup[src];
		pickup[src]=pickup[i];
		pickup[i]=temp;
		temp=drop_off[dst];
		drop_off[dst]=drop_off[i];
		drop_off[i]=temp;
	}
	
	
	for (int i = 0; i < number; i++)           //traverse path for the SORTED PICKUP AND DROP_OFF ARRAY THE ORDER OF WHICH IS FOUND USING DJIKSTRA'S ALGORITHM
	{
		if (i == 0)
		{
			source = 1;
			path_travel(cost,source, pickup[i]);
		}
		if((dijsktra(cost,pickup[i],drop_off[i]))>(dijsktra(cost,pickup[i],pickup[i+1]))) //Second Arm Pickup if the first arm drop_off distance > nearest block distance 
		{
			path_travel(cost,pickup[i],pickup[i+1]);
			path_travel(cost,pickup[i+1],drop_off[i+1]);
		}
		else{
			path_travel(cost, pickup[i], drop_off[i]);
			path_travel(cost, drop_off[i], pickup[i+1]);
		}
		if (i == number-1)
		{
			break;
		}
	}
}

/*
*
*Function Name : int_main();
* Input : void
* Output : void
* Logic : main function to initialise the graph
*
*/

int main()
{
	init_devices();
	lcd_set_4bit();
	lcd_init();
	initialise_graph();
	buzzer_on();		//sound buzzer for 5 seconds
	_delay_ms(5000);		//delay
	buzzer_off();
	return 0;
}