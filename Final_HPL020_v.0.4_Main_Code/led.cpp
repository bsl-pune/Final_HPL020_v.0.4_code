#include "Arduino.h"
#include "Ext_var.h"

long number = 0;                                    //declear the variables
int first_digit = 0;
int second_digit = 0;
int third_digit = 0;
int fourth_digit = 0;

int first_digit2 = 0;
int second_digit2 = 0;
int third_digit2 = 0;
int fourth_digit2 = 0;

static int CA_5 = 11 ; //5;                                // Digit 1 Pins
static int CA_6 = 10; // 6;
static int CA_7 = 9; //7;
static int CA_8 = 8; //4;

static int CA_1 = 4;                                // Digit 1 Pins
static int CA_2 = 5;
static int CA_3 = 6;
static int CA_4 = 7;

static int data1 = 18;                                 // Control Pins
static int latch1 = 19;
static int clk1 = 20;

static int data2  = 31;                                 // Control Pins
static int latch2 = 32;
static int clk2   = 30;

int digits1[4] = {0} ;
int digits2[4] = {0} ;

int count1 = 0;
int count2 = 0;

int CAS1[4] = { 11, 10, 9, 8}; //{ 4, 5, 6, 7}; // {85, 4, 5, 6, 7};
int CAS2[4] = { 4, 5, 6, 7}; //{8, 9, 10, 11};//, 50};

bool DOT2_1=0,DOT2_0=0, DOT2_2=0, DOT1_0=0, DOT1_1=0, DOT1_2=0;

//h  g  f  e  d  c  b  a
//a  b  c  d  e  f  g  h
//1  1  1  1  0  1  1  0 
 
byte numbers[35]={B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110, 0x9c, 0xce,  0xb6, 0xEE, 0x3e, 0x9e,  0x1e, 0x0a, 0xAA,  0xDE, 0x2a,  0x8E,  0x02,  0x3a ,  0x00,   0x08,  0x1C, 0xf6,  0x7C,   0xC6,   0x6E,  0x0C, 0x7a , 0x01             , 0x38};
//                                                                                                                                C      P     S      A     b     E     t     r    m       e     n      F      -     o      blank     i     L     G     U     degree   H      l       d    blank with dot      u
//                    0           1         2            3         4         5           6           7       8           9        10     11    12     13   14    15    16     17    18     19     20     21     22    23     24        25    26    27    28      29     30     31     32     33                34
//************************************************************************************************************************************************************************************************************************************************************************


Eclass::Eclass()
{}

void Eclass:: SETUP()
{
    pinMode(CA_5, OUTPUT);
    pinMode(CA_6, OUTPUT);
    pinMode(CA_7, OUTPUT);
    pinMode(CA_8, OUTPUT);
  
    pinMode(CA_1, OUTPUT);
    pinMode(CA_2, OUTPUT);
    pinMode(CA_3, OUTPUT);
    pinMode(CA_4, OUTPUT);
    
    pinMode(clk1, OUTPUT);
    pinMode(latch1, OUTPUT);
    pinMode(data1, OUTPUT);
    
    pinMode(clk2, OUTPUT);
    pinMode(latch2, OUTPUT);
    pinMode(data2, OUTPUT);
    
    digitalWrite(CA_5, HIGH);
    digitalWrite(CA_6, HIGH);
    digitalWrite(CA_7, HIGH);
    digitalWrite(CA_8, HIGH);
    
    digitalWrite(CA_1, HIGH);
    digitalWrite(CA_2, HIGH);
    digitalWrite(CA_3, HIGH);
    digitalWrite(CA_4, HIGH);

    pinMode(TIMER_LED1, OUTPUT);
    pinMode(TIMER_LED2, OUTPUT);
    
    digitalWrite(TIMER_LED1, LOW);
    digitalWrite(TIMER_LED2, LOW);
  
}


