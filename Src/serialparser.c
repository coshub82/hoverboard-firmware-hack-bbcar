
/*
*
* Copyright (C) 2017-2018 coshub82 
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
Function to parse serial frame with a typoplogy of "@""cmd"" ""Value" "\n"
cmd is on 1 char Lengh followed by a space and then number in asci format. 
This state machine is designed to be non blockant when character flow and need to be called each time a new char is received. ( Eg Main Loop). 
/*

//#include<stdio.h>
//#include<stdlib.h>

//#define uint8_t int
//#define uint32_t unsigned int

//char buf[50] = {'@','S',' ','1','0',0x0A,'@','O',' ','1','1','0','0',0x0A,'@','M',0x0A,'@','T',' ','2','2','0',0x0A,'@','T',' ','2','2','0',0x0A};

//HandleTypeDef_Parsermanager parsermanager;

uint8_t UartParser(HandleTypeDef_Parsermanager *P,char SerialIn)
{
switch ((*P).state) {
case 1 : // Test if fisrt Character is "@" & init variable
	if (SerialIn == '@') {
		(*P).state = 2;
		(*P).index = 0;
		(*P).command=0;
		(*P).data[0]=0x00;
		(*P).data[1]=0x00;
		(*P).data[2]=0x00;
		(*P).data[3]=0x00;
	}
break;

case 2: // Check for command value or go back if \n or '@' is detected
	(*P).index = 0;
	(*P).data[0]=0x00;
	(*P).data[1]=0x00;
	(*P).data[2]=0x00;
	(*P).data[3]=0x00;
	if (SerialIn == '@') { // If inputchar is @ don't go to state 3 and Stay to state 2 
		(*P).state = 2;
		(*P).command = 0;
		break;
	}
	if (SerialIn == 0x0A) { // If inputchar is \n (0x0A) go back to state 1
		(*P).state = 1;
		(*P).command = 0;
		break ;
		}	
	(*P).state = 3;     
	(*P).command = SerialIn;
break;

case 3: 
	if (SerialIn == ' ') {
		(*P).state = 4;
		break;
	}
	if (SerialIn == '@') {
		(*P).state = 2;
		(*P).command = 0;
		break;
	}
	if (SerialIn == 0x0A) { 
	    (*P).state=1;
		(*P).value=0;
		return 1; 
	}
	(*P).state = 1;
	(*P).command = 0;	
break;

case 4:
	if (SerialIn == 0x0A) { 
	    (*P).state=1;
		(*P).value=atoi((*P).data);
		return 1; 
	}
	(*P).data[(*P).index]=SerialIn;
	(*P).index++;
	if ((*P).index >4) {
		(*P).state=1;
	}
break;
default: 
(*P).state = 1;
}
return 0;
}


/*void main (void) 
{
	parsermanager.state=1;
	 for (int i = 0; i <= 40; i++){
	     if(UartParser(&parsermanager,buf[i])==1){
	         printf("Commande=%c Value =%i \n",parsermanager.command,parsermanager.value); 
	     }
    }
}*/
