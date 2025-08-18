#ifndef Butn_h
#define Butn_h

#define HEATER_STATUS_LED 27 // 28
#define CURRENT_PARA_LED  28 // 27
#define SET_PARA_LED      29// 26
#define ALARM_LED         26// 29

#define BUZZER            21//16    21

#define MODE_KEY          35   
#define UP_KEY            37  
#define DOWN_KEY          36
#define RST_KEY           34

#define HEATER            24
// #define VENT              25
#define safteyRelay       33
#define TIMER_LED1        12
#define TIMER_LED2        13
//24-HEATER //25 -VENT//
//#define SAFETY_RTD        22  

class Bclass
{
   public :
   Bclass();
   void SETUP();
   void PID_RUN();
   void TIME_CAL();
   void DISP_UPDATE();   
   void ALL_LED_OFF();
   void EEPROM_CLEAR();
   void SET_CHANGE_UP();
   void BUZZ_TICK_STOP();
   void BUZZ_TICK_START();
   void SET_CHANGE_DOWN();
   void BUZZ_INTERVAL(int i);
   void BUZZ_INTERVAL1(int i);
   void BUZZ_INTERVAL2(int i);
   void TICKER_UPDATE();
   void PID_callingg();
   void RTD_TEMP_CONV();   
   void Save_Vent_Data();
   void Data_Update();
   void DISPLAY_UPDATE();

};
extern Bclass Butn;

#endif
