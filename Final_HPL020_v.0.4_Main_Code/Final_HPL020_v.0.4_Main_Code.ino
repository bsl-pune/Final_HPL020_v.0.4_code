#include "Arduino.h"
#include "Ext_var.h"
#include <EEPROM.h>

long data = 0; //check github
void AUTO_UPDATE();
void DATA_UPDATE();
void DISP_REFRESH();
void PID_LOOP();
void STOP_BUZZER();
void RTD_TEMP_CAL();
void STOP_BUZZER1();
void STOP_BUZZER2();
void CHANGE_FLAG();
Ticker BUZZ(STOP_BUZZER, 100, 1);
Ticker Toggle1(CHANGE_FLAG, 1000, 0);
Ticker PID_EXE(PID_LOOP, 400, 0);
Ticker TEMP_UPDATE(RTD_TEMP_CAL, 100, 0);
Ticker BUZZ_TICK1(STOP_BUZZER1, 2000, 0);
Ticker BUZZ_TICK2(STOP_BUZZER2, 1000, 1);
Ticker DISP(DISP_REFRESH, 4000, 0, MICROS_MICROS);
//Ticker DISP(DISP_REFRESH, 5, 0,MILLIS);
Ticker AUTO(AUTO_UPDATE, 1000, 0);
Ticker DATA_PRINT(DATA_UPDATE, 1000, 0);
//*********************************************temp**********************************************
int serialtime = 0;
float TEMP_DEG_RAW = 25.5 , VOUT_1 = 0.0, TEMP_DEG_RAW1 = 0.0, TEMP_FAH_RAW = 0.0; //0.0
float TEMP_FAH;
float TEMP_DEG = 25.5,  z = 0;
//------------------------------------------------------------------------------------------------
// #define Rb 100.0
// #define Ra 1500 //120//470.0   for old pcb 4700   for new pcb 400
// #define Rc 1500 //120//470.0   for old pcb 4700   for new pcb 400
// #define Vs 5

#define Rb 100.0
#define Ra 4700 //120//470.0   for old pcb 4700   for new pcb 400
#define Rc 4700 //120//470.0   for old pcb 4700   for new pcb 400   
#define Vs 5

#define Vb (Rb*Vs)/(Rb+Ra)
#define Alpha  0.00385
float R1 = 100.0;
float R2 = 4700.0;
float R3 = 4700.0;
float Vin = 0, Vin1 = 0, Rt = 0, Rt1 = 0, Rx, TEMP_UPDATETD;
float  TotalVout = 0, AvgVout = 0 , AvgVout1 = 0  , TotalVout1 = 0;
int avrage_val = 1, avrage_val1 = 1, sample, sample1;
//*************************************************Button***************************************
OneButton button1(MODE_KEY,  true);
OneButton button2(UP_KEY,    true);
OneButton button3(DOWN_KEY,  true);
OneButton button4(RST_KEY,   true);
int8_t ENTER_BUT = 0, MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 0, LONG_PRESS = 0, INC5 = 0, TIME_WASTE_FLAG = 0;
//bool SET_KEY_PRESS=0, SINGLE_SWITCH_PRESS=0;
//int Ki_saved=0;
//***********************************************************************************************
float  TEMPERARY_SAVED_VARIABLE_FOR_CAL1_FAH, TEMP_CAL2_RAW_LOW_VALUE, TEMP_CAL2_RAW_HIGH_VALUE;
int TEMPERARY_SAVED_VARIABLE_FOR_CAL1_DEG;
//***********************************************************************************************
bool ERROR_SET_IN_DEG_FLAG = 0, ERROR_SET_IN_FAH_FLAG = 0;
bool VENT_FLAG = 0;
bool SIGN = 0;
float TEMP_DEG_RAW2, TEMP_FAH_RAW2;
int FOR_REP = 0, FOR_REP2 = 0,  REP = 0, REP1 = 0, p = 0, fast_inc = 0;
long PRES = 0, lup = 0;

bool EEPROM_AUTO_RESUM_SET = 0;


