#include "Ext_var.h"
#include <SPI.h>

#define MISO 15
#define SCLK 16
#define CS1 3
#define CS2 2

bool Probe_error_flag=0;
bool Intial_time_for_ktype_read_FLAG=0;

 
Mclass::Mclass()
{}

byte spiread(void); 
float Selection_Of_Thermocouple(uint8_t ChipSelectionPin);

void READ_THERMOCOUPLE();
Ticker TEMP_READING(READ_THERMOCOUPLE, 2000, 0);

//void READ_THERMOCOUPLE2();
//Ticker TEMP_READING2(READ_THERMOCOUPLE2, 2500, 0);

void Thermocouple_TEMP_Conversion();
Ticker TEMP_Conversion(Thermocouple_TEMP_Conversion, 3000, 0);

void Intial_time_for_ktype_read();
Ticker Intial_time_for_ktype_read_ticker(Intial_time_for_ktype_read, 5000, 1);
void Intial_time_for_ktype_read()
{
  Intial_time_for_ktype_read_FLAG=1;
}

//************************************************Thermocouple***********************************************
//MAX6675 thermocouple(SCK_PIN, CS_PIN, SO_PIN);
//-----------------------------------------------------------------------------------------------------------
//*********************************************temp**********************************************
// float TEMP_DEG_RAW = 25.5, TEMP_DEG_RAW2, TEMP_DEG_RAW1;
// float TEMP_DEG = 25.5;
// bool SIGN=0;
//------------------------------------------------------------------------------------------------

void Mclass:: Setup()
{      
  SPI.swap(2);
   Serial2.pins(38,39);
  Serial2.begin(9600);

  pinMode(MISO, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(CS1, OUTPUT);
  pinMode(CS2, OUTPUT);

  // digitalWrite(CS1, HIGH);
  // digitalWrite(CS2, HIGH);
//   Serial2.pins(38,39);
//  Serial2.begin(9600);
  
  TEMP_READING.start();
  TEMP_Conversion.start();
  Intial_time_for_ktype_read_ticker.start();
}

float Selection_Of_Thermocouple(uint8_t ChipSelectionPin) //(uint8_t )
{
  uint16_t v;

  digitalWrite(ChipSelectionPin, LOW);
  delayMicroseconds(10);

  v = spiread();
  v <<= 8;
  v |= spiread();

  digitalWrite(ChipSelectionPin, HIGH);

  if (v & 0x4) 
  {
    // uh oh, no thermocouple attached!
//    return NAN;
     return 0;
  }

  v >>= 3;

  return v * 0.25;
   

}

void Mclass:: Temp_Tick_Update()
{    
  TEMP_READING.update();
  Intial_time_for_ktype_read_ticker.update();
  // TEMP_Conversion.update();
}

void READ_THERMOCOUPLE()
{
    TEMP_DEG_RAW1  = Selection_Of_Thermocouple(CS1);   
    TEMP_DEG_RAW2  = Selection_Of_Thermocouple(CS2);
    if(((TEMP_DEG_RAW1==0) && (TEMP_DEG_RAW2==0))  && Intial_time_for_ktype_read_FLAG==1)  //&& Intial_time_for_ktype_read_FLAG==1
    {
      Probe_error_flag=1;
    }
    else if(((TEMP_DEG_RAW1>0) || (TEMP_DEG_RAW2>0)) && Intial_time_for_ktype_read_FLAG==1)  // && Intial_time_for_ktype_read_FLAG==1
    {
      Probe_error_flag=0;
    }
    //  Serial2.print("TEMP_DEG_RAW1 :");
    //  Serial2.println(TEMP_DEG_RAW1);

    //  Serial2.print("TEMP_DEG_RAW2 :");
    //  Serial2.println(TEMP_DEG_RAW2);
}

byte spiread(void) 
{
  int i;
  byte d = 0;

  for (i = 7; i >= 0; i--) 
  {
    digitalWrite(SCLK, LOW);
    delayMicroseconds(10);
    if (digitalRead(MISO))
    {
      // set the bit to 0 no matter what
      d |= (1 << i);
    }
    digitalWrite(SCLK, HIGH);
    delayMicroseconds(10);
  }
  return d;
}

void Thermocouple_TEMP_Conversion()
{ 
    // if(TEMP_DEG_RAW1 == 0) TEMP_DEG_RAW = TEMP_DEG_RAW2;
    // else                   TEMP_DEG_RAW = TEMP_DEG_RAW1;
    // if (!CAL_SELECT_VALUE)  
    // {       
    //   if(SIGN) {ERROR_CAL =(ERROR_CAL1/10) * -1;}      //look in the change
    //   else     {ERROR_CAL =(ERROR_CAL1/10) ;}
    //   TEMP_DEG  =   TEMP_DEG_RAW  +  ERROR_CAL; 
    // }
    // else
    // {
    //    CAL2_RAW_LOW_VALUE = (CAL2_RAW_LOW_VALUE_CONV / 10.0);
    //    CAL2_RAW_HIGH_VALUE = (CAL2_RAW_HIGH_VALUE_CONV  / 10.0);
    //    MASTER_LOW =  (MASTER_LOW_CONV  / 10.0);
    //    MASTER_HIGH = (MASTER_HIGH_CONV  / 10.0);
    //    TEMP_DEG = mapFloat( TEMP_DEG_RAW, CAL2_RAW_LOW_VALUE, CAL2_RAW_HIGH_VALUE, MASTER_LOW,  MASTER_HIGH);
    // }
}

Mclass K_type = Mclass();
