#include "Arduino.h"
#include "Ext_var.h"

int8_t EnergySetValue;

int _curCycle;
int _NUM_CYCLES;
int array_count=0;

boolean _PIN_STATUS;

unsigned long _tsaved;
long unsigned _ON_DURATION=0;
long unsigned _OFF_DURATION=0; 

long unsigned off1[]={0,1000,2000,3000,4000,5000,6000,7000,8000,9000};
long unsigned onn1[]={0,9000,8000,7000,6000,5000,4000,3000,2000,1000};

void restart();
void check(long unsigned onn, long unsigned off);


Rclass::Rclass()
{}

void Rclass:: SETUP()
{ 
}

void Rclass:: EnergyModeOn()
{    
          if(EnergySetValue  == 0)  digitalWrite( HEATER , LOW);            //----------------------------------Heater full off
          else 
          if(EnergySetValue  == 10) digitalWrite( HEATER , HIGH);            //----------------------------------Heater full on
          else
          {
            array_count= EnergySetValue ;                                     //---------------------------------- Energy value pass
            check(onn1[array_count], off1[array_count]);                 //---------------------------------- ON timeand off time pass
          }

   
}

void restart() //----------------------------------All Supporting Flag OFF
{
 _tsaved = 0;
  _curCycle = 0;
  _PIN_STATUS = false;
  digitalWrite( HEATER , LOW);
}

void check(long unsigned onn, long unsigned off)
{
  _OFF_DURATION = off;                   //----------------------------------Heater off time passing
  _ON_DURATION  = onn;                   //----------------------------------Heater on  time passing
  if(!_PIN_STATUS)                       //---------------------------------- till complition of ON time heater on loop
  {  
    if(_ON_DURATION <= millis() - _tsaved) 
    {
       _PIN_STATUS = !_PIN_STATUS;
       _tsaved = millis();
    } 
    digitalWrite( HEATER , LOW);  
  }  
  else                                   //---------------------------------- till complition of OFF time heater off loop
  {  
    if(_OFF_DURATION <= millis() - _tsaved) 
    {
      _PIN_STATUS = !_PIN_STATUS;
      _tsaved = millis();              
    }
    digitalWrite( HEATER , HIGH);       
  }
}


Rclass EnergyRegulator =Rclass();