void setup()
{
  Serial2.swap(1);
  Serial2.pins(38, 39);
  Serial2.begin(9600);
  lup = 0;
  RESUME_SET_TEMP = 0;
  TEMP_BUFF = 0;
  RESUME_Temp = 0;
  TEMP_FLAG = 0;
  ErroR = 0;
  AUTO.start();
  DATA_PRINT.start();
  REPET = 0;
  led.SETUP();
  Proces.SETUP();
  Butn.SETUP();
  K_type.Setup();
  //  DISP.start();
  TEMP_UPDATE.start();
  PID_EXE.start();
  Toggle1.start();
  pinMode(MODE_KEY, INPUT_PULLUP);
  pinMode(UP_KEY,   INPUT_PULLUP);
  pinMode(DOWN_KEY, INPUT_PULLUP);
  pinMode(RST_KEY,  INPUT_PULLUP);
  //  pinMode(SAFETY_RTD, INPUT);


  button1.attachClick(MODE_CLICK);  //mode single press
  button1.attachLongPressStart(MODE_LONG_PRESS_START);
  button1.attachLongPressStop(MODE_LONG_PRESS_STOP);

  button2.attachClick(INC_CLICK);
  button2.attachLongPressStart(INC_LONG_PRESS_START);
  button2.attachDuringLongPress(INC_LONGPRESS);
  button2.attachLongPressStop(INC_LONGPRESS_STOP);

  button3.attachLongPressStart(DEC_LONG_PRESS_START);
  button3.attachClick(DEC_CLICK);
  button3.attachDuringLongPress(DEC_LONGPRESS);
  button3.attachLongPressStop(DEC_LONGPRESS_STOP);

  button4.attachClick(RESET_CLICK);
  button4.attachLongPressStart(RESET_START);
  button4.attachLongPressStop(RESET_STOP);

  SET_TEMP_DEG = (EEPROM.read(150) << 8) + EEPROM.read(151);
  EnergySetValue = EEPROM.read(180);
  SET_VALUE[3]  = (EEPROM.read(152) << 8) + EEPROM.read(153);
  SET_VALUE[4] = (EEPROM.read(154) << 8) + EEPROM.read(155);
  SET_VALUE[1] = SET_TEMP_DEG;
  // SET_VALUE[3]=SET_HOUR;
  // SET_VALUE[3] = SET_HOUR;
  // SET_VALUE[4] = SET_MIN;




  //  Butn.SETUP();
  //  TIME_SOCK_OR_TOTAL = EEPROM.read(0);
  //  ifTIME_SOCK_OR_TOTAL = 1 ;
  EEPROM_AUTO_RESUM_SET = EEPROM.read(55);  // AUTO_RESUM_SET=EEPROM.read(55);
  AUTO_RESUM_SET = EEPROM_AUTO_RESUM_SET;
  // AUTO_RESUM_SET = 0;
  //  RESUME_TIME  =   EEPROM.get(50);
  //  RESUME_TEMP  =   EEPROM.get(60);
  EEPROM.get(50, RESUME_TIME);  //120
  EEPROM.get(60, RESUME_TEMP);
  // EEPROM.get(65, SAFE_VALUE_SET);
  SAFE_VALUE_SET  = (EEPROM.read(200) << 8) + EEPROM.read(201);

  //  EEPROM.get(55, AUTO_RESUM_SET);
  EEPROM.get(0, TIME_SOCK_OR_TOTAL);
  //  TIME_SOCK_OR_TOTAL = 1;
  //  PRINTER_VALUE_SET   =   EEPROM.read(1);
  SAFE_VALUE_SET_DEG =   EEPROM.read(2);
  Energy_Mode =   EEPROM.read(15);

  //  VENT_TIME_SET =   EEPROM.read(30) ;
  //  EEPROM.get(3, SAFE_VALUE_SET_FAH);

  EEPROM.get(70, ERROR_CAL1);
  SIGN = EEPROM.read(7);
  ERROR_SET_IN_DEG_FLAG = EEPROM.read(8);
  ERROR_SET_IN_FAH_FLAG = EEPROM.read(9);
  EEPROM.get(80, CAL2_RAW_LOW_VALUE_CONV);
  EEPROM.get(90, CAL2_RAW_HIGH_VALUE_CONV);
  EEPROM.get(100, MASTER_LOW_CONV);
  EEPROM.get(110, MASTER_HIGH_CONV);
  CAL_SELECT_VALUE=EEPROM.read(5);

  //  EEPROM.get(30, Kp);
  //  EEPROM.get(40, Ki);
  //  EEPROM.get(51, Kd);
  //  EEPROM.get(65, TIME_SOCK_OR_TOTAL);
  //  Ki_saved = Ki * 100;
  PRES = 0;
  DISP.start();
  //  PID_EXE.start();

  // Serial1.println("Setpoint  Current Temp   kp  ki  kd   PID Output");

}

void loop()
{
  // Serial2.print("TEMP_DEG_RAW1 :");
  // Serial2.println(TEMP_DEG_RAW1);
  // Serial2.print("ERROR_CAL : ");
  // Serial2.println(ERROR_CAL);

  DISP.update();
  Butn.RTD_TEMP_CONV();
  data++;

  if (PRES < 10000)  //display version
  {
    REPET = 0;
    Butn.BUZZ_INTERVAL(25);
    led.DISPLAY_DIGIT2(34, 24, 24, 24);
    led.DISPLAY_DIGIT1(24, 0, 0, 4);    //version
    DOT1_1 = 1;
    DOT1_0 = 0;
    DOT1_2 = 0;
    PRES++;

  }
  else
  {
    //    {
    //      if(TEMP_DEG_RAW >= 310) //probe error
    //      {
    //        DOT1_1=0;
    //        DOT1_0=0;
    //        DOT1_2=0;
    //        DOT2_1=0;
    //        DOT2_2=0;
    //        DOT2_0=0;
    //        ENTER_BUT = 0;
    //        LONG_PRESS = 0;
    //        PID_SETPOINT =0;
    //        MODE_BUT_SINGLE_PRESS_IN_SETTINGS=0;
    //        digitalWrite(TIMER_LED1, LOW);
    //        digitalWrite(TIMER_LED2, LOW);
    //        led.DISPLAY_DIGIT2(11, 17, 23, 14) ;  //prob eror
    //        led.DISPLAY_DIGIT1(15, 17, 23, 17) ;
    //        Butn.BUZZ_INTERVAL2(1000);
    //        Butn.BUZZ_INTERVAL1(2000);
    //        if(REP == 0)
    //        {
    //          BUZZ_TICK1.start();
    //          REP++;
    //        }
    //      }
    //      else
    //    }
    {
      if (LONG_PRESS )   {
        Proces.SET_LONG_PRESS_FUNCTION(); //settings   str,unit...
      }
      else               {
        // if(EEPROM_AUTO_RESUM_SET==0)
        Proces.SET_SHORT_PRESS_FUNCTION();
        // else if(EEPROM_AUTO_RESUM_SET==1)
        //           {
        //             ENTER_BUT=5;
        //             Proces.SET_SHORT_PRESS_FUNCTION();
        //           }
      } //set function   temp and time

    }
  }
}

void Bclass :: DISPLAY_UPDATE()
{
  DISP.update();
}

void DATA_UPDATE()
{
  //          Butn.Data_Update();
  //          Serial1.print("Setpoint   =  ");

  //           Serial2.print(PID_SETPOINT);
  //           Serial2.print("     ");
  //           Serial2.print(TEMP_DEG);
  //           Serial2.print("     ");
  // //          Serial2.print(VOUT_1);
  // //          Serial2.print("     ");
  // //          Serial1.println("P     I      D");
  // //          Serial1.print(Kp);
  // //          Serial1.print("     ");
  // //          Serial1.print(Ki);
  // //          Serial1.print("     ");
  // //          Serial1.print(Kd);
  // //          Serial1.print("     ");  //PID_OUTPUT  SET_MIN

  //           Serial2.print(PID_OUTPUT1);
  //           Serial2.print("     ");
  //           Serial2.print(Heat);
  //           Serial2.print("     ");
  //           Serial2.print(TEMP_BUFF);
  //           Serial2.print("     ");
  //           Serial2.println(SET_MIN);
  //           Serial2.print("     ");

}

