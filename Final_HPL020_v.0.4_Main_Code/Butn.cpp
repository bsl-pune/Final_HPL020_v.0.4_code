/**********************************************************************************************
*AVINASH GUNJAL
*HPL_V0.0
*Last Modified Date : 23-09-2022
*
**Kishor M
*HPL_V0.04
*Modification : cal2 values are not saved after calibration. code updated
*Last Modified Date : 23-07-2025
***********************************************************************************************/
#include "Arduino.h"
#include "Ext_var.h"

#define PERIOD_EXAMPLE_VALUE  250
Bclass::Bclass()
{}

//------------------------------------------------------PID------------------------------------------------------------------------------
// double PID_SETPOINT, PID_INPUT,PID_OUTPUT,PID_OUTPUT1=0;//PID_OUTPUT     (p=31, Ki=0.024, Kd=290 == pid values for best responce)
// double Kp=15, Ki=0.024, Kd=80, Ki_saved;   //Kp=150, Ki=0, Kd=0, Ki_saved;  0,10,0   Kp=35, Ki=0.1, Kd=70, Ki_saved;
// double consKp = 150, consKi = 0.5, consKd = 400;     // for internal probe values p 200   consKp = 150, consKi = 0.5, consKd = 400; 
//------------------------------------------------------PID------------------------------------------------------------------------------
//------------------------------------------------------PID------------------------------------------------------------------------------
//------------------------------------------------------PID------------------------------------------------------------------------------
double PID_SETPOINT, PID_INPUT,PID_OUTPUT,PID_OUTPUT1=0;//PID_OUTPUT     (p=31, Ki=0.024, Kd=290 == pid values for best responce)
double Kp=15, Ki=0.024, Kd=80, Ki_saved;   //Kp=150, Ki=0, Kd=0, Ki_saved;  0,10,0   Kp=35, Ki=0.1, Kd=70, Ki_saved;
double consKp = 150, consKi = 0.5, consKd = 400;     // for internal probe values p 200   consKp = 150, consKi = 0.5, consKd = 400; 



int STOP_PROCESS=0,WINDOW_SIZE = 1000;   //WINDOW_SIZE = 1000;
unsigned long WINDOW_START_TIME;
PID myPID(&PID_INPUT, &PID_OUTPUT, &PID_SETPOINT, Kp, Ki, Kd, DIRECT);



void Bclass:: SETUP()
{ 
  pinMode(HEATER_STATUS_LED, OUTPUT);
  pinMode(CURRENT_PARA_LED, OUTPUT);
  pinMode(SET_PARA_LED, OUTPUT);
  pinMode(ALARM_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(HEATER, OUTPUT);
  // pinMode(VENT, OUTPUT);
  
  digitalWrite(BUZZER, LOW);
  digitalWrite(HEATER, LOW);
  
  myPID.SetOutputLimits(0, WINDOW_SIZE);
  WINDOW_START_TIME = millis();
  myPID.SetMode(AUTOMATIC);
  
  ALL_LED_OFF();  
   
   cli();
   TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
   TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
   TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);
   TCA0.SINGLE.PER = PERIOD_EXAMPLE_VALUE;
   TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;
   sei();
}

void  Bclass:: TIME_CAL()
{
  if(COUNTDOWN_START)
  {
     TIME_INC++;   
     if(TIME_INC == 3760)   //3760
     {
       if(!ElapsedTime)
       {      
         SET_MIN--;   TIME_INC=0; 
         if(SET_MIN==0)
         {
           ExitFromProcess=1;
           COUNTDOWN_START=0; 
         }
       }
       else
       {
         ElapsedMinute++;
         TIME_INC=0; 
         if(ElapsedMinute >= 6000) 
         {
          ElapsedMinute=0;
          ExitFromProcess=1;
          COUNTDOWN_START=0;
         }
       }
     }
 }
}

