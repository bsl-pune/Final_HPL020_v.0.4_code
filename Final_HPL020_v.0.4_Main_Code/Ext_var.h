#ifndef Ext_Var_h
#define Ext_Var_h


// #include <EEPROM.h>
#include "OneButton.h"
#include "MapFloat.h"
//#include "process.h"
#include "Arduino.h"
#include <PID_v1.h>
#include "Ticker.h"
#include "Butn.h"
#include "led.h"
#include "Proces.h"
#include "EnergyRegulator.h"
#include "K_type.h"

extern int8_t ENTER_BUT, LONG_PRESS, MODE_BUT_SINGLE_PRESS_IN_SETTINGS, TIME_WASTE_FLAG;
extern float ERROR_CAL, CAL2_CALCULATED_LOW_CAL_VALUE, CAL2_CALCULATED_HIGH_CAL_VALUE,  TEMP_CAL2_CALCULATED_HIGH_CAL_VALUE ;
extern float CAL2_RAW_LOW_VALUE, CAL2_RAW_HIGH_VALUE, RAW_LOW_VALUE, RAW_HIGH_VALUE,MASTER_LOW, MASTER_HIGH;
extern float  TEMP_DEG,TEMP_DEG_RAW, TEMP_FAH_RAW, TEMP_FAH;
extern int SET_TEMP_DEG;
extern int SET_HOUR, SET_MIN;
extern float RATE_OF_HEATING,VOUT_1,BUFF;
extern int  currentMillis, i, digits1[4],  digits2[4] , TEMPERARY_SAVED_VARIABLE_FOR_CAL1_DEG;
extern bool CHECK_RATE_OF_HEATING;
extern bool TIMER_LED_BLINK,CHANGE1,Auto;
extern bool DOT2_1, DOT2_2, DOT1_1, DOT1_2, DOT2_0,DOT1_0;
extern bool PRO_STOP_WITH_LONG_PRESS, PRINTER_VALUE_SET, CAL2_START,AUTO_RESUM_SET;
extern bool VENT_VALUE_SET, PRINTER_VALUE_SET, START_CAL_VALUE,SAFE_VALUE_SET_DEG;
extern bool TIME_SOCK_OR_TOTAL,RST_BUTT, BLINK_STATS;
extern bool REPETATION_FLAG, EXECUTE_PID_LOOP;
extern bool ERROR_SET_IN_DEG_FLAG, ERROR_SET_IN_FAH_FLAG;
extern bool VENT_FLAG,Auto,Heat;
extern int  FOR_REP, FOR_REP2, REP, REP1,REPET,Flag;
extern int  VALUE_FOR_BLINK,SET_MIN, serialtime, RESUME_TIME, RESUME_TEMP, MIN_RESUME_TIME, TIME_FACT, CURRENT_TIME;
extern int  SAFE_VALUE_SET,SAFE_VALUE_SET_FAH, ERROR_VALUE_SET, VENT_TIME_SET;
extern int  LOW_CAL_VALUE_SET, HIGH_CAL_VALUE_SET, CAL_SELECT_VALUE;
extern int  CAL1_VALUE_SAVED_DEG1,CAL1_VALUE_SAVED_DEG2, CAL1_VALUE_SAVED_FAH;
extern int  CAL2_LOW1_VALUE_SET, CAL2_LOW2_VALUE_SET, CAL2_HIGH1_VALUE_SET, CAL2_HIGH2_VALUE_SET;
extern int  SELECTION_PARAMETER,TEMP_FLAG,ErroR,RESUME_Temp,DIV_FACT;
extern int  currentMillis1, SET_TEMP_FAH, ERROR_CAL_IN_FAH;
extern int  SET_LONG_VALUE[25],SET_VALUE[7];
extern int  MIN_SET_TEMP, MIN_SET_FAH,RESUME_SET_TEMP,TEMP_BUFF;
extern long   CAL2_RAW_LOW_VALUE_CONV, CAL2_RAW_HIGH_VALUE_CONV,MASTER_LOW_CONV, MASTER_HIGH_CONV,lup;
extern double Kp,Ki,Kd, Ki_saved,consKp, consKi, consKd, PID_SETPOINT;
extern double  ERROR_CAL1,PID_OUTPUT1;
extern bool EEPROM_AUTO_RESUM_SET;
extern float z;
extern bool Inprocess_autoresume_flag;
extern bool once1,Inprocess_flag;
extern float  TEMP_DEG_RAW2, TEMP_DEG_RAW1;
extern bool Probe_error_flag,Intial_time_for_ktype_read_FLAG;
/*-----------------------------------------------------------------------------------------------------------------------*/

extern bool COUNTDOWN_START;

extern int STOP_PROCESS;
extern int INC;
extern int TIME_INC;
extern int INTERATION, TOLERANCE_VALUE;
extern int SPEED_SW[5];
extern int MAX_SET_VALUE_LONG[20];
extern int MAX_SET_VALUE[7];

extern double PID_SETPOINT; //,PID_OUTPUT1=0;
extern double PID_INPUT;
extern double PID_OUTPUT;

extern bool Energy_Mode;
extern int8_t EnergySetValue;
extern unsigned int ElapsedMinute;
extern bool ExitFromProcess;
extern bool ElapsedTime;
extern bool checkTempRaised;

//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern
//extern

class Vclass
{
  public :
  Vclass();
};

extern Vclass var;
#endif