void Bclass ::Data_Update()
{
  DATA_PRINT.update();
}

void AUTO_UPDATE()
{
  Auto ? Auto = 0 : Auto = 1;
  //  Auto = ~Auto;
  lup++;
}

void STOP_BUZZER()
{
  BUZZ.stop();
  digitalWrite(BUZZER, LOW);
}

void DISP_REFRESH()
{
  button1.tick();
  button2.tick();
  button3.tick();
  button4.tick();
  led.DISPLAY_NUMBER1();
  led.DISPLAY_NUMBER2();
}

void PID_LOOP()
{
  if (EXECUTE_PID_LOOP) Butn.PID_RUN();
}

void CHANGE_FLAG()
{
  if (CHANGE1)
  {
    if (TIMER_LED_BLINK) TIMER_LED_BLINK = 0;
    else                TIMER_LED_BLINK = 1;
  }
}

void Bclass :: BUZZ_INTERVAL(int i)
{
  BUZZ.interval(i);
  digitalWrite(BUZZER, HIGH);
  BUZZ.start();
}

void STOP_BUZZER2()
{
  digitalWrite(BUZZER, LOW);
  if (REPETATION_FLAG) FOR_REP++;
  FOR_REP2++;
}

void Bclass :: BUZZ_TICK_STOP()
{
  BUZZ_TICK1.stop();
}

void Bclass :: BUZZ_TICK_START()
{
  BUZZ_TICK1.start();
}

void STOP_BUZZER1()
{
  BUZZ_TICK2.start();
  digitalWrite(BUZZER, HIGH);
}

void Bclass :: BUZZ_INTERVAL1(int i)
{
  BUZZ_TICK1.interval(i);
}

void Bclass :: BUZZ_INTERVAL2(int i)
{
  BUZZ_TICK2.interval(i);
}

void Bclass :: Save_Vent_Data()
{
  if (!VENT_FLAG)
  {
    VENT_VALUE_SET = SET_VALUE[5];
    //                  EEPROM.write(0, VENT_VALUE_SET);
  }
}

void Bclass :: DISP_UPDATE()
{
  DISP.update();
}