ISR(TCA0_OVF_vect)
{
    Butn.TICKER_UPDATE();
    i++;
    Butn.TIME_CAL();
    currentMillis++;
    currentMillis1++;
    INTERATION++;
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;  
}

void Bclass :: PID_RUN()
{
   myPID.Compute();  
}


void Bclass ::PID_callingg()
{
  if(!LONG_PRESS)    PID_INPUT = TEMP_DEG;
  else               PID_INPUT = TEMP_DEG_RAW;
  
//  int PID_DIFF =  PID_SETPOINT - PID_INPUT;
    
  

   
//  if((PID_SETPOINT - PID_INPUT) > 5)       myPID.SetTunings(Kp, Ki, Kd);   //150    
//  else                                     myPID.SetTunings(consKp, consKi, consKd); 
//  
//  if((PID_SETPOINT - PID_INPUT) > 1)       myPID.SetTunings(Kp, Ki, Kd);

   
    if( PID_SETPOINT >= 0  &&  PID_SETPOINT <= 160 ) {Kp=15, Ki=0.050, Kd=80;}  //Kp=15, Ki=0.040, Kd=80;
    else if(PID_SETPOINT > 160 && PID_SETPOINT < 220 ) {Kp=30, Ki=0.07, Kd=100;}  //{Kp=70, Ki=0.07, Kd=100;}
    else if(PID_SETPOINT >= 220 && PID_SETPOINT < 320 ) {Kp=60, Ki=0.05, Kd=100;}   //{Kp=50, Ki=0.40, Kd=100;}  it good when error is less than 20
//    else if(PID_SETPOINT >= 220 && PID_SETPOINT < 320 && PID_SETPOINT - TEMP_DEG < 20 ) {Kp=100, Ki=0.05, Kd=100;}   //{Kp=70, Ki=0.05, Kd=100;}
//    {
//      if(ErroR < 20 || PID_SETPOINT - TEMP_DEG <= 20 && PID_SETPOINT - TEMP_DEG > 10 ) {Kp=70, Ki=0.05, Kd=100;}
//      else {Kp=15, Ki=0.01, Kd=100;}
//    }
//    else Kp=15, Ki=0.024, Kd=80;

    
    myPID.SetTunings(Kp, Ki, Kd); 
    
    PID_OUTPUT1 = PID_OUTPUT;
    if ( PID_SETPOINT - TEMP_DEG < 20 )  //!RESUME_SET_TEMP || RESUME_SET_TEMP > 0 && PID_SETPOINT - TEMP_DEG > 20
    {                                          
    if(PID_SETPOINT >= 30  && PID_SETPOINT <= 160)
    {
      if((PID_SETPOINT - (PID_INPUT)) > 8 && Flag==0)  {WINDOW_SIZE = 1000; Flag++;}    
      else if((PID_SETPOINT - (PID_INPUT)) <= 6 && Flag==1)  {WINDOW_SIZE = 1200; Flag++;} //1200
      else if((PID_SETPOINT - (PID_INPUT)) <= 3 && Flag==2)  {WINDOW_SIZE = 1400; Flag++;} //1600
      else if((PID_SETPOINT - (PID_INPUT)) >= 2 && Flag==3)  {WINDOW_SIZE = 1000; Flag=0;Flag++;} 
    }
    else if(PID_SETPOINT > 160  && PID_SETPOINT < 230)
    {
      if((PID_SETPOINT - (PID_INPUT)) > 8 && Flag==0)  {WINDOW_SIZE = 1000; Flag++;}    
      else if((PID_SETPOINT - (PID_INPUT)) <= 8 && Flag==1)  {WINDOW_SIZE = 1550; Flag++;}//1550   //1600 future value 
      else if((PID_SETPOINT - (PID_INPUT)) <= 4 && Flag==2)  {WINDOW_SIZE = 1750; Flag++;}//1750   //1850 future value 
      else if((PID_SETPOINT - (PID_INPUT)) >= 3 && Flag==3)  {WINDOW_SIZE = 1000; Flag=0;Flag++;} 
    }
//    else if(PID_SETPOINT >= 230  && PID_SETPOINT < 270)
//    {
//      if((PID_SETPOINT - (PID_INPUT)) > 7 && Flag==0)  {WINDOW_SIZE = 1000; Flag++;}    
//      else if((PID_SETPOINT - (PID_INPUT)) <= 7 && Flag==1)  {WINDOW_SIZE = 1600; Flag++;}   //1200
//      else if((PID_SETPOINT - (PID_INPUT)) <= 5 && Flag==2)  {WINDOW_SIZE = 1800; Flag++;}  //1400
//      else if((PID_SETPOINT - (PID_INPUT)) >= 3 && Flag==3)  {WINDOW_SIZE = 1000; Flag=0;Flag++;} 
//    }
    else if(PID_SETPOINT >= 230  && PID_SETPOINT < 320)
    {
     if((PID_SETPOINT - (PID_INPUT)) > 7 && Flag==0)  {WINDOW_SIZE = 1000; Flag++;}    
     else if((PID_SETPOINT - (PID_INPUT)) <= 4 && Flag==1)  {WINDOW_SIZE = 1550; Flag++;}  //1500
     else if((PID_SETPOINT - (PID_INPUT)) <= 2 && Flag==2)  {WINDOW_SIZE = 1750; Flag++;}  //1650
     else if((PID_SETPOINT - (PID_INPUT)) >= 3 && Flag==3)  {WINDOW_SIZE = 1000; Flag=0;Flag++;} 
    }
    } 
    else if( PID_SETPOINT - TEMP_DEG  >= 20 ) WINDOW_SIZE = 1000;  
//    else if(RESUME_SET_TEMP > 0 && RESUME_SET_TEMP < 200 && PID_SETPOINT - TEMP_DEG  <= 20 ) WINDOW_SIZE = 1000;  
//    else if( RESUME_SET_TEMP >= 220 && RESUME_SET_TEMP < 320 )   
//          {
//            if(PID_SETPOINT - TEMP_DEG  >= 10) WINDOW_SIZE = 1000;
//            else WINDOW_SIZE = 1300;
//          }
                         
  if (millis() - WINDOW_START_TIME > WINDOW_SIZE)
  { 
    WINDOW_START_TIME += WINDOW_SIZE;
  }
  
  if (PID_OUTPUT < millis() - WINDOW_START_TIME)      {digitalWrite(HEATER, LOW);}
  else  
  {
    if(PID_INPUT >= (float(PID_SETPOINT) + 0.1))      {digitalWrite(HEATER, LOW);}
    else                                              {digitalWrite(HEATER, HIGH);}
  }
}