void Eclass:: DISPLAY_NUMBER1() 
{ 
    cathode_high1(); //black screen
    digitalWrite(latch1, LOW); //put the shift register to read
    if(DOT1_0 && count1==0)
    {
      shiftOut(data1, clk1, LSBFIRST, numbers[(digits1[count1])] | 0x01);
    }
    else if(DOT1_1 && count1==1)
    {
      shiftOut(data1, clk1, LSBFIRST, numbers[(digits1[count1])] | 0x01);
    }
    else if(DOT1_2 && count1==2)
    {
      shiftOut(data1, clk1, LSBFIRST, numbers[(digits1[count1])] | 0x01);
    }
    else
    {
      shiftOut(data1, clk1, LSBFIRST, numbers[digits1[count1]]); //send the data   shiftOut(data, clk, LSBFIRST, numbers[digits[count]]); -for 4digits
    }
    digitalWrite(CAS1[count1], LOW); //turn on the relevent digit             digitalWrite(CAS[count], LOW);                         -for 4digits 
    digitalWrite(latch1, HIGH); //put the shift register to write mode
   
     count1++; //count up the digit
     if (count1 >=4) 
     { // keep the count between 0-3
       count1 = 0;
     }
}

void Eclass:: DISPLAY_NUMBER2() 
{ 
    cathode_high2(); //black screen
    digitalWrite(latch2, LOW); //put the shift register to read
    if(DOT2_0 && count2==0)
    {
      shiftOut(data2, clk2, LSBFIRST, numbers[digits2[count2]] | 0x01);
    }
    else if(DOT2_1 && count2==1)
    {
      shiftOut(data2, clk2, LSBFIRST, numbers[digits2[count2]] | 0x01);
    }
    else if(DOT2_2 && count2==2)
    {
      shiftOut(data2, clk2, LSBFIRST, numbers[digits2[count2]] | 0x01);
    }
    else
    {
      shiftOut(data2, clk2, LSBFIRST, numbers[digits2[count2]]); //send the data   shiftOut(data, clk, LSBFIRST, numbers[digits[count]]); -for 4digits
    }
    digitalWrite(CAS2[count2], LOW); //turn on the relevent digit             digitalWrite(CAS[count], LOW);                         -for 4digits 
    digitalWrite(latch2, HIGH); //put the shift register to write mode
   
     count2++; //count up the digit
     if (count2 >=4) 
     { // keep the count between 0-3
       count2 = 0;
       count1 = 0;
     }
}

void Eclass::DISPLAY_DIGIT1(int DIG1_1, int DIG1_2, int DIG1_3, int DIG1_4)
{
    digits1[0] = DIG1_1;
    digits1[1] = DIG1_2;
    digits1[2] = DIG1_3;
    digits1[3] = DIG1_4;
}

void Eclass::DISPLAY_DIGIT2(int DIG2_1, int DIG2_2, int DIG2_3, int DIG2_4)
{
    digits2[0] = DIG2_1;
    digits2[1] = DIG2_2;
    digits2[2] = DIG2_3;
    digits2[3] = DIG2_4;
}

void Eclass:: cathode_high1()
{ 
    digitalWrite(CA_5, HIGH);
    digitalWrite(CA_6, HIGH);
    digitalWrite(CA_7, HIGH);
    digitalWrite(CA_8, HIGH);
}

void Eclass:: cathode_high2()
{ 
    digitalWrite(CA_1, HIGH);
    digitalWrite(CA_2, HIGH);
    digitalWrite(CA_3, HIGH);
    digitalWrite(CA_4, HIGH);
}

void Eclass:: BREAK_NUMBER1(long num) 
{ 
    first_digit = num / 1000;
    digits1[0] = first_digit;
    int first_left = num - (first_digit * 1000);
    second_digit = first_left / 100;
    digits1[1] = second_digit;
    int second_left = first_left - (second_digit * 100);
    third_digit = second_left / 10;
    digits1[2] = third_digit;
    fourth_digit = second_left - (third_digit * 10);
    digits1[3] = fourth_digit;
}

void Eclass:: BREAK_NUMBER2(long num2) 
{ 
    first_digit2 = num2 / 1000;
    digits2[0] = first_digit2;
    int first_left2 = num2 - (first_digit2 * 1000);
    second_digit2 = first_left2 / 100;
    digits2[1] = second_digit2;
    int second_left2 = first_left2 - (second_digit2 * 100);
    third_digit2 = second_left2 / 10;
    digits2[2] = third_digit2;
    fourth_digit2 = second_left2 - (third_digit2 * 10);
    digits2[3] = fourth_digit2;
}


Eclass led =Eclass();