void MODE_CLICK()
{
  if (checkTempRaised == 0)
  {
    BLINK_STATS = 0;
    currentMillis1 = 0;
    if (!LONG_PRESS )
    {
      i = 0;
      if (!ENTER_BUT ) //(!ENTER_BUT && !PRINTER_VALUE_SET)
      {
        int CONV3 = (TEMP_DEG * 10);
        MIN_SET_TEMP = ((TEMP_DEG + ((CONV3 % 10) / 10.0)) + 10); //7
      }

      if (ENTER_BUT == 1 &&  SET_VALUE[1] == 0 && !Energy_Mode) ENTER_BUT = 1;  //SET TEMPRATURE IF NOT SET more than 0IT BLINK COUNTINUESLY
      //      else if (ENTER_BUT == 4 && SET_MIN == 0) ENTER_BUT = 4;  //SET_TEMP_DEG         &&  SET_VALUE[1]== 0
      else                                     ENTER_BUT++;   //0 1 2 3 4
      if (ENTER_BUT == 2) ENTER_BUT++;
      // if(ENTER_BUT == 0)
      // {
      //     SET_TEMP_DEG = (EEPROM.read(150) << 8) + EEPROM.read(151);
      //     SET_HOUR  =(EEPROM.read(152) << 8) + EEPROM.read(153);
      //     SET_MIN= (EEPROM.read(154) << 8) + EEPROM.read(155);
      // }
      // if(ENTER_BUT == 5)
      // {

      //    EEPROM.write(150, SET_TEMP_DEG >> 8);//Save set rpm value
      //    EEPROM.write(151, SET_TEMP_DEG & 0xFF);

      //    EEPROM.write(152, SET_HOUR >> 8);//Save set rpm value
      //    EEPROM.write(153, SET_HOUR & 0xFF);

      //    EEPROM.write(154, SET_MIN >> 8);//Save set rpm value
      //    EEPROM.write(155, SET_MIN & 0xFF);

      // }

      //     }
    }
    else
    {
      if (!MODE_BUT_SINGLE_PRESS_IN_SETTINGS)
      {
        EEPROM.get(0, TIME_SOCK_OR_TOTAL);
        SET_LONG_VALUE[Time_Mode]          = TIME_SOCK_OR_TOTAL;
        SET_LONG_VALUE[Auto_Resume_Enable] = AUTO_RESUM_SET;
        SET_LONG_VALUE[Safe_Value_ON_OFF]  = SAFE_VALUE_SET_DEG;
        SET_LONG_VALUE[SafeValue]          = SAFE_VALUE_SET;

        SET_LONG_VALUE[Energy_Mode_Selection]  = Energy_Mode;

        SET_LONG_VALUE[Calibration_Selection] = CAL_SELECT_VALUE;

        SET_LONG_VALUE[OnePointCalibration] = TEMP_DEG_RAW; //TEMP_DEG;                    //CAL1_VALUE_SAVED_DEG1;
        TEMPERARY_SAVED_VARIABLE_FOR_CAL1_DEG =  TEMP_DEG_RAW * 10;


        SET_LONG_VALUE[Cal2LowTemp_Set_Display]  = LOW_CAL_VALUE_SET ;
        SET_LONG_VALUE[Cal2HighTemp_Set_Display] = HIGH_CAL_VALUE_SET ;

        SET_LONG_VALUE[Cal2LowTemp_Enter_Heat]   = TEMP_DEG_RAW; //TEMP_DEG ;

        int CONV = (TEMP_DEG_RAW * 10);
        SET_LONG_VALUE[Cal2LowTemp_Enter_Maintain] = CONV % 10 ;

        if (SELECTION_PARAMETER == Cal2LowTemp_Enter_Maintain  )     TEMP_CAL2_RAW_LOW_VALUE = TEMP_DEG_RAW;
        if (SELECTION_PARAMETER == Cal2HighTemp_Enter_Maintain )     TEMP_CAL2_RAW_HIGH_VALUE = TEMP_DEG_RAW;


        SET_LONG_VALUE[Cal2HighTemp_Enter_Heat] = TEMP_DEG_RAW;   //TEMP_DEG;

        int CONV2 = (TEMP_DEG_RAW * 10);
        SET_LONG_VALUE[Cal2HighTemp_Enter_Maintain]  = ( CONV2 % 10 ) ;

        if (SELECTION_PARAMETER == 17 )  currentMillis = 0;

        if (!TIME_SOCK_OR_TOTAL && SELECTION_PARAMETER == blank )   SELECTION_PARAMETER--;

        MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 1;
      }
      else
      {
        switch (SELECTION_PARAMETER)     //after setting parameters, here we write in eeprom
        { //WRITE EEPROM HERE
          case Time_Mode :
            TIME_SOCK_OR_TOTAL = SET_LONG_VALUE[Time_Mode];
            EEPROM.write(0, TIME_SOCK_OR_TOTAL);
            break;

          case Auto_Resume_Enable :
            AUTO_RESUM_SET = SET_LONG_VALUE[Auto_Resume_Enable];
            EEPROM.write(55, AUTO_RESUM_SET);//EEPROM.put(55, AUTO_RESUM_SET);
            if (!AUTO_RESUM_SET ) {
              RESUME_TIME = 0;
              RESUME_TEMP = 0;
              EEPROM.put(50, RESUME_TIME);
              EEPROM.put(60, RESUME_TEMP);
            }
            //          else                 {RESUME_TIME = SET_MIN; RESUME_TEMP = SET_TEMP_DEG; EEPROM.put(50, RESUME_TIME); EEPROM.put(60, RESUME_TEMP);}
            break;

          case Safe_Value_ON_OFF :
            SAFE_VALUE_SET_DEG = SET_LONG_VALUE[Safe_Value_ON_OFF];
            EEPROM.write(2, SAFE_VALUE_SET_DEG);
            break;

          case Energy_Mode_Selection :
            Energy_Mode = SET_LONG_VALUE[Energy_Mode_Selection];
            EEPROM.write(15, Energy_Mode);
            break;


          case Calibration_Selection :
            CAL_SELECT_VALUE = SET_LONG_VALUE[Calibration_Selection];
            EEPROM.write(5, CAL_SELECT_VALUE);
            break;

          case OnePointCalibration :
            CAL1_VALUE_SAVED_DEG1 = SET_LONG_VALUE[OnePointCalibration];  // EEPROM.write(6, CAL1_VALUE_SAVED_DEG1);
            ERROR_CAL = ((CAL1_VALUE_SAVED_DEG1  * 10) - TEMPERARY_SAVED_VARIABLE_FOR_CAL1_DEG) / 10; //f;
            ERROR_SET_IN_DEG_FLAG = 1; //ERROR_SET_IN_FAH_FLAG=0;

            if (ERROR_CAL < 0)  {
              SIGN = 1;
              ERROR_CAL1 = (( ERROR_CAL * 10 ) * (-1));
            }
            else             {
              SIGN = 0;
              ERROR_CAL1 = ERROR_CAL * 10;
            }
            EEPROM.put(70, ERROR_CAL1);
            EEPROM.write(7, SIGN);
            EEPROM.write(8, ERROR_SET_IN_DEG_FLAG);
            break;

          case Cal2LowTemp_Set_Display :
            LOW_CAL_VALUE_SET   =  SET_LONG_VALUE[Cal2LowTemp_Set_Display];
            break;

          case Cal2HighTemp_Set_Display :
            HIGH_CAL_VALUE_SET  = SET_LONG_VALUE[Cal2HighTemp_Set_Display];
            TEMP_CAL2_RAW_LOW_VALUE = TEMP_DEG_RAW;
            break;

          case Cal2LowTemp_Enter_Maintain :
            TEMP_CAL2_RAW_HIGH_VALUE = TEMP_DEG_RAW;
            break;

          case Cal2HighTemp_Enter_Heat:
            //          case Cal2HighTemp_Enter_Maintain :
            CAL2_RAW_LOW_VALUE_CONV  = ( TEMP_CAL2_RAW_LOW_VALUE * 10);
            CAL2_RAW_HIGH_VALUE_CONV = ( TEMP_CAL2_RAW_HIGH_VALUE * 10);
            EEPROM.put(80, CAL2_RAW_LOW_VALUE_CONV);
            EEPROM.put(90, CAL2_RAW_HIGH_VALUE_CONV);
            MASTER_LOW_CONV  = (CAL2_CALCULATED_LOW_CAL_VALUE * 10);
            MASTER_HIGH_CONV = (CAL2_CALCULATED_HIGH_CAL_VALUE * 10);
            EEPROM.put(100, MASTER_LOW_CONV);
            EEPROM.put(110, MASTER_HIGH_CONV);
            break;

          case SafeValue :
            SAFE_VALUE_SET = SET_LONG_VALUE[SafeValue];
            //  EEPROM.write(65, SAFE_VALUE_SET);
            EEPROM.write(200, SAFE_VALUE_SET >> 8);//Save set rpm value
            EEPROM.write(201, SAFE_VALUE_SET & 0xFF);
            break;

          default :
            break;
        }

        if (SELECTION_PARAMETER == Cal2LowTemp_Enter_Heat)
          SELECTION_PARAMETER = Cal2LowTemp_Enter_Maintain;
        else if (SELECTION_PARAMETER == Cal2HighTemp_Enter_Heat)
          SELECTION_PARAMETER = Cal2HighTemp_Enter_Maintain;

        if (  SELECTION_PARAMETER == Cal2LowTemp_Set_Display
              || SELECTION_PARAMETER == FactoryAndSure_Reset )   //||SELECTION_PARAMETER == 8 || SELECTION_PARAMETER == 10)
        {
          MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 1;
          SELECTION_PARAMETER++;
        }
        else if (SELECTION_PARAMETER == OnePointCalibration
                 || SELECTION_PARAMETER == Cal2HighTemp_Set_Display
                 || SELECTION_PARAMETER == Cal2LowTemp_Enter_Maintain
                 || SELECTION_PARAMETER == Cal2HighTemp_Enter_Maintain)
        {
          MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 0;
          SELECTION_PARAMETER++;
        }
        else        MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 0;

      }
      i = VALUE_FOR_BLINK + 1;
    }
    //
    if ( ENTER_BUT >= 5) //5
    {
      if (Inprocess_flag == 0)
      {
        ENTER_BUT = 5;  //5
        if (VENT_FLAG)   VENT_FLAG = 0;
        else            VENT_FLAG = 1;
      }
    }
    if (ENTER_BUT >= 5 && REP1 < 1) {
      Butn.BUZZ_INTERVAL(1000);
      REP1++;
    }//if(ENTER_BUT >= 5 && REP1<1) {  Butn.BUZZ_INTERVAL(1000);  REP1++; }
    else                           Butn.BUZZ_INTERVAL(50);    //single press control is here
  }
}