void Bclass :: SET_CHANGE_UP()
{
  if(LONG_PRESS )
  {
        if( SELECTION_PARAMETER == Time_Mode ) //time       //  SET_LONG_VALUE[SELECTION_PARAMETER]  SET_LONG_VALUE[25]={0};       SPEED_SW[5]={1, 2, 4, 5, 10};   INC=0      MAX_SET_VALUE[6]={0, 99, 9, 99, 59, 1};
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 0; 
        }
       
        else if( SELECTION_PARAMETER == Auto_Resume_Enable )  
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC];
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 0; 
        }
        
        else if( SELECTION_PARAMETER == Safe_Value_ON_OFF )   //safe value   
        {
          SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
          if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 0;//99

        }
        
        else if( SELECTION_PARAMETER == Calibration_Selection )   //calb
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 0; 
        }
       
        else if( SELECTION_PARAMETER == OnePointCalibration )   //cal1-l
        { 
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC];
           if(SET_LONG_VALUE[SELECTION_PARAMETER] >MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 320; 
        } 
        
        else if( SELECTION_PARAMETER == Cal2LowTemp_Set_Display )   // cal2-l
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC];
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 150; //cal2 LOW 
        } 
       
        else if( SELECTION_PARAMETER == Cal2HighTemp_Set_Display )   //cal2-H
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 300;  //cal2 HIGH
        }       
      
        else if( SELECTION_PARAMETER == Cal2LowTemp_Enter_Heat )   //inprocess entering values
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC];
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] =150; 
        }
       
        else if( SELECTION_PARAMETER == Cal2LowTemp_Enter_Maintain )   //inprocess entering values
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] =9; 
        }
     
        else if( SELECTION_PARAMETER ==  Cal2HighTemp_Enter_Heat )   //inprocess entering values
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] =310; 
        }
        
        else if( SELECTION_PARAMETER == Cal2HighTemp_Enter_Maintain )   //inprocess entering values
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 9; 
        } 
       
        else if( SELECTION_PARAMETER == SafeValue )   //safe value set
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 300; 
        }
       
        else if( SELECTION_PARAMETER == blank1 )   //vent open time
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 50; 
        }

        else if( SELECTION_PARAMETER == Energy_Mode_Selection )   //sure rest 16
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 0; 
        }

   
        else if( SELECTION_PARAMETER == FactoryAndSure_Reset )   //sure rest 16
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 99; 
        }
     
        else if( SELECTION_PARAMETER == ResetDone )   // rest (---- ----) 17
        {
           SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC]; 
           if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER]) SET_LONG_VALUE[SELECTION_PARAMETER] = 1; 
        }
 
  }   
  else       //set limit values for single mode click and set temp hour and minute limit values
  {
              if( ENTER_BUT == 1 )   //SPEED_SW[5]={1, 2, 4, 5, 10};  SET_LONG_VALUE[25]={0}; MAX_SET_VALUE[6]={0, 99, 9, 99, 59, 1};
              {       
                  if(!Energy_Mode)
                  {  
                     
                     SET_VALUE[ENTER_BUT] += SPEED_SW[INC];
                  
                     if(SAFE_VALUE_SET_DEG == 1)
                       {
                         if(SET_VALUE[ENTER_BUT] > SAFE_VALUE_SET) SET_VALUE[ENTER_BUT] = SAFE_VALUE_SET;   //FOR 1ST TWO DIGIT ON TEMP SET 00XC  
                       }
                    else if(SET_VALUE[ENTER_BUT] > 300) SET_VALUE[ENTER_BUT] = 0; 
                  }
                  else
                  {
                    SET_VALUE[6] += SPEED_SW[INC];
                    if(SET_VALUE[6] > 10) SET_VALUE[6] = 0;
                  }
                
               }
               else if( ENTER_BUT == 3 )   //set limit for hour 
               {
                SET_VALUE[ENTER_BUT] +=SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] > MAX_SET_VALUE[ENTER_BUT]) SET_VALUE[ENTER_BUT] = 0;   // FOR 1st TWO DIGIT ON TIME SET 00XX
               }
               else if( ENTER_BUT == 4 )    //set limit for minute
               {
                SET_VALUE[ENTER_BUT] +=SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] > MAX_SET_VALUE[ENTER_BUT]) SET_VALUE[ENTER_BUT] = 0;   //FOR LAST TWO DIGIT ON TIME SET XX00
               }  
               else if( ENTER_BUT == 5 && VENT_FLAG )   //set vent open or close
               {
                SET_VALUE[ENTER_BUT] +=SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] > MAX_SET_VALUE[ENTER_BUT]) SET_VALUE[ENTER_BUT] = 0; 
               }
  
  }
}

