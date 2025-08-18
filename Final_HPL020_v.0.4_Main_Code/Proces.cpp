#include "Arduino.h"
#include "Ext_var.h"
#include <EEPROM.h>

Pclass::Pclass()
{}

bool process_end = 0;
bool soak_once_flag = 0;
bool Inprocess_flag = 0;
//-----------------------------------------------------------------------------------------------------------------------------------------------------
bool blankDisplayFlag = 0;
void flahDisplayFunction();
Ticker flahDisplay(flahDisplayFunction, 500, 0, MILLIS);

void flahDisplayFunction()
{
  blankDisplayFlag == 1 ? blankDisplayFlag = 0 : blankDisplayFlag = 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
int tempArray[6];
int checkArray[5];
int incArray = 0;
bool checkTempRaised = 0;

bool processRunUpdates = 0;

void readTemp();
Ticker tempRiseCheck(readTemp, 30000, 0, MILLIS);

void readTemp()
{
  // Serial3.println("tick"); //--------------change
  if (incArray < 6)
  {
    tempArray[incArray] = TEMP_DEG;
    incArray++;
  }
}

void Pclass:: tempRiseCheckUpdate()
{
  tempRiseCheck.update();
}
// -----------------------------------------------------------------------------------------------------------------------------

// void Autoresume_show();
// Ticker Autoresume_show_tick(Autoresume_show, 1000, 0, MILLIS);

// void Autoresume_show()
// {
//  if(auto_flag==1)
//    auto_flag=0;
//   else if (auto_flag==0)
//    auto_flag=1;
// }
bool SSR_fail_toggle = 0;
void toggle_fun();
Ticker toggle_ticker(toggle_fun, 1000, 0, MILLIS);  //Time_speed_show_toggle
void toggle_fun()
{

  SSR_fail_toggle == 0 ? SSR_fail_toggle = 1 : SSR_fail_toggle = 0;

}



bool Energy_Mode = 1;
bool ElapsedTime = 0;
unsigned int ElapsedMinute = 0;
bool ExitFromProcess = 0;

//********************************************************LONGPRESS***************************************
float ERROR_CAL, CAL2_CALCULATED_LOW_CAL_VALUE, CAL2_CALCULATED_HIGH_CAL_VALUE ;
float CAL2_RAW_LOW_VALUE = 0.00, CAL2_RAW_HIGH_VALUE = 0.00, RAW_LOW_VALUE = 0.00, RAW_HIGH_VALUE = 0.00, MASTER_LOW = 0.00, MASTER_HIGH = 0.00;

bool LED_STATE = HIGH,  RST_BUTT = 0, TIME_SOCK_OR_TOTAL = 0;
bool PRO_STOP_WITH_LONG_PRESS = 0, CAL2_START = 0;
bool BLINK_STATS = 0, EXECUTE_PID_LOOP = 0;
//bool START_COUNTDOWN_FLAG=false;
double  ERROR_CAL1;
long    CAL2_RAW_LOW_VALUE_CONV, CAL2_RAW_HIGH_VALUE_CONV, MASTER_LOW_CONV, MASTER_HIGH_CONV;
bool VENT_VALUE_SET = 0, PRINTER_VALUE_SET = 0, START_CAL_VALUE = 0, AUTO_RESUM_SET = 0, SAFE_VALUE_SET_DEG = 0;
int  SAFE_VALUE_SET = 0, SAFE_VALUE_SET_FAH = 0, ERROR_VALUE_SET = 0, VENT_TIME_SET = 0;
int  LOW_CAL_VALUE_SET = 100, HIGH_CAL_VALUE_SET = 250, CAL_SELECT_VALUE = 0;
int  CAL2_LOW1_VALUE_SET = 0, CAL2_LOW2_VALUE_SET = 0, CAL2_HIGH1_VALUE_SET = 0, CAL2_HIGH2_VALUE_SET = 0;
int  CAL1_VALUE_SAVED_DEG1 = 0, CAL1_VALUE_SAVED_DEG2 = 0, CAL1_VALUE_SAVED_FAH = 0;
int  SELECTION_PARAMETER = 1, REPET = 0, TEMP_FLAG = 0;
int  SET_LONG_VALUE[25] = {0};
//---------------------------0  1  2  3   4   5   6    7    8   9   10  11   12  13  14   15   16  17
int  MAX_SET_VALUE_LONG[20] = {1, 1, 1, 1, 320, 9, 150, 300, 150, 9, 310,  9,  1,   1,  1,  999, 50, 300, 1 };
//MAX_SET_VALUE_LONG[20]   ={1, 1, 99, 1, 99, 9, 50, 95, 50, 9, 99, 9, 1, 1, 999, 9999, 999, 1 };


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//********************************************************SINGLEPRESS***************************************
// float SET_TEMP_DEG=0.00;
int SET_TEMP_DEG = 0;
int SET_TEMP_FAH = 0, SET_TEMP = 0, SET_HOUR = 0, SET_MIN = 0, RESUME_TIME = 0, RESUME_TEMP = 0, MIN_RESUME_TIME = 0, TIME_FACT = 0, CURRENT_TIME = 0;
int i = 0, j = 0, TIME_INC = 0, REMAIN_MIN = 0;
int INC = 0;
double DISP_INC;
int SET_VALUE[7] = {0, 0, 0, 0, 0, 0, 0};
int MAX_SET_VALUE[7] = {0, 99, 9, 99, 59, 1, 10}; //{0, 99, 9, 99, 59, 1}
int SPEED_SW[5] = {1, 2, 4, 5, 10};
bool COUNTDOWN_START = 0, Auto = 0;
int  currentMillis = 0;
int  currentMillis1 = 0, flag = 0;
long previousMillis = 0;
//********************************************************LEVEL_DETECTION***************************************
bool CHECK_RATE_OF_HEATING = 0;
float TEMP_VALUE_INITIAL, TEMP_VALUE_FINAL ;
unsigned long START_COUNT_VALUE, STOP_COUNT_VALUE ;
float RATE_OF_HEATING = 0;

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//------------------------------------------------------------------------------------------------------------------------------------
int INTERATION = 0, TOLERANCE_VALUE = 0;
unsigned int FOR_INTERATION = 0;
int VALUE_FOR_BLINK = 25, TOTAL_VALUE_FOR_BLINK = 50;
float PREVIOUS_TEMP_COOLING_RATE = 0.00, CURRENT_TEMP_COOLING_RATE = 0.00, BUFF = 1;
float COOLING_RATE = 0.00, COOLOING_RATE_COMP_VAL = 0.04;
unsigned long  PREVIOUS_COOLING_RATE_TIME = 0 , CURRENT_COOLING_RATE_TIME = 0, CAL_OF_10_MIN = 0, TO_CHECK_REPETATION_WITHIN_TIME = 0;
int COOLING_CYCLE_COUNT = 0;
bool FILL_BATH_ERROR_NOT_CLEAR = 0;
bool FOR_BUZZ = 0;
//------------------------------------------------------------------------------------------------------------------------------------
int CounT = 0, Flag = 0, ErroR = 0, ErroRConst = 0, RESUME_Temp = 0, DIV_FACT = 0;
int MIN_SET_TEMP = 0, MIN_SET_FAH = 0, RESUME_SET_TEMP = 0, TEMP_BUFF = 0;
bool IN_ERROR = 0, REPETATION_FLAG = 0;
bool  TIMER_LED_BLINK = 0, CHANGE1 = 0, Heat = 0;
bool Inprocess_autoresume_flag = 0;
bool once1 = 0;
// bool Contactor_off_once_flag=0;
// int z=0;

/*------------------------------------------------------------------------------------------------------------------------------------*/
void Pclass:: SETUP()
{
  pinMode(safteyRelay, OUTPUT);
  // digitalWrite(safteyRelay, HIGH);  //HIGH  //---------------------
  digitalWrite(safteyRelay, LOW);  //---------Initially contactor should OFF(LOW),heater circuit will be open

  tempRiseCheck.interval(30000);
  flahDisplay.start();
  tempRiseCheck.start();
  toggle_ticker.start();

  Inprocess_autoresume_flag = EEPROM.read(160);

}

void Pclass :: SSR_fail_show_ticker_update()
{
  toggle_ticker.update();
}


void Pclass :: SET_SHORT_PRESS_FUNCTION()
{
  switch (ENTER_BUT)
  { //with out any key press control is here display current temp and time
    case 0 :
      //     {
      //          digitalWrite( HEATER , LOW);
      // digitalWrite(ALARM_LED, HIGH);
      // Intial_time_for_ktype_read_FLAG=0;
      ExitFromProcess = 0;
      soak_once_flag = 0;
      Inprocess_flag = 0;
      process_end = 0;
      TIME_INC = 0;
      if (!Energy_Mode)
      {
        SET_VALUE[1] = SET_TEMP_DEG;
      }
      else
      {
        SET_VALUE[6] = EnergySetValue;
      }
      SET_VALUE[3]  = (EEPROM.read(152) << 8) + EEPROM.read(153);
      SET_VALUE[4] = (EEPROM.read(154) << 8) + EEPROM.read(155);
      ElapsedMinute = 0;
      // SET_VALUE[3]=
      // SET_VALUE[4]=
      // if (RESUME_TIME > MIN_RESUME_TIME && AUTO_RESUM_SET == 1 )   //&& REPET == 0  RESUME_TIME > MIN_RESUME_TIME &&

      if (Inprocess_autoresume_flag && EEPROM_AUTO_RESUM_SET == 1)
      {
        // REPET=0;
        // SET_MIN = RESUME_TIME;

        // if(!Energy_Mode)
        // {
        //   SET_TEMP_DEG = RESUME_TEMP;
        //   TEMP_DEG = RESUME_TEMP;
        // }
        // else
        // {
        //   EnergySetValue;
        // }

        // BLINK_STATS=0;
        // i++;
        // lup=0;
        // if(autoresume_alert_flag==0)
        // {


        //       if()
        //       {
        //         digitalWrite(BUZZER, LOW);
        //         // digitalWrite(TIMER_LED2, LOW);  //SET_PARA_LED

        //         led.DISPLAY_DIGIT2(24, 24, 24, 24) ;
        //         led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
        //       }
        //       else
        //       {
        //         digitalWrite(BUZZER, HIGH);
        //         DOT2_1=0;
        //         //  digitalWrite(TIMER_LED2, LOW);
        //         led.DISPLAY_DIGIT2(12, 12, 17, 24);// ssr         led.DISPLAY_DIGIT1(16, 23, 16, 26);  //off   (0, 21, 21, 24)
        //         led.DISPLAY_DIGIT1(21, 13, 25, 26);// FAiL       led.DISPLAY_DIGIT1(12, 23, 13, 10);  //on    (0, 20, 24, 24)
        //       }
        // }
        // else if(autoresume_alert_flag==1)
        // {
        // ENTER_BUT = 5;
        // }
        if (z < 3)
        {
          DOT1_2 = 0;
          DOT1_1 = 0;
          DOT1_0 = 0;
          if (i < VALUE_FOR_BLINK + 50)
          {
            digitalWrite(BUZZER, HIGH);
            led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
            led.DISPLAY_DIGIT2(24, 24, 24, 24) ;
          }
          else
          {
            digitalWrite(BUZZER, LOW);

            led.DISPLAY_DIGIT2(11, 17, 23, 12);
            led.DISPLAY_DIGIT1(12, 16, 17, 16);
          }
          if (i > TOTAL_VALUE_FOR_BLINK + 100) {
            z ++;
            i = 0;
          }
        }
        else  ENTER_BUT = 5;

      }
      else
      {

        //flag=0;
        once1 = 0;
        REPET = 0;
        DOT1_1 = 0;
        DOT1_0 = 0;
        DOT1_2 = 0;
        DOT2_2 = 0;
        DOT2_0 = 0;
        REP = 0;
        REP1 = 0;
        CHANGE1 = 0;
        VENT_FLAG = 0;
        IN_ERROR = 0;
        FOR_REP = 0;
        FOR_REP2 = 0;
        CAL2_START = 0;
        PID_SETPOINT = 0;
        STOP_PROCESS = 0;
        Butn.BUZZ_TICK_STOP();
        RATE_OF_HEATING = 0;
        MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 0;
        EXECUTE_PID_LOOP = 0;
        REPETATION_FLAG = 0;
        CHECK_RATE_OF_HEATING = 1;
        SELECTION_PARAMETER = Time_Mode; //0;

        // digitalWrite(VENT, LOW);
        digitalWrite(HEATER, LOW);
        PRO_STOP_WITH_LONG_PRESS = 0;
        CounT++;


        if (incArray >= 6 && !checkTempRaised)
        {
          int i;
          //  Serial3.println("**************************************"); //--------------change


          for (i = 0; i < 5; i++)
          {
            if (tempArray[i] < tempArray[i + 1])
              checkArray[i] = 1;
            else
              checkArray[i] = 0;
            // Serial3.println(tempArray[i]); //--------------change
            // Serial3.println(checkArray[i]);
          }
          i = 0;
          if (checkArray[i] & checkArray[i + 1] & checkArray[i + 2] & checkArray[i + 3] & checkArray[i + 4])
          {
            flahDisplay.interval(500);
            checkTempRaised = 1;
          }
          else
            checkTempRaised = 0;
          if (processRunUpdates)
          {
            tempRiseCheck.interval(30000);
            processRunUpdates = 0;
          }
          incArray = 0;
        }

        if (TEMP_DEG >= 430)
        {

          digitalWrite(safteyRelay, LOW);
          // checkTempRaised=1;
        }
        //  else    checkTempRaised=0;

        if (!checkTempRaised && Probe_error_flag == 0)
        {
          digitalWrite(BUZZER, LOW);
          if (TEMP_DEG >= 50) // HOT SURFACE INDICATION
          {
            Butn.ALL_LED_OFF();
            if (CounT >= 0  && CounT < 10000)
            {
              if (!Energy_Mode)
              {
                led.BREAK_NUMBER2(TEMP_DEG * 10);
                digits2[3] = 10;  //C
                DOT2_1 = 0;
              }
              else
              {
                DOT2_2 = 0;
                DOT2_1 = 1;
                led.BREAK_NUMBER2(0000);
                digits2[0] = 15;   //C
                digits2[1] = 20;   //C
              }
              digitalWrite(TIMER_LED1, HIGH);
              led.BREAK_NUMBER1(0000);
            }
            else if (CounT > 10000 && CounT < 30000)
            {
              digitalWrite(TIMER_LED1, LOW);

              //                if(i<VALUE_FOR_BLINK)    {led.DISPLAY_DIGIT2(24, 24, 24, 24) ;  led.DISPLAY_DIGIT1(24, 24, 24, 24) ;}
              //                else
              {

                DOT2_1 = 0;
                led.DISPLAY_DIGIT2(30, 23, 16, 24);// Hot
                led.DISPLAY_DIGIT1(12, 34, 17, 21);// SurF
              }
              if (i > TOTAL_VALUE_FOR_BLINK)    i = 0;
            }
            else if (CounT > 30000) CounT = 0;
          }
          else
          {
            digitalWrite(TIMER_LED1, HIGH);

            if (!Energy_Mode)
            {
              led.BREAK_NUMBER2(TEMP_DEG * 10);
              digits2[3] = 10;  //C
              DOT2_1 = 0;
              DOT2_2 = 0;
            }
            else
            {
              DOT2_2 = 0;
              DOT2_1 = 1;
              led.BREAK_NUMBER2(0000);
              digits2[0] = 15;   //E
              digits2[1] = 20;   //n
            }

            led.BREAK_NUMBER1(0000);  //0000  without any key press control is here
            Butn.ALL_LED_OFF();
          }

        }
        else if (checkTempRaised && Probe_error_flag == 0)
        {
          digitalWrite(safteyRelay, LOW); //--------Contactor OFF
          digitalWrite(SET_PARA_LED, HIGH);
          digitalWrite(CURRENT_PARA_LED, HIGH);
          digitalWrite(HEATER_STATUS_LED, HIGH);  //
          digitalWrite(ALARM_LED, LOW);
          digitalWrite(TIMER_LED2, LOW);
          digitalWrite(TIMER_LED1, LOW);
          flahDisplay.update();
          if (SSR_fail_toggle == 0)
          {
            digitalWrite(BUZZER, LOW);
            // digitalWrite(TIMER_LED2, LOW);  //SET_PARA_LED

            led.DISPLAY_DIGIT2(24, 24, 24, 24) ;
            led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
          }
          else
          {
            digitalWrite(BUZZER, HIGH);
            DOT2_1 = 0;
            //  digitalWrite(TIMER_LED2, LOW);

            if (!Energy_Mode)
            {
              led.DISPLAY_DIGIT2(12, 12, 17, 24);// ssr
              led.DISPLAY_DIGIT1(21, 13, 25, 26);// FAiL
            }
            else
            {
              // led.DISPLAY_DIGIT2(11, 17, 23, 12); //Pros
              // led.DISPLAY_DIGIT1(12, 16, 23, 11); //Stop
              led.DISPLAY_DIGIT2(12, 12, 17, 24);// ssr
              led.DISPLAY_DIGIT1(21, 13, 25, 26);// FAiL
            }

          }


        }
        if (Probe_error_flag == 1)
        {
          DOT2_1 = 0;
          DOT2_2 = 0;
          digitalWrite( HEATER , LOW);
          digitalWrite(safteyRelay, LOW); //--------Contactor OFF
          digitalWrite(SET_PARA_LED, HIGH);
          digitalWrite(CURRENT_PARA_LED, HIGH);
          digitalWrite(HEATER_STATUS_LED, HIGH);  //
          digitalWrite(TIMER_LED2, LOW);
          digitalWrite(TIMER_LED1, LOW);
          if (SSR_fail_toggle == 0)
          {
            digitalWrite(BUZZER, LOW);
            // digitalWrite(TIMER_LED2, LOW);  //SET_PARA_LED

            led.DISPLAY_DIGIT2(24, 24, 24, 24) ;
            led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
          }
          else
          {
            digitalWrite(BUZZER, HIGH);
            led.DISPLAY_DIGIT2(11, 17, 23, 14) ;
            led.DISPLAY_DIGIT1(15, 17, 23, 17) ;

          }
        }
      }
      break;
    //     }
    case 1 :                     //single press set for temprature 1st and 2nd digit right side display
      //    {
      if (Probe_error_flag == 1)
      {
        ENTER_BUT = 0;
        // ExitFromProcess=1;
      }
      DOT1_1 = 0;
      // digitalWrite(BUZZER, LOW);
      digitalWrite(SET_PARA_LED, LOW);
      digitalWrite(CURRENT_PARA_LED, HIGH);
      //          digitalWrite(CURRENT_PARA_LED, LOW);
      digitalWrite(TIMER_LED1, HIGH);

      if (!Energy_Mode)
      {
        DOT2_1 = 0;
        if (i < VALUE_FOR_BLINK)   //i<25   BLINK_STATS=0
        {
          if (!BLINK_STATS) {
            digits2[0] = 24;
            digits2[1] = 24;
            digits2[2] = 24;
            DOT2_2 = 0;
          }
          else i = VALUE_FOR_BLINK; //i=25
        }
        else
        {
          DOT2_2 = 1; //DOT2_1=1;
          led.BREAK_NUMBER2((SET_VALUE[ENTER_BUT] * 10));
        }
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
        digits2[3] = 10;   //C
      }
      else
      {
        DOT2_1 = 1;
        DOT2_2 = 0;
        if (i < VALUE_FOR_BLINK)   //i<25   BLINK_STATS=0
        {
          if (!BLINK_STATS)
          {
            digits2[2] = 24; digits2[3] = 24;
          }
          else i = VALUE_FOR_BLINK; //i=25
        }
        else
        {
          led.BREAK_NUMBER2((SET_VALUE[6]));
        }

        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
        digits2[0] = 15;   //E
        digits2[1] = 20;   //n

      }
      led.BREAK_NUMBER1(0000);
      break;
    //    }
    case 3 :                                  //single press set for time hour 1st and 2nd digit on right side display
      //    {
      if (Probe_error_flag == 1)
      {
        ENTER_BUT = 0;
        // ExitFromProcess=1;
      }
      digitalWrite(SET_PARA_LED, HIGH);
      digitalWrite(CURRENT_PARA_LED, LOW);
      if (!Energy_Mode)
      {
        led.BREAK_NUMBER2((SET_VALUE[1] * 10));
        digits2[3] = 10;  //C
        DOT2_1 = 0;
        SET_TEMP_DEG = (SET_VALUE[1] );
      }
      else
      {
        DOT2_1 = 1;
        led.BREAK_NUMBER2(SET_VALUE[6] );
        digits2[0] = 15;   //C
        digits2[1] = 20;   //C
        EnergySetValue = SET_VALUE[6];

      }

      if (i < VALUE_FOR_BLINK)
      {
        if (!BLINK_STATS)    {
          digits1[0] = 24;
          digits1[1] = 24;
          DOT1_1 = 0;
        }
        else i = VALUE_FOR_BLINK;
      }
      else
      {
        DOT1_1 = 0;
        led.BREAK_NUMBER1((SET_VALUE[ENTER_BUT] * 100) + SET_VALUE[ENTER_BUT + 1]);
      }
      if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
      // SET_HOUR = SET_VALUE[3];
      break;
    //    }
    case 4 :                                 //SET MINUTE VALUE UPTO 59 MINUTE
      //    {
      if (Probe_error_flag == 1)
      {
        ENTER_BUT = 0;
        // ExitFromProcess=1;
      }
      if (!Energy_Mode)
      {
        led.BREAK_NUMBER2((SET_VALUE[1] * 10));
        digits2[3] = 10;  //C
        DOT2_1 = 0;
        SET_TEMP_DEG = (SET_VALUE[1] );
      }
      else
      {
        DOT2_1 = 1;
        led.BREAK_NUMBER2(EnergySetValue );
        digits2[0] = 15;   //C
        digits2[1] = 20;   //C
      }

      SET_HOUR = SET_VALUE[3];                 //FOR single press set for time MINUTE 3rd and 4th digit on right side displaY
      led.BREAK_NUMBER1( (SET_HOUR * 100) + SET_VALUE[ENTER_BUT] );
      if (i < VALUE_FOR_BLINK)
      {
        if (!BLINK_STATS)    {
          digits1[2] = 24;
          digits1[3] = 24;
        }
        else i = VALUE_FOR_BLINK;
      }
      //          T=0;
      if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
      SET_MIN = (SET_HOUR * 60) + SET_VALUE[4];
      REP1 = 0;
      VENT_FLAG = 1;
      DISP_INC = 0;
      currentMillis = currentMillis1 = 0;
      if ( SET_MIN > 0 ) flag = 0;

      //          COUNTDOWN_START=0;
      break;
    //    }
    case 5:                    //after set the time in hour or minute control is here  start process
      //    {
      if ( SET_MIN == 0) ElapsedTime = 1;
      else             ElapsedTime = 0;

      if (!Energy_Mode)
      {
        if (TEMP_FLAG == 0 && RESUME_SET_TEMP > 0 )
        {
          RESUME_Temp = TEMP_DEG;
          TEMP_FLAG++;
        }
        if ( SET_TEMP_DEG >= 310 ) BUFF = 1;
        else BUFF = 0;

        PID_SETPOINT = SET_TEMP_DEG + BUFF;
      }

      //####################### RESUME TIME LOGIC    ##################

      if (!Energy_Mode)
      {

        EEPROM.write(150, SET_TEMP_DEG >> 8);//Save set rpm value
        EEPROM.write(151, SET_TEMP_DEG & 0xFF);
      }
      else
      {

        EEPROM.write(180, EnergySetValue);

      }
      EEPROM.write(152, SET_HOUR >> 8);//Save set rpm value
      EEPROM.write(153, SET_HOUR & 0xFF);

      EEPROM.write(154, SET_VALUE[4] >> 8);//Save set rpm value
      EEPROM.write(155, SET_VALUE[4] & 0xFF);



      flahDisplay.interval(1000);
      tempRiseCheck.stop();

      //**********************************Contactor ON***********************************************//
      // if(process_end==0)
      digitalWrite(safteyRelay, HIGH);
      EEPROM.write(160, 1);
      //  TIME_SOCK_OR_TOTAL=EEPROM.read(0);
      //  Serial2.println(TIME_SOCK_OR_TOTAL);
      //  TIME_SOCK_OR_TOTAL=EEPROM.read(0);

      while (!ExitFromProcess) //(SET_MIN>0 ) //&& SET_TEMP_DEG>0  )    //&& !PRO_STOP_WITH_LONG_PRESS)
      {
        Inprocess_flag = 1;
        if (Probe_error_flag == 1)
        {
          ENTER_BUT = 0;
          ExitFromProcess = 1;
        }

        if (TEMP_DEG >= 430) //if temp greater than 400 ,it will show SSR fail
        {
          checkTempRaised = 1;
          // SSR_fail_toggle=0;
          // Contactor_off_once_flag=1;
          digitalWrite(safteyRelay, LOW);  //--------if temp raises beyond 480, Contactor will get OFF, heater circuit will be open.
          ENTER_BUT = 0;
          EEPROM.write(160, 0);
          ExitFromProcess = 1;
        }
        else
        {
          checkTempRaised = 0;
        }

        digitalWrite(HEATER_STATUS_LED, LOW);

        Butn.RTD_TEMP_CONV();
        Butn.DISP_UPDATE();
        /*
          //               {
          //               Save_Vent_Data();
          //               if(VENT_VALUE_SET) digitalWrite(VENT, HIGH);
          //               else               digitalWrite(VENT, LOW);
        */

        if (!Energy_Mode)
        {
          if (flag == 0)
          {
            TIME_FACT = 1;
            CURRENT_TIME = SET_MIN;
            RESUME_TEMP = SET_TEMP_DEG;
            flag++;
          }
          if (AUTO_RESUM_SET == 1)
          {
            {
              if ( SET_MIN == (CURRENT_TIME - TIME_FACT) && SET_MIN >= 2 )
              {
                CURRENT_TIME = SET_MIN;
                RESUME_TIME = SET_MIN;
                EEPROM.put(50, RESUME_TIME);
                EEPROM.put(60, RESUME_TEMP);
              }
              else if (SET_MIN == 1 && flag == 1)
              {
                RESUME_TEMP = 0;
                RESUME_TIME = 0;
                EEPROM.put(50, RESUME_TIME);
                EEPROM.put(60, RESUME_TEMP);
                flag++;
              }
            }
          }
          //               digitalWrite(SET_PARA_LED, HIGH); //----------------18012024

          EXECUTE_PID_LOOP = 1;

          if ( RESUME_SET_TEMP > 0 && TEMP_FLAG == 3 )
          {
            ErroR = PID_SETPOINT - TEMP_DEG;
            if (RESUME_SET_TEMP >= 130 && RESUME_SET_TEMP < 180 && ErroR <= 25 && ErroR > 5) DIV_FACT = 3;
            else if (RESUME_SET_TEMP >= 180 && ErroR <= 22 && ErroR > 3) DIV_FACT = 3;
            TEMP_BUFF = RESUME_Temp + ( ErroR / DIV_FACT );
            //                      if (ErroR <= 7 && TEMP_FLAG == 3  ) TEMP_BUFF=0;
            TEMP_FLAG = 4;
          }

          if (RESUME_SET_TEMP >= 30  && RESUME_SET_TEMP <= 310 && ErroR >= ErroRConst )
          {
            if ( TEMP_DEG <= TEMP_BUFF ) {
              digitalWrite(HEATER, HIGH);  //
              Heat = 1;
            }
            else {
              Butn.PID_callingg();
              Heat = 0;
            }
          }
          else {
            Butn.PID_callingg();
            Heat = 0;
          }

          if (ErroR >= 5 && ErroR < 10) ErroRConst = 3;
          else if (ErroR >= 10 && ErroR < 15 ) ErroRConst = 5;
          else if (ErroR >= 20 && ErroR < 25 ) ErroRConst = 7;


          //                    Butn.PID_callingg();
          Butn.Data_Update();
          //                    COUNTDOWN_START=0;

          if (TEMP_DEG >= PID_SETPOINT )
          {
            RESUME_SET_TEMP = 0;
            RESUME_Temp = 0;
            TEMP_BUFF = 0;
            // Butn.BUZZ_INTERVAL2(400);
            // Butn.BUZZ_INTERVAL1(800);
            if (REP == 0)
            {
              REPETATION_FLAG = 1;
              // Butn.BUZZ_TICK_START();
              REP++;
            }
            if (FOR_REP == 2)
            {
              // Butn.BUZZ_TICK_STOP();
              REPETATION_FLAG = 0;
            }
          }
        }
        else
        {
          EXECUTE_PID_LOOP = 0;
        }

        if (!ElapsedTime)
        {
          if (!Energy_Mode)
          {

            if (TIME_SOCK_OR_TOTAL)
            {
              // Serial2.println(2);
              COUNTDOWN_START = 1;    //TIME SOAC OR TOTAL
            }
            else
            {
              //  Serial2.println(3);
              if ( TEMP_DEG >= PID_SETPOINT )
              {
                //  Serial2.println(4);
                COUNTDOWN_START = 1;
                soak_once_flag = 1;
              }
              else if ( TEMP_DEG < PID_SETPOINT  && soak_once_flag == 0)
              {
                // Serial2.println(5);
                COUNTDOWN_START = 0;
              }
            }
          }
          else
          {

            COUNTDOWN_START = 1;
          }
          SET_HOUR = SET_MIN / 60;
          REMAIN_MIN = SET_MIN % 60;
        }
        else
        {
          COUNTDOWN_START = 1;
          SET_HOUR   = ElapsedMinute / 60;
          REMAIN_MIN = ElapsedMinute % 60;
        }



        DISP_INC++;
        //               TIMER_LED_BLINK++;
        //-------------------------------------------------------------------------------------------------------------------------------------------//
        DOT1_1 = 0;
        led.BREAK_NUMBER1( (SET_HOUR * 100) + REMAIN_MIN ); //(SET_HOUR *100)

        // DOT1_2=1;
        // led.BREAK_NUMBER1(TEMP_DEG*10);

        if (VENT_FLAG)
        {
          DOT2_2 = 0;
          DOT1_1 = 0;
          DOT2_1 = 0;
          DOT1_0 = 0;
          CHANGE1 = 0;
          Butn.ALL_LED_OFF();
          digitalWrite(TIMER_LED1, LOW);
          led.DISPLAY_DIGIT2(16, 25, 18, 15) ;  //time  //STIR
          if (!TIME_SOCK_OR_TOTAL)   led.DISPLAY_DIGIT1(12, 23, 13, 10); //soac  if(!SET_VALUE[5])12, 23, 13, 10
          else                      led.DISPLAY_DIGIT1(16, 23, 16, 26);  //total
        }
        else
        {

          CHANGE1 = 1;
          if ( TIMER_LED_BLINK )                  digitalWrite(TIMER_LED1, HIGH);
          else                                   digitalWrite(TIMER_LED1, LOW);

          if (IN_ERROR)  Butn.BUZZ_TICK_STOP();

          if (!Energy_Mode)
          {
            DOT2_1 = 0;
            if (DISP_INC >= 0 && DISP_INC <= 5000)  //SHOW DISPLAY CURRENT TEMPRATURE VALUES AND SET TEMPRATURE VALUE  if(DISP_INC>=0 && DISP_INC<=3000)
            {
              digitalWrite(CURRENT_PARA_LED, HIGH);
              digitalWrite(SET_PARA_LED, LOW);             //SHOWING SET TEMP VALUES

              led.BREAK_NUMBER2((PID_SETPOINT - BUFF) * 10); //(SET_TEMP_DEG*100);  (PID_SETPOINT*100)
              //                      digits2[3] = 10;  //C
              DOT2_2 = 1; //DOT2_1=0;


            }
            else if (DISP_INC > 5000 && DISP_INC <= 10000)  //SHOW CURRENT TEMP VALUES   (DISP_INC>3000 && DISP_INC<=6000)
            {
              digitalWrite(CURRENT_PARA_LED, LOW);
              digitalWrite(SET_PARA_LED, HIGH);

              led.BREAK_NUMBER2(TEMP_DEG * 10);  DOT2_2 = 1;
              //                      digits2[3] = 10;  //C

            }
            else DISP_INC = 0;
          }
          else
          {
            digitalWrite(CURRENT_PARA_LED, HIGH);
            digitalWrite(SET_PARA_LED, LOW);
            DOT2_1 = 1;
            led.BREAK_NUMBER2(EnergySetValue );
            digits2[0] = 15;   //C
            digits2[1] = 20;   //C
            DOT2_2 = 0;

          }
        }
        //               }
        currentMillis = currentMillis1 = 0;
      }
      soak_once_flag = 0;
      process_end = 1;
      Inprocess_flag = 0;
      digitalWrite(safteyRelay, LOW);
      EEPROM.write(160, 0);

      //  EEPROM_AUTO_RESUM_SET=0;
      Inprocess_autoresume_flag = 0;
      //  ENTER_BUT=0;
      while (STOP_PROCESS < 1)
      {

        //  digitalWrite(safteyRelay, LOW);
        // DISP.update();
        // digitalWrite(SET_PARA_LED, HIGH);
        // digitalWrite(CURRENT_PARA_LED, HIGH);
        // digitalWrite(HEATER_STATUS_LED, HIGH);  //
        Butn.DISPLAY_UPDATE();
        digitalWrite(BUZZER, HIGH);   // turn the LED on (HIGH is the voltage level)
        if (currentMillis >= 40)
        {
          LED_STATE = !LED_STATE;
          currentMillis = 0;
          if (currentMillis1 > 120) {
            STOP_PROCESS++;
            currentMillis = 0;
            currentMillis1 = 0;
          }
        }
        if (LED_STATE)   digitalWrite(ALARM_LED, HIGH);
        else            digitalWrite(ALARM_LED, LOW);
        digitalWrite(ALARM_LED, LED_STATE);   //RED LED
      }

      digitalWrite(BUZZER, LOW);

      PID_SETPOINT = 0;
      EXECUTE_PID_LOOP = 0;
      // EEPROM.write(160, 0);

      Flag == 0;
      ENTER_BUT = 0;
      //            Butn.BUZZ_INTERVAL(200);
      break;
    default:
      break;
      //  }
  }
}


void Pclass :: SET_LONG_PRESS_FUNCTION()   //mode buttone long press and enter = mode single press
{
  digitalWrite(TIMER_LED1, LOW);  //LOW
  if (MODE_BUT_SINGLE_PRESS_IN_SETTINGS) //for display and else for set   MODE_BUT_SINGLE_PRESS_IN_SETTINGS=mode button single in long press function
  {
    switch (SELECTION_PARAMETER)
    {
      case Time_Mode  :                                           //  TIME SELECTION  - TOTAL OR SOAC
        DOT1_1 = 0;
        DOT2_1 = 0;
        DOT1_0 = 0;
        DOT2_2 = 0;
        Butn.ALL_LED_OFF();
        led.DISPLAY_DIGIT2(16, 25, 18, 15) ;
        //i++;
        if (i < VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
        else
        {
          if (!SET_LONG_VALUE[SELECTION_PARAMETER])   led.DISPLAY_DIGIT1(12, 23, 13, 10); // soac        led.DISPLAY_DIGIT1(16, 23, 16, 26);  //off   (0, 21, 21, 24)
          else                                       led.DISPLAY_DIGIT1(16, 23, 16, 26);// total       led.DISPLAY_DIGIT1(12, 23, 13, 10);  //on    (0, 20, 24, 24)
        }
        if (i > TOTAL_VALUE_FOR_BLINK)    i = 0;
        break;

      case Auto_Resume_Enable     :                                           //AUTO RESUME - ON OR OFF
        Butn.ALL_LED_OFF();
        DOT1_1 = 0;
        DOT2_1 = 0;
        DOT2_2 = 0;
        led.DISPLAY_DIGIT2(17, 15, 12, 34) ;         //rsum
        //                i++;
        if (i < VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
        else
        {
          if (!SET_LONG_VALUE[SELECTION_PARAMETER])      led.DISPLAY_DIGIT1(0, 21, 21, 24); //off    //0c    (29, 10, 24, 24)
          else                                          led.DISPLAY_DIGIT1(0, 20, 24, 24);  //on     //0f    (29, 21, 24, 24)
        }
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
        break;

      case Safe_Value_ON_OFF  :                                                     //SAFE VALUE SET - ON OR OFF
        Butn.ALL_LED_OFF();
        DOT1_1 = 0;
        DOT2_1 = 0;
        DOT2_2 = 0;
        led.DISPLAY_DIGIT2(12, 13, 21, 15) ;                  //safe
        //                i++;
        if (i < VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
        else
        {
          if (SET_LONG_VALUE[SELECTION_PARAMETER] == 0)   led.DISPLAY_DIGIT1(0, 21, 21, 24); //off    //0c    (29, 10, 24, 24)
          else                                          led.DISPLAY_DIGIT1(0, 20, 24, 24);  //on     //0f    (29, 21, 24, 24)
        }
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
        break;

      case Calibration_Selection  :                                                    //CALB - CAL1 OR CAL2
        DOT1_1 = 0;
        DOT2_2 = 0;
        Butn.ALL_LED_OFF();
        led.DISPLAY_DIGIT2(10, 13, 26, 14) ;                 //calb
        //i++;
        if (i < VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
        else
        {
          if (SET_LONG_VALUE[SELECTION_PARAMETER])   led.DISPLAY_DIGIT1(10, 13, 26, 2) ; //cal2
          else                                      led.DISPLAY_DIGIT1(10, 13, 26, 1) ;  //cal1
        }
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
        break;

      case OnePointCalibration  :                                                            //CAL1
        DOT1_0 = 0;
        DOT2_2 = 0;
        Butn.ALL_LED_OFF();
        led.DISPLAY_DIGIT2(10, 13, 26, 1) ;                         //cal1
        //i++;
        if (i < VALUE_FOR_BLINK)
        {
          if (!BLINK_STATS)    {
            digits1[0] = 24;
            digits1[1] = 24;
            digits1[2] = 24;
          }
          else i = VALUE_FOR_BLINK;
        }
        else  led.BREAK_NUMBER1((SET_LONG_VALUE[4] * 10)); //+ ( SET_LONG_VALUE[5] * 10));   SET CAL1 VALUE
        digits1[3] = 10;
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
        break;

      case Cal2LowTemp_Set_Display  :                                                                      //cal2-L
        DOT1_0 = 1;
        DOT1_1 = 0;
        DOT2_2 = 0;
        Butn.ALL_LED_OFF();
        CAL2_START = 1;
        led.DISPLAY_DIGIT2(10, 13, 26, 2) ;
        //i++;
        if (i < VALUE_FOR_BLINK)
        {
          if (!BLINK_STATS) led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
          else i = VALUE_FOR_BLINK;
        }
        else
        {
          led.BREAK_NUMBER1(SET_LONG_VALUE[SELECTION_PARAMETER] * 1);
        }
        digits1[0] = 26;
        //               digits1[3] = 10;
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
        break;
      //               SET_LONG_VALUE[7] = SET_LONG_VALUE[6] + 10;

      case Cal2HighTemp_Set_Display  :                                                               //cal2-H
        Butn.ALL_LED_OFF();
        led.DISPLAY_DIGIT2(10, 13, 26, 2) ;
        //i++;
        if (i < VALUE_FOR_BLINK)
        {
          if (!BLINK_STATS)   led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
          else i = VALUE_FOR_BLINK;
        }
        else
        {
          led.BREAK_NUMBER1(SET_LONG_VALUE[SELECTION_PARAMETER] * 1);
        }
        digits1[0] = 30;
        //               digits1[3] = 10; ;
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
        break;

      case Cal2LowTemp_Enter_Heat :                                                           //Inprocess-Entering value
        Butn.ALL_LED_OFF();
        DOT1_2 = 0;
        DOT1_1 = 0;                                                 //start heating at low cal value
        DOT2_0 = 1;
        led.BREAK_NUMBER2(LOW_CAL_VALUE_SET);
        digits2[0] = 26;     //LO
        //              digits2[1] = 23;
        //i++;
        if (i < VALUE_FOR_BLINK)
        {
          if (!BLINK_STATS)  {
            digits1[0] = 24;
            digits1[1] = 24;
            digits1[2] = 24;
          }
          else i = VALUE_FOR_BLINK;
        }
        else
        {
          led.BREAK_NUMBER1((SET_LONG_VALUE[SELECTION_PARAMETER] * 10)); // + (SET_LONG_VALUE[SELECTION_PARAMETER+1] *10));

        }
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
        digits1[3] = 10;
        CAL2_CALCULATED_LOW_CAL_VALUE  = ((SET_LONG_VALUE[SELECTION_PARAMETER]));
        break;

      case Cal2LowTemp_Enter_Maintain :                                                           //Inprocess-Entering value decimal
        Butn.ALL_LED_OFF();
        DOT1_2 = 0;
        DOT1_1 = 0;                                                 //start heating at low cal value
        DOT2_0 = 1;
        led.BREAK_NUMBER2(LOW_CAL_VALUE_SET);                       //ask to user enter actual value at low cal value
        digits2[0] = 26;     //LO
        //              digits2[1] = 23;
        //i++;
        if (i < VALUE_FOR_BLINK)
        {
          if (!BLINK_STATS) {
            digits1[0] = 24;
            digits1[1] = 24;
            digits1[2] = 24;
          }
          else i = VALUE_FOR_BLINK;
        }
        else
        {
          led.BREAK_NUMBER1((SET_LONG_VALUE[SELECTION_PARAMETER] * 10)); // + (SET_LONG_VALUE[SELECTION_PARAMETER] *10));
        }
        digits1[3] = 10;
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;

        CAL2_CALCULATED_LOW_CAL_VALUE  = ((SET_LONG_VALUE[SELECTION_PARAMETER]));// ) + (SET_LONG_VALUE[9] / 10.0));   //f;

        break;
      case Cal2HighTemp_Enter_Heat :                                                          //Inprocess-Entering value High
        Butn.ALL_LED_OFF();
        led.BREAK_NUMBER2(HIGH_CAL_VALUE_SET);                     //heating state
        digits2[0] = 30;     //HI
        //              digits2[1] = 25;
        DOT1_2 = 0;
        DOT1_1 = 0;
        DOT2_0 = 1;
        //i++;
        if (i < VALUE_FOR_BLINK)
        {
          if (!BLINK_STATS)  {
            digits1[0] = 24;
            digits1[1] = 24;
            digits1[2] = 24;
          }
          else i = VALUE_FOR_BLINK;
        }
        else
        {
          led.BREAK_NUMBER1((SET_LONG_VALUE[SELECTION_PARAMETER] * 10)); // + (SET_LONG_VALUE[SELECTION_PARAMETER+1] *10));
        }
        digits1[3] = 10;
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
        CAL2_CALCULATED_HIGH_CAL_VALUE = ((SET_LONG_VALUE[SELECTION_PARAMETER]));
        break;

      case Cal2HighTemp_Enter_Maintain :                                                            //Inprocess-Entering value High
        Butn.ALL_LED_OFF();
        led.BREAK_NUMBER2(HIGH_CAL_VALUE_SET);                       //ask to user enter high cal value
        digits2[0] = 30;      //HI
        //              digits2[1] = 25;
        DOT1_2 = 0;
        DOT1_1 = 0;
        DOT2_0 = 1;
        //i++;
        if (i < VALUE_FOR_BLINK)
        {
          if (!BLINK_STATS)  {
            digits1[0] = 24;
            digits1[1] = 24;
            digits1[2] = 24;
          }
          else i = VALUE_FOR_BLINK;
        }
        else
        {
          led.BREAK_NUMBER1((SET_LONG_VALUE[SELECTION_PARAMETER] * 10)); // + (SET_LONG_VALUE[SELECTION_PARAMETER] *10));
        }
        digits1[3] = 10;
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;

        CAL2_CALCULATED_HIGH_CAL_VALUE = ((SET_LONG_VALUE[SELECTION_PARAMETER]));// -1]) + (SET_LONG_VALUE[SELECTION_PARAMETER] / 10.0)); //f;
        break;

      case Cal2_Complete_Display:
        LONG_PRESS = 0;
        break;


      case Energy_Mode_Selection :
        DOT1_0 = 0;
        DOT1_1 = 0;
        DOT2_1 = 0;
        DOT2_2 = 0;
        Butn.ALL_LED_OFF();
        led.DISPLAY_DIGIT2(15, 20, 17, 27) ;  //Enrg
        //i++;
        if (i < VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
        else
        {
          if (SET_LONG_VALUE[SELECTION_PARAMETER])    led.DISPLAY_DIGIT1(15, 20, 14, 26); // EnbL
          else                                       led.DISPLAY_DIGIT1(32, 12, 14, 26);// dSbL
        }
        if (i > TOTAL_VALUE_FOR_BLINK)    i = 0;
        break;

      case FactoryAndSure_Reset:
        DOT1_0 = 0;
        DOT1_1 = 0;
        DOT2_1 = 0;
        DOT2_2 = 0;
        Butn.ALL_LED_OFF();
        led.DISPLAY_DIGIT2(12, 28, 17, 15) ;  //sure
        led.DISPLAY_DIGIT1(17, 15, 12, 16) ;  //reset
        currentMillis = 0;
        break;

      case ResetDone:
        DOT1_0 = 0;
        DOT1_1 = 0;
        DOT2_1 = 0;
        DOT2_2 = 0;
        Butn.ALL_LED_OFF();
        //               Butn.EEPROM_CLEAR();
        if (currentMillis >= 0 && currentMillis <= 30)
        {
          digitalWrite(BUZZER, HIGH);
          led.DISPLAY_DIGIT2(22, 22, 22, 22) ;  //----
          led.DISPLAY_DIGIT1(22, 22, 22, 22) ;  //----
        }
        else if (currentMillis > 40  &&  currentMillis <= 170)
        {
          digitalWrite(BUZZER, LOW);
          led.DISPLAY_DIGIT2(17, 15, 12, 16)  ;  //rest
          led.DISPLAY_DIGIT1(32, 23, 20, 15) ;   //done
          //  EEPROM.put(65, 300);
        }
        else  if (currentMillis > 170)
        {
          Butn.EEPROM_CLEAR();
        }
        break;
      //             {
      //       case 15  :                                                     //vent open time
      //                Butn.ALL_LED_OFF();
      //                DOT1_0=0;
      //                DOT2_2=0;
      //                led.DISPLAY_DIGIT2(34, 15, 20, 16) ;      //open
      //                //i++;
      //                if(i<VALUE_FOR_BLINK)
      //                {
      //                  if(!BLINK_STATS)    {led.DISPLAY_DIGIT1(16, 24, 24, 24) ; DOT1_0=0;}
      //                  else i=VALUE_FOR_BLINK;
      //                }
      //                else
      //                {
      //                    led.BREAK_NUMBER1(SET_LONG_VALUE[SELECTION_PARAMETER] * 1);
      //
      //                }
      //                DOT1_0=1;
      //                digits1[0] = 16;
      //                digits1[1] = 24;
      //                if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
      //                break;
      //             }

      case SafeValue   :                                                     //safe value
        Butn.ALL_LED_OFF();
        DOT1_1 = 0;
        DOT2_2 = 0;
        led.DISPLAY_DIGIT2(12, 13, 21, 15) ;     //safe
        //i++;
        if (i < VALUE_FOR_BLINK)
        {
          if (!BLINK_STATS)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
          else i = VALUE_FOR_BLINK;
        }
        else
        {
          led.BREAK_NUMBER1(SET_LONG_VALUE[SELECTION_PARAMETER] * 10);
        }
        digits1[3] = 10;
        if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
        break;

      default:
        break;
    }
  }
  else                                           //without pressing mode button single it show EEPROM values
  {
    switch (SELECTION_PARAMETER)
    {
      case Time_Mode  :                                //TIME SELECTION
        DOT1_1 = 0;
        DOT2_1 = 0;
        DOT1_0 = 0;
        DOT2_2 = 0;
        Butn.BUZZ_TICK_STOP();
        TIME_SOCK_OR_TOTAL = EEPROM.read(0);
        led.DISPLAY_DIGIT2(16, 25, 18, 15) ;     //time
        if (!TIME_SOCK_OR_TOTAL)   led.DISPLAY_DIGIT1(12, 23, 13, 10);   //soac
        else                      led.DISPLAY_DIGIT1(16, 23, 16, 26);    //totL
        break;
      case Auto_Resume_Enable  :                                 //AUTO RESUME
        Butn.BUZZ_TICK_STOP();
        DOT1_1 = 0;
        DOT2_1 = 0;
        DOT1_0 = 0;
        DOT2_2 = 0;
        AUTO_RESUM_SET = EEPROM.read(55);
        led.DISPLAY_DIGIT2(17, 15, 12, 34) ; //rsum
        if (!AUTO_RESUM_SET)      led.DISPLAY_DIGIT1(0, 21, 21, 24); //off
        else                     led.DISPLAY_DIGIT1(0, 20, 24, 24); //on
        break;

      case Safe_Value_ON_OFF  :                                  //SAFE VALUE SELECTION
        Butn.BUZZ_TICK_STOP();
        DOT1_1 = 0;
        DOT2_1 = 0;
        DOT1_0 = 0;
        DOT2_2 = 0;
        led.DISPLAY_DIGIT2(12, 13, 21, 15) ; //safe
        SAFE_VALUE_SET_DEG =   EEPROM.read(2);
        if (!SAFE_VALUE_SET_DEG)      led.DISPLAY_DIGIT1(0, 21, 21, 24); //off
        else                         led.DISPLAY_DIGIT1(0, 20, 24, 24); //on
        break;

      case Calibration_Selection  :                                                 //CALIBRATION SELECTION
        DOT1_1 = 0;
        DOT1_0 = 0;
        DOT2_2 = 0;
        CAL_SELECT_VALUE = EEPROM.read(5);
        Butn.BUZZ_TICK_STOP();
        led.DISPLAY_DIGIT2(10, 13, 26, 14) ;
        if (CAL_SELECT_VALUE)   led.DISPLAY_DIGIT1(10, 13, 26, 2) ; //cal2
        else                   led.DISPLAY_DIGIT1(10, 13, 26, 1) ;  //cal1
        break;

      case OnePointCalibration  :                                                   //cal1 - first two digit
        DOT1_0 = 0;
        DOT2_2 = 0;
        Butn.BUZZ_TICK_STOP();
        led.DISPLAY_DIGIT2(10, 13, 26, 1) ;
        led.BREAK_NUMBER1(TEMP_DEG_RAW * 10);   DOT1_1 = 0;
        digits1[3] = 10;

        break;

      case Cal2LowTemp_Set_Display  :  //cal2-L                                                                        // CAL1
        DOT1_0 = 1;
        DOT1_1 = 0;
        DOT2_2 = 0;
        Butn.BUZZ_TICK_STOP();
        led.DISPLAY_DIGIT2(10, 13, 26, 2);
        led.BREAK_NUMBER1(LOW_CAL_VALUE_SET * 1);
        digits1[0] = 26;
        //               digits1[3] = 10;
        break;

      case Cal2HighTemp_Set_Display  :  //cal2-H
        Butn.BUZZ_TICK_STOP();
        led.DISPLAY_DIGIT2(10, 13, 26, 2) ;
        led.BREAK_NUMBER1(HIGH_CAL_VALUE_SET * 1);
        //               digits1[0] = 30;
        digits1[3] = 10;
        break;

      case Cal2LowTemp_Enter_Heat  :                                               //cal2-strt process heat
        DOT1_0 = 0;
        DOT2_0 = 1;
        DOT1_2 = 0;
        DOT1_1 = 0;
        DOT2_2 = 0;
        Butn.BUZZ_TICK_STOP();
        PID_SETPOINT = SET_LONG_VALUE[6];
        digitalWrite(safteyRelay, HIGH);
        while ((PID_INPUT < PID_SETPOINT) && RST_BUTT)
        {
          Butn.RTD_TEMP_CONV();
          PID_SETPOINT = SET_LONG_VALUE[6];
          EXECUTE_PID_LOOP = 1;
          Butn.PID_callingg();
          digitalWrite(HEATER_STATUS_LED, LOW);
          Butn.DISP_UPDATE();
          if (i < VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
          else                     led.DISPLAY_DIGIT1(30, 15, 13, 16) ;    //heat
          if (i > TOTAL_VALUE_FOR_BLINK)    i = 0;
          led.BREAK_NUMBER2(LOW_CAL_VALUE_SET);
          digits2[0] = 26;
          //                digits2[1] = 23;
        }
        digitalWrite(safteyRelay, LOW);
        SET_MIN = 30;
        currentMillis1 = 0;
        COUNTDOWN_START = 1;
        SELECTION_PARAMETER++;
        break;

      case  Cal2LowTemp_Enter_Maintain  :
        digitalWrite(safteyRelay, HIGH);
        while ((!MODE_BUT_SINGLE_PRESS_IN_SETTINGS) && RST_BUTT  && COUNTDOWN_START)
        {
          Butn.RTD_TEMP_CONV();
          PID_SETPOINT = SET_LONG_VALUE[6];
          Butn.DISP_UPDATE();
          EXECUTE_PID_LOOP = 1;
          Butn.PID_callingg();
          digitalWrite(ALARM_LED, LOW);
          digitalWrite(HEATER_STATUS_LED, HIGH);
          led.BREAK_NUMBER2(LOW_CAL_VALUE_SET);
          digits2[0] = 26;
          //                digits2[1] = 23;
          DOT2_0 = 1;
          DOT1_2 = 0;
          if (currentMillis1 > 0 && currentMillis1 < 100 )  digitalWrite(BUZZER, HIGH);
          else if (currentMillis1 >= 100 && currentMillis1 < 200 )  digitalWrite(BUZZER, LOW);
          if (currentMillis1 > 200) currentMillis1 = 0;
          if (i < VALUE_FOR_BLINK)
          {
            led.DISPLAY_DIGIT1(24, 24, 24, 24) ;

          }
          else
          {
            led.BREAK_NUMBER1(TEMP_DEG_RAW * 10) ;
            //                  if(TEMP_DEG>99.9)  { led.BREAK_NUMBER1(TEMP_DEG_RAW*10); DOT1_1=0; }
            //                  else               { led.BREAK_NUMBER1(TEMP_DEG_RAW*100); DOT1_1=1; }
            //                  DOT1_2=1;

          }
          if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
          digits1[3] = 10;
        }
        digitalWrite(safteyRelay, LOW);
        if (COUNTDOWN_START)
        {
          SELECTION_PARAMETER =  Cal2LowTemp_Enter_Heat ; //8;
        }
        else
        {
          SELECTION_PARAMETER = Cal2HighTemp_Enter_Heat;  //10
          MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 0;
        }
        break;

      case Cal2HighTemp_Enter_Heat :                                               //cal2-strt
        DOT1_0 = 0;
        DOT1_1 = 0;
        DOT1_2 = 0;
        DOT2_0 = 1;
        DOT2_2 = 0;
        PID_SETPOINT = SET_LONG_VALUE[7];
        digitalWrite(safteyRelay, HIGH);
        while ((PID_INPUT < PID_SETPOINT) && RST_BUTT  )
        {
          Butn.RTD_TEMP_CONV();
          PID_SETPOINT = SET_LONG_VALUE[7];
          EXECUTE_PID_LOOP = 1;
          Butn.PID_callingg();
          Butn.DISP_UPDATE();
          digitalWrite(HEATER_STATUS_LED, LOW);
          if (i < VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
          else          led.DISPLAY_DIGIT1(30, 15, 13, 16) ;
          if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
          led.BREAK_NUMBER2(HIGH_CAL_VALUE_SET);
          digits2[0] = 30;   //H
          //                digits2[1] = 25;
        }
        digitalWrite(safteyRelay, LOW);
        currentMillis1 = 0;
        SET_MIN = 30;
        COUNTDOWN_START = 1;
        SELECTION_PARAMETER++;
        break;
      case Cal2HighTemp_Enter_Maintain :                                                // maintain temp till user interupt for entering temp
        digitalWrite(safteyRelay, HIGH);
        while ((!MODE_BUT_SINGLE_PRESS_IN_SETTINGS) && RST_BUTT && COUNTDOWN_START )
        {
          Butn.RTD_TEMP_CONV();
          PID_SETPOINT = SET_LONG_VALUE[7];
          EXECUTE_PID_LOOP = 1;
          Butn.PID_callingg();
          Butn.DISP_UPDATE();
          digitalWrite(ALARM_LED, LOW);
          digitalWrite(HEATER_STATUS_LED, HIGH);
          led.BREAK_NUMBER2(HIGH_CAL_VALUE_SET);
          digits2[0] = 30;
          //                digits2[1] = 25;
          DOT2_2 = 0;

          if (currentMillis1 > 0 && currentMillis1 < 100 )  digitalWrite(BUZZER, HIGH);
          else if (currentMillis1 >= 100 && currentMillis1 < 200 )  digitalWrite(BUZZER, LOW);
          if (currentMillis1 > 200) currentMillis1 = 0;
          if (i < VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
          else
          {
            led.BREAK_NUMBER1(TEMP_DEG_RAW * 10) ;
            //                  DOT1_2=1;
            //                  if(TEMP_DEG>99.9)  { led.BREAK_NUMBER1(TEMP_DEG_RAW*10); DOT1_1=0; }
            //                  else               { led.BREAK_NUMBER1(TEMP_DEG_RAW*100);DOT1_1=1; }

            //                  DOT1_1=1;
          }
          if (i > TOTAL_VALUE_FOR_BLINK)   i = 0;
          digits1[3] = 10;
        }
        digitalWrite(safteyRelay, LOW);
        PID_SETPOINT = 0;
        EXECUTE_PID_LOOP = 0;
        if (COUNTDOWN_START)
        {
          SELECTION_PARAMETER = Cal2HighTemp_Enter_Heat;
        }
        else
        {
          LONG_PRESS = 0;
          MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 0;
        }
        break;
      case Cal2_Complete_Display:
        DOT2_1 = 0;
        DOT1_1 = 0;
        DOT1_0 = 0;
        DOT2_2 = 0;
        DOT2_0 = 0;
        if (i < TOTAL_VALUE_FOR_BLINK + 150)
        {
          led.DISPLAY_DIGIT2(10, 13, 26, 14) ;   //calb
          led.DISPLAY_DIGIT1(32, 23, 20, 19) ;   //done  (12, 13, 28, 15)
        }
        else
        {

          MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 0;
          LONG_PRESS = 0;
        }
        break;

      case Energy_Mode_Selection :
        DOT1_0 = 0;
        DOT1_1 = 0;
        DOT2_1 = 0;
        DOT2_2 = 0;
        Butn.ALL_LED_OFF();
        led.DISPLAY_DIGIT2(15, 20, 17, 27) ;  //Enrg
        led.DISPLAY_DIGIT1(18, 23, 32, 15) ;  //mode
        currentMillis = 0;
        break;

      case FactoryAndSure_Reset:
        DOT1_0 = 0;
        DOT1_1 = 0;
        DOT2_1 = 0;
        DOT2_2 = 0;
        Butn.BUZZ_TICK_STOP();
        led.DISPLAY_DIGIT2(21, 13, 10, 16) ;  //fact
        led.DISPLAY_DIGIT1(17, 15, 12, 16) ;  //rest
        break;
        {
          //       case 15  :  //vent open time                                                                       // CAL1
          //               DOT1_0=1;
          //               DOT1_1=0;
          //               DOT2_2=0;
          //               Butn.BUZZ_TICK_STOP();
          //               led.DISPLAY_DIGIT2(34, 15, 20, 16);   //open
          //               led.BREAK_NUMBER1(VENT_TIME_SET);    //VENT_TIME_SET  SET_LONG_VALUE[SELECTION_PARAMETER] * 1
          //               digits1[0] = 16;
          //               digits1[1] = 24;
          //               break;
        }
      case SafeValue  :  //safe value set                                                              // CAL1
        DOT1_0 = 0;
        DOT1_1 = 0;
        DOT2_2 = 0;

        Butn.BUZZ_TICK_STOP();
        //  SAFE_VALUE_SET=EEPROM.read(65);
        // EEPROM.get(65, SAFE_VALUE_SET);
        SAFE_VALUE_SET  = (EEPROM.read(200) << 8) + EEPROM.read(201);
        //  Serial2.println(SAFE_VALUE_SET);
        led.DISPLAY_DIGIT2(12, 13, 21, 15);    //SAFE
        led.BREAK_NUMBER1(SAFE_VALUE_SET * 10);
        digits1[3] = 10;

        //               if(!SAFE_VALUE_SET_DEG)   led.DISPLAY_DIGIT1(0, 21, 21, 24);    //off
        //               else                      led.DISPLAY_DIGIT1(0, 20, 24, 24);    //on
        break;
      default:
        break;
    }
  }
}



Pclass MainProcess = Pclass();