void MODE_LONG_PRESS_STOP()
{
  //  SINGLE_SWITCH_PRESS=0;
  //  digitalWrite(BUZZER, LOW);
}

void MODE_LONG_PRESS_START()
{
  if (checkTempRaised == 0)
  {

    //  EEPROM.write(0, TIME_SOCK_OR_TOTAL);
    if (!ENTER_BUT)  //mode single press incriment
    {
      if (!LONG_PRESS )
      {
        i = 350;
        LONG_PRESS = 1;
        RST_BUTT = 1;
      }
    }
    Butn.BUZZ_INTERVAL(200);
  }
}

void DEC_CLICK()    //INC_CLICK()
{
  if (checkTempRaised == 0)
  {
    if (LONG_PRESS && !MODE_BUT_SINGLE_PRESS_IN_SETTINGS )    // Auto_Resume_Enable
    {

      if      ( SELECTION_PARAMETER == Time_Mode )                  SELECTION_PARAMETER = FactoryAndSure_Reset;
      else if ( SELECTION_PARAMETER == FactoryAndSure_Reset )       SELECTION_PARAMETER = Energy_Mode_Selection;
      else if ( SELECTION_PARAMETER == Energy_Mode_Selection )      SELECTION_PARAMETER = Cal2LowTemp_Set_Display;
      else if ( SELECTION_PARAMETER == Cal2HighTemp_Set_Display)    SELECTION_PARAMETER = Cal2LowTemp_Set_Display;
      else if ( SELECTION_PARAMETER == Cal2LowTemp_Set_Display )    SELECTION_PARAMETER = OnePointCalibration;
      else if ( SELECTION_PARAMETER == OnePointCalibration   )      SELECTION_PARAMETER = Calibration_Selection;
      else if ( SELECTION_PARAMETER == SafeValue )                  SELECTION_PARAMETER = Safe_Value_ON_OFF;
      else if ( SELECTION_PARAMETER == Safe_Value_ON_OFF )          SELECTION_PARAMETER = Auto_Resume_Enable;
      else if ( SELECTION_PARAMETER == Auto_Resume_Enable )         SELECTION_PARAMETER = Time_Mode;
      else if ( SELECTION_PARAMETER == Calibration_Selection && SAFE_VALUE_SET_DEG)
        SELECTION_PARAMETER = SafeValue;
      else                                                          SELECTION_PARAMETER--;

      /*
        if      ( SELECTION_PARAMETER == Cal2LowTemp_Set_Display )             SELECTION_PARAMETER = OnePointCalibration;
        else if ( SELECTION_PARAMETER == Time_Mode )                           SELECTION_PARAMETER = Auto_Resume_Enable;
        else if ( SELECTION_PARAMETER == Safe_Value_ON_OFF )                   SELECTION_PARAMETER = Time_Mode;
        else if ( SELECTION_PARAMETER == SafeValue )                           SELECTION_PARAMETER = Safe_Value_ON_OFF;
        else if ( SELECTION_PARAMETER == Cal2LowTemp_Enter_Heat )              SELECTION_PARAMETER = Cal2LowTemp_Set_Display;

        //    else if ( SELECTION_PARAMETER == FactoryAndSure_Reset )                SELECTION_PARAMETER = Cal2LowTemp_Set_Display;
        else if ( SELECTION_PARAMETER == FactoryAndSure_Reset )                SELECTION_PARAMETER = Energy_Mode_Selection;
        else if ( SELECTION_PARAMETER == blank1 )                              SELECTION_PARAMETER = Time_Mode;
        else if ( SELECTION_PARAMETER == Calibration_Selection && SAFE_VALUE_SET_DEG)            SELECTION_PARAMETER = SafeValue;
        //    else if ( SELECTION_PARAMETER == 16 )            SELECTION_PARAMETER = 1;
        else                                             SELECTION_PARAMETER--;

        if (SELECTION_PARAMETER < Time_Mode) SELECTION_PARAMETER = Time_Mode;
      */
    }
    if (!LONG_PRESS  || (LONG_PRESS && MODE_BUT_SINGLE_PRESS_IN_SETTINGS ))    Butn.SET_CHANGE_DOWN();

  }
}

void INC_LONG_PRESS_START()
{
  //  INC_LONGPRESS_START = millis();
}

void INC_LONGPRESS()
{
  if (checkTempRaised == 0)
  {
    BLINK_STATS = 1;
    fast_inc++;
    if (fast_inc < 600)
    {
      INC5++;
      if (INC5 >= 25)
      {
        Butn.SET_CHANGE_UP();
        INC5 = 0;
      }
    }
    else
    {
      INC5++;
      if (INC5 >= 10)
      {
        Butn.SET_CHANGE_UP();
        INC5 = 0;
      }
    }
  }
}