void Bclass :: SET_CHANGE_DOWN()
{
  if(LONG_PRESS )
  {
        if( SELECTION_PARAMETER == Time_Mode  )    //settings time
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 1;
        }
        
        else if( SELECTION_PARAMETER == Auto_Resume_Enable )   //settinge print(unit)
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 1;
        }
        
        else if( SELECTION_PARAMETER == Safe_Value_ON_OFF )   // safe value on off
        {
          SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC];
          if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 1;  
         }
       
        else if( SELECTION_PARAMETER == Calibration_Selection )   // calb
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 1;
        }
       
        else if( SELECTION_PARAMETER == OnePointCalibration )   //cal1
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 0;
        }
        
        else if( SELECTION_PARAMETER == Cal2LowTemp_Set_Display )   
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 70) SET_LONG_VALUE[SELECTION_PARAMETER] = 70;
        }
     
        else if( SELECTION_PARAMETER == Cal2HighTemp_Set_Display )   
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC];
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 230) SET_LONG_VALUE[SELECTION_PARAMETER] = 230;
        }
       
        else if( SELECTION_PARAMETER == Cal2LowTemp_Enter_Heat )   
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 0;
        }
        
        else if( SELECTION_PARAMETER == Cal2LowTemp_Enter_Maintain )   
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 0;
        }
        
        else if( SELECTION_PARAMETER == Cal2HighTemp_Enter_Heat )   
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 0;
        }
        
        else if( SELECTION_PARAMETER == Cal2HighTemp_Enter_Maintain )   
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 0;
        }

        else if( SELECTION_PARAMETER == SafeValue )   //settings safe values lower limit
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 0;
        }

        
        else if( SELECTION_PARAMETER == Energy_Mode_Selection )   
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 1;
        }
        
        else if( SELECTION_PARAMETER == blank1 )   
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 0;
        }
        
        else if( SELECTION_PARAMETER == FactoryAndSure_Reset )   
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 0;
        }   
             
        if( SELECTION_PARAMETER == ResetDone )   
        {
            SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
            if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0) SET_LONG_VALUE[SELECTION_PARAMETER] = 0;
        }
  }
  else       //set limit values for single mode click and set temp hour and minute limit values
  {
               if( ENTER_BUT == 1 )   
               {                    
                 if(!Energy_Mode)
                 { 
                       SET_VALUE[ENTER_BUT] -= SPEED_SW[INC];

                      if( SAFE_VALUE_SET_DEG == 1 )  //SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; 
                      {  
                                 if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 0;//SAFE_VALUE_SET   SAFE_VALUE_SET_DEG; 
                      }
                    // else SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; 
                    else if( SAFE_VALUE_SET_DEG == 0 ) 
                    {
                      if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 300;
                    }
                    
                    // INC=0;
                    // if( SAFE_VALUE_SET_DEG == 1 ) SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; 
                    //             if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 0;//SAFE_VALUE_SET   SAFE_VALUE_SET_DEG; 
                    // else SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; 
                    //             if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 300;
                 }
                else
                {                  
                  SET_VALUE[6] -= SPEED_SW[INC];
                  if(SET_VALUE[6] < 0) SET_VALUE[6] = 10;                
                }
               }
//               else if( ENTER_BUT == 2 )   
//               {
//                SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 9; //SET_VALUE[ENTER_BUT] = 9; 
                
//                 if(!PRINTER_VALUE_SET) SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 9; 
//                 else                  SET_VALUE[22] -= SPEED_SW[INC]; if(SET_VALUE[22] < 32) SET_VALUE[22] = SAFE_VALUE_SET_FAH; 
//               }
               else if( ENTER_BUT == 3 )   //for hour
               {
                SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 99; 
               }
               else if( ENTER_BUT == 4 )   //for minute
               {
                SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 59; 
               }
               else if( ENTER_BUT == 5 && VENT_FLAG )   //vent on off
               {
                SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 1; 
               }
  }
}

void Bclass :: ALL_LED_OFF()
{
  digitalWrite(ALARM_LED, HIGH);
  digitalWrite(SET_PARA_LED, HIGH);
  digitalWrite(CURRENT_PARA_LED, HIGH);
  digitalWrite(HEATER_STATUS_LED, HIGH);
}


Bclass Butn =Bclass();
