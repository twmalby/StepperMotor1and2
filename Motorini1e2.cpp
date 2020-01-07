
#include<conio.h>
#include<bios.h>
#include<stdio.h>
#include<dos.h>
#include<string.h>


#define LPT1      0x378

#define DATA_READY 0x100
#define SETTINGS ( 0x80 | 0x02 | 0x00 | 0x00)

#define N_STATI 7
#define M_STATI 7
//clrscr();
//textbackground(233);
//textclor(3);
unsigned int stepper1[N_STATI]  = {0x01,0x03,0x06,0x04,0x0C,0x08,0x09};
unsigned int stepper2[M_STATI]  = {0x10,0x30,0x60,0x40,0xC0,0x80,0x90};
unsigned int ultimostato= 0x01;

unsigned int get_stato(int indice_motorino){
unsigned int feedback = ultimostato;


if (indice_motorino==1){
return feedback & 0x0F;
}
if (indice_motorino == 2){
  return feedback & 0xF0;
}
   return 0;
}

void move(int indice_motorino, int direzione){
clrscr();
textbackground(233);
textcolor(3);
int indice;
unsigned int *stepper;
unsigned int stato;
if (indice_motorino==1){
  stepper = stepper1;
}
if (indice_motorino==2){
    stepper = stepper2;
}
stato = get_stato(indice_motorino);

for(int i=0; i<N_STATI; i++){
  if (stato==stepper[i]){
    if (direzione==1){
    indice = (i+1)%7;
  }
      if (direzione==-1){
	indice = ((i-1)%7);
	if(indice < 0){
	indice = 6;
	}
      }
    printf("\n i = %d\n",i);
printf("index =  %d \n",indice);
  }
}

indice = indice%7;

printf("\n index =  %d \n",indice);
unsigned int mask;
unsigned int newvalue = stepper[indice];
if(indice_motorino == 1){
  mask=0x0F;
}
if(indice_motorino == 2){
  mask=0xF0;
}
unsigned int value = (stato & ~mask) | (newvalue & mask);
printf("\n value =0x%X \n",value);
outportb(LPT1,value);
ultimostato=value;
delay(5);
clrscr();

}

void move_k_times(int indice_motorino, int direzione, int k ){
 clrscr();
 textbackground(233);
 textcolor(3);
  for(int x=0; x<k; x++){

  while(inportb(LPT1+1)==0xF8){
  printf("\n### Status CLOSED end-run fine-corsa ###\n");
  delay(100);
  clrscr();
   }

move(indice_motorino,direzione);

  }




}

void scanner(){
clrscr();
textbackground(233);
textcolor(3);
printf("\n ### Led Blinking! press any key to stop the scanning... ### \n");
  while(!kbhit()){

  outportb(LPT1,127);
  delay(10);
  outportb(LPT1,0);
  delay(10);

  }

}


void main(){
clrscr();
clrscr();
textbackground(233);
textcolor(3);
printf("\n Welcome to STPMTR12 v1.2, type help to show the command's list\n \n");
while(1){
int stepper,times,direzione;
char command[15];

printf("Command line: ");

scanf("%s",command);
if(strcmp(command,"help")==0){
//printf("\n###################################################");
//printf("#");
printf("\n#### Welcome to STPMTR12 #####  v1.2 ###############\n");
printf("\n# scanner = check if the stepper are linked then blinking\n");
printf("\n# move = to let a stepper to move forward or back for k-steps\n The Stepper 1 is linked to the first 4 bit and the Stepper 2 is linked to the last 4 bit. \nConsidering D0 synched to the bit 1 \n");
printf("\n# read = check the pin 11 of LPT is CLOSED or OPENED.\n");
printf("\n# exit = to back to ms-dos\n");
printf("\n# version = to check version and author");
printf("\n# feedback = to check the last state, for ex. 0x0C\n");
printf("\n# lpt1+2, lpt1+3 = check which address is synched\n");
printf("\n# clearscreen = erase all the screen\n");
printf("\n \n");
}
if (strcmp(command,"scanner")==0) {
scanner();
}
if(strcmp(command,"move")==0) {

  printf("Which stepper 1 or 2: ");
  scanf("%d",&stepper);
  if(stepper>2){
  printf("Out of range.");
  return; }
  printf("\n Direction Foreward 1, Back -1: ");
  scanf("%d",&direzione);
  printf("\n How many k-times");
  printf("\n 2670 is equal (about) 4/4 of circle");
  printf("\n k = ");
  scanf("%d",&times);
  move_k_times(stepper,direzione,times);


}
if(strcmp(command,"read")==0) {
  clrscr();
  printf("LPT1+1 = 0x%X\n ", inportb(LPT1+1));
  if (inportb(LPT1+1)==0x78){
  printf("Status: OPEN\n");   }
  if (inportb(LPT1+1)==0xF8){
  printf("Status: CLOSED\n");}
}

if(strcmp(command,"synch")==0){
printf("\nSynching...\n");
delay(1000);
printf("\n Hemmm, but what?\n");

}
if (strcmp(command,"exit")==0) {
return;
}

if (strcmp(command,"feedback")==0) {
clrscr();
printf("Last state = 0x%X ",ultimostato);
}


if (strcmp(command,"version")==0) {
clrscr();
printf("\nSTPMTR12 - Step Motor 1 and 2  version 1.2 \nBy Alberto Ragagnin \n Thanks to Riccardo Ragagnin and Antonio Ragagnin \nfor the useful help given to do that project.\n\n ");
printf("\nLast update: October 2019\n\n");
}
if(strcmp(command,"lpt1+2")==0){
clrscr();
printf("\n LPT1+2 is synched to : 0x%X  \n",inportb(LPT1+2));
}

if(strcmp(command,"lpt1+3")==0){
clrscr();
printf("\n LPT1+3 is synched to : 0x%X\n",inportb(LPT1+3));


}

if(strcmp(command,"clearscreen")==0){
clrscr();
}
else{printf("\n ### Type 'help' to see the command's list. ###\n \n");}





textbackground(233);
textcolor(3);
}

}