void INC_LONGPRESS_STOP()
{
  if (checkTempRaised == 0)
  {
    BLINK_STATS = 0;
    fast_inc = 0;
  }
}

void INC_CLICK()   //DEC_CLICK()
{
  if (checkTempRaised == 0)
  {
    if (LONG_PRESS && !MODE_BUT_SINGLE_PRESS_IN_SETTINGS )   //SET_LONG_VALUE[SELECTION_PARAMETER]
    {

      if ( SELECTION_PARAMETER == Time_Mode )                                      SELECTION_PARAMETER = Auto_Resume_Enable;  //if ( SELECTION_PARAMETER == 0 &&  VENT_VALUE_SET == 1 )             SELECTION_PARAMETER = 15;
      else if ( SELECTION_PARAMETER == Auto_Resume_Enable )                             SELECTION_PARAMETER = Safe_Value_ON_OFF;
      else if ( SELECTION_PARAMETER == Safe_Value_ON_OFF  && SAFE_VALUE_SET_DEG)   SELECTION_PARAMETER = SafeValue;
      else if ( SELECTION_PARAMETER == SafeValue )                                 SELECTION_PARAMETER = Calibration_Selection;
      else if ( SELECTION_PARAMETER == Calibration_Selection )                     SELECTION_PARAMETER = OnePointCalibration;
      else if ( SELECTION_PARAMETER == OnePointCalibration )                       SELECTION_PARAMETER = Cal2LowTemp_Set_Display;
      else if (SELECTION_PARAMETER == Cal2LowTemp_Set_Display &&  CAL2_START )     SELECTION_PARAMETER = Cal2LowTemp_Enter_Heat;
      else if (CAL2_START == 0 && SELECTION_PARAMETER >= Cal2LowTemp_Set_Display && SELECTION_PARAMETER <= Cal2_Complete_Display  )
        SELECTION_PARAMETER = Energy_Mode_Selection;

      else if (SELECTION_PARAMETER == Energy_Mode_Selection )                       SELECTION_PARAMETER = FactoryAndSure_Reset; //0;   17 on both place
      else if (SELECTION_PARAMETER == FactoryAndSure_Reset )                        SELECTION_PARAMETER = Time_Mode;           //0;   17 on both place
      else                                                                         SELECTION_PARAMETER++;
      //    if (SELECTION_PARAMETER > 17)  SELECTION_PARAMETER = 17;
    }
    if (!LONG_PRESS  || (LONG_PRESS && MODE_BUT_SINGLE_PRESS_IN_SETTINGS ))       Butn.SET_CHANGE_UP();

  }
}

void DEC_LONG_PRESS_START()
{

}

void DEC_LONGPRESS()
{
  if (checkTempRaised == 0)
  {
    BLINK_STATS = 1;
    fast_inc++;
    if (fast_inc < 600)
    {
      INC5++;
      if (INC5 >= 25)
      {
        Butn.SET_CHANGE_DOWN();
        INC5 = 0;
      }
    }
    else
    {
      INC5++;
      if (INC5 >= 10)
      {
        Butn.SET_CHANGE_DOWN();
        INC5 = 0;
      }
    }
  }
}

void DEC_LONGPRESS_STOP()
{
  if (checkTempRaised == 0)
  {
    BLINK_STATS = 0;
    fast_inc = 0;
  }
  //  DEC_LONGPRESS_START=0;
  //  INC_LONGPRESS_START=0;
}

void RESET_CLICK()
{
  if (checkTempRaised == 0)
  {
    // EEPROM.get(0, TIME_SOCK_OR_TOTAL);
    // EEPROM.write(0, TIME_SOCK_OR_TOTAL);
    if (LONG_PRESS )
    {
      if ( SELECTION_PARAMETER == blank  || SELECTION_PARAMETER == Cal2LowTemp_Enter_Maintain || SELECTION_PARAMETER == Cal2HighTemp_Enter_Maintain )  SELECTION_PARAMETER--;
      else
      {
        if (SELECTION_PARAMETER == Cal2LowTemp_Enter_Heat || SELECTION_PARAMETER == Cal2HighTemp_Enter_Heat )  MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 1;
        else
        {
          if (MODE_BUT_SINGLE_PRESS_IN_SETTINGS)    MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 0;
        }
      }
    }
    if (!LONG_PRESS )
    {
      if (ENTER_BUT >= 0 && ENTER_BUT <= 4)  ENTER_BUT--;
      if (ENTER_BUT == 2 ) ENTER_BUT--;                       //THIS LINE ADDED BY ME
      if ( ENTER_BUT == 5   &&  RATE_OF_HEATING >= 1  && CHECK_RATE_OF_HEATING == 1  && !Energy_Mode)
      {
        RATE_OF_HEATING = 0;
        CHECK_RATE_OF_HEATING = 1;
      }
    }
    Butn.BUZZ_INTERVAL(50);
  }
}

void RESET_STOP()
{

}

void RESET_START()
{
  if (checkTempRaised == 0)
  {
    DOT1_2 = 0;
    DOT1_1 = 0;
    DOT2_0 = 0;
    Flag == 0;
    ErroR = 0;
    RESUME_Temp = 0;
    TEMP_FLAG = 0;
    RESUME_TIME = 0;
    RESUME_TEMP = 0;
    EEPROM_AUTO_RESUM_SET = 0;
    Inprocess_autoresume_flag = 0;
    EEPROM.write(160, 0);
    EEPROM.put(50, RESUME_TIME);
    EEPROM.put(60, RESUME_TEMP);


    if (LONG_PRESS )   LONG_PRESS = 0;
    if (!ENTER_BUT)
    {
      RST_BUTT = 0;
      LONG_PRESS = 0;
      //    SELECTION_PARAMETER = 1;// 0;
    }
    if (ENTER_BUT > 0)
    {
      once1 = 0;
      ENTER_BUT = 0;
      SET_MIN = 0;
      ExitFromProcess = 1;
    }
    switch (SELECTION_PARAMETER)
    {
      case Time_Mode :
        TIME_SOCK_OR_TOTAL = SET_LONG_VALUE[Time_Mode];
        break;
      case Auto_Resume_Enable :
        AUTO_RESUM_SET = SET_LONG_VALUE[Auto_Resume_Enable];
        break;
      case Safe_Value_ON_OFF :
        SAFE_VALUE_SET_DEG = SET_LONG_VALUE[Safe_Value_ON_OFF];
        break;
      case Calibration_Selection :
        CAL_SELECT_VALUE = SET_LONG_VALUE[Calibration_Selection];
        break;
      case OnePointCalibration :
        CAL1_VALUE_SAVED_DEG1 = SET_LONG_VALUE[OnePointCalibration];
        break;

      case Cal2LowTemp_Set_Display :
        LOW_CAL_VALUE_SET   = SET_LONG_VALUE[Cal2LowTemp_Set_Display];
        break;

      case Cal2HighTemp_Set_Display :
        HIGH_CAL_VALUE_SET  = SET_LONG_VALUE[Cal2HighTemp_Set_Display];
        break;

      case Cal2LowTemp_Enter_Heat :
        CAL2_LOW1_VALUE_SET  =  SET_LONG_VALUE[Cal2LowTemp_Enter_Heat] ;
        break;
      case Cal2LowTemp_Enter_Maintain :
        CAL2_LOW2_VALUE_SET  =  SET_LONG_VALUE[Cal2LowTemp_Enter_Maintain];
        break;
      case Cal2HighTemp_Enter_Heat :
        CAL2_HIGH1_VALUE_SET = SET_LONG_VALUE[Cal2HighTemp_Enter_Heat];
        break;
      case Cal2HighTemp_Enter_Maintain :
        CAL2_HIGH2_VALUE_SET = SET_LONG_VALUE[Cal2HighTemp_Enter_Maintain];
        break;
      case  Energy_Mode_Selection  :
        Energy_Mode = SET_LONG_VALUE[ Energy_Mode_Selection ];
        break;
      case SafeValue  :

        SAFE_VALUE_SET = SET_LONG_VALUE[SafeValue ];
        // Serial2.println(SAFE_VALUE_SET);

        break;
      default :
        break;
    }
    Butn.BUZZ_INTERVAL(200);
  }
}

void RTD_TEMP_CAL()
{
  //     sample++;
  //     AvgVout = analogRead(A0);//SAFETY RTD  //AvgVout = analogRead(A1); A1 FOR RTD 2 AND A0 FOR RTD 1
  //     TotalVout  += AvgVout ;  // Vout;
  //     if(sample >= avrage_val)
  //     {
  //     TotalVout /= avrage_val;
  //     float Vout = ((TotalVout * Vs) / 1023.0); //voltage in mv                                     // Voltage conversion
  //     Vin = Vout / 10; //amplifier gain 1000/100=10    Vb=(Rb*Vs)/(Rb+Ra)                                                            // Gain
  //     Rt = ((Vb + Vin) * Rc) / (Vs - (Vb + Vin));   //(Va = Vb + Vin)   Bridge referance voltage and RTD voltage calculation
  //     // Rt=(Vin +
  //     TEMP_DEG_RAW = ((Rt / Rb) - 1) / (Alpha); //pt100 transducer relationship and voltage to temp conversion  // alpha is temp coefficient for platinum
  // //    TEMP_FAH_RAW = (( TEMP_DEG_RAW * 1.8 ) + 32);
  //     sample = 0;   AvgVout = 0;
  //     }
  //    avrage_val = 50;


  //    //for 2nd analog read
  //    sample1++;
  //    AvgVout1 = analogRead(A1);   //ADC1
  //    TotalVout1  += AvgVout1;
  //    if(sample1 >= avrage_val1)
  //    {
  //     TotalVout1 /= avrage_val1;
  //     float Vout1=(( TotalVout1 * Vs) / 1023.0);
  //     Vin1 =  Vout1 / 10;
  //     Rt1 = ((Vb + Vin1) * Rc) / (Vs - (Vb + Vin));
  //     VOUT_1 = ((Rt1 / Rb) - 1) / (Alpha);
  //     sample1 = 0;   AvgVout1 = 0;
  //    }
  //    avrage_val1 = 50;


}

void Bclass :: RTD_TEMP_CONV()
{
  if (TEMP_DEG_RAW1 == 0) TEMP_DEG_RAW = TEMP_DEG_RAW2;
  else                   TEMP_DEG_RAW = TEMP_DEG_RAW1;
  if (SIGN) {
    ERROR_CAL = (ERROR_CAL1 / 10) * -1;
  }
  else     {
    ERROR_CAL = (ERROR_CAL1 / 10) ;
  }
  if (ERROR_SET_IN_DEG_FLAG == 1)
  {
    //      Serial2.println(1);
    TEMP_DEG = TEMP_DEG_RAW + ERROR_CAL;
  }
  if (ERROR_SET_IN_DEG_FLAG == 0)
  {
    //      Serial2.println(2);
    TEMP_DEG = TEMP_DEG_RAW;
  }

  if (!CAL_SELECT_VALUE)
  {
    //        Serial2.println(3);
    // if(ERROR_SET_IN_DEG_FLAG)                         TEMP_DEG  =  TEMP_DEG_RAW + ERROR_CAL;     //show this values as temprature
    //     if(SIGN) {ERROR_CAL =(ERROR_CAL1/10) * -1;}
    //     else     {ERROR_CAL =(ERROR_CAL1/10) ;}

    if (ERROR_SET_IN_DEG_FLAG == 1)
    {
      //      Serial2.println(4);
      TEMP_DEG  =  TEMP_DEG_RAW + ERROR_CAL;     //show thi
    }
    // TEMP_DEG  =   TEMP_DEG_RAW  +  ERROR_CAL;
    //   // Serial2.print("TEMP_DEG_RAW1 :");
    //   // Serial2.println(TEMP_DEG_RAW1);
    // // Serial2.print("ERROR_CAL");
    // Serial2.println(ERROR_CAL);

  }
  else
  {
    // Serial2.println(2);
    CAL2_RAW_LOW_VALUE = (CAL2_RAW_LOW_VALUE_CONV / 10.0);
    CAL2_RAW_HIGH_VALUE = (CAL2_RAW_HIGH_VALUE_CONV  / 10.0);
    MASTER_LOW =  (MASTER_LOW_CONV  / 10.0);
    MASTER_HIGH = (MASTER_HIGH_CONV  / 10.0);
    TEMP_DEG = mapFloat( TEMP_DEG_RAW, CAL2_RAW_LOW_VALUE, CAL2_RAW_HIGH_VALUE, MASTER_LOW,  MASTER_HIGH);
    TEMP_FAH = ( TEMP_DEG * 1.8 ) + 32;
  }
  //  Serial2.println(TEMP_DEG);
}

void Bclass :: TICKER_UPDATE()
{
  BUZZ.update();
  Toggle1.update();
  Proces.SSR_fail_show_ticker_update();
  if (TEMP_DEG >= 390)
  {
    digitalWrite( HEATER , LOW);
  }
  else
  {
    if (!Energy_Mode)  PID_EXE.update();
    else
    {
      if (ENTER_BUT == 5)
        EnergyRegulator.EnergyModeOn();
    }
  }

  BUZZ_TICK1.update();
  BUZZ_TICK2.update();
  TEMP_UPDATE.update();
  K_type.Temp_Tick_Update();
}

void Bclass ::EEPROM_CLEAR()
{

  TIME_SOCK_OR_TOTAL = 1;
  // TIME_SOCK_OR_TOTAL = 0;
  AUTO_RESUM_SET = 0;
  SAFE_VALUE_SET_DEG = 0;  //99
  SAFE_VALUE_SET = 300;
  //                SAFE_VALUE_SET_FAH = 212;
  CAL_SELECT_VALUE = 0;
  ERROR_CAL1 = 0;
  SIGN = 0;
  ERROR_SET_IN_DEG_FLAG = 1;
  ERROR_SET_IN_FAH_FLAG = 0;
  CAL2_RAW_LOW_VALUE_CONV = 30;
  CAL2_RAW_HIGH_VALUE_CONV  = 84;
  MASTER_LOW_CONV  = 28;
  MASTER_HIGH_CONV  = 82;
  Kp = 150, Ki = 0, Kd = 0, Ki_saved = 0;
  consKp = 150, consKi = 0.5, consKd = 400;
  //                TIME_SOCK_OR_TOTAL =0;
  //                currentMillis=0;
  //                MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 0;
  LONG_PRESS = 0;
  //  PRINTER_VALUE_SET=0;
  // SET_LONG_VALUE[0] = TIME_SOCK_OR_TOTAL;
  SET_LONG_VALUE[Time_Mode] = TIME_SOCK_OR_TOTAL;
  SET_LONG_VALUE[1] = AUTO_RESUM_SET;
  SET_LONG_VALUE[2] = SAFE_VALUE_SET_DEG;
  //  SET_LONG_VALUE[21]= SAFE_VALUE_SET_FAH;
  SET_LONG_VALUE[3] = CAL_SELECT_VALUE;
  //  SET_LONG_VALUE[13] = TIME_SOCK_OR_TOTAL;
  //  SET_LONG_VALUE[14] = Kp;
  //  SET_LONG_VALUE[15] = VENT_TIME_SET;   //  SET_LONG_VALUE[15] = Ki * 100;
  SET_LONG_VALUE[16] = SAFE_VALUE_SET;

  Energy_Mode = 1;
  SET_LONG_VALUE[Energy_Mode_Selection] = Energy_Mode;

  //  SET_LONG_VALUE[16] = Kd;
  EEPROM.write(2, SAFE_VALUE_SET_DEG);
  // EEPROM.put(65, SAFE_VALUE_SET);
  EEPROM.write(200, SAFE_VALUE_SET >> 8);//Save set rpm value
  EEPROM.write(201, SAFE_VALUE_SET & 0xFF);
  EEPROM.write(0, TIME_SOCK_OR_TOTAL);
  EEPROM.write(55, AUTO_RESUM_SET);

  EEPROM.write(15, Energy_Mode);
  //  EEPROM.put(30, VENT_TIME_SET);   //  EEPROM.put(30, Kp);
  //  EEPROM.write(1, PRINTER_VALUE_SET);
  //  EEPROM.write(2, AUTO_RESUM_SET);
  //  EEPROM.put(3, SAFE_VALUE_SET_FAH);
  EEPROM.write(5, CAL_SELECT_VALUE);
  EEPROM.put(70, ERROR_CAL1);
  EEPROM.write(7, SIGN);
  EEPROM.write(8, ERROR_SET_IN_DEG_FLAG);
  EEPROM.write(9, ERROR_SET_IN_FAH_FLAG);
  EEPROM.put(80, CAL2_RAW_LOW_VALUE_CONV);
  EEPROM.put(90, CAL2_RAW_HIGH_VALUE_CONV);
  EEPROM.put(100, MASTER_LOW_CONV);
  EEPROM.put(110, MASTER_HIGH_CONV);

  EEPROM.write(160, 0);

  EEPROM.write(150, 0 >> 8);//Save set rpm value
  EEPROM.write(151, 0 & 0xFF);

  EEPROM.write(180, 0);

  EEPROM.write(152, 0 >> 8);//Save set rpm value
  EEPROM.write(153, 0 & 0xFF);

  EEPROM.write(154, 0 >> 8);//Save set rpm value
  EEPROM.write(155, 0 & 0xFF);

  //  EEPROM.put(65, 0);
  //  EEPROM.get(30, Kp);
  //  EEPROM.put(40, Ki);               // EEPROM.get(30, Kp);EEPROM.get(40, Ki);EEPROM.get(51, Kd);
  //  EEPROM.put(51, Kd);
  MODE_BUT_SINGLE_PRESS_IN_SETTINGS = 0;
  currentMillis = 0;
}
