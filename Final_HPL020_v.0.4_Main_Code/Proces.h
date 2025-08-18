#ifndef Proces_h
#define Proces_h

class Pclass
{
   public :
   Pclass();
   void SETUP();
   void SET_LONG_PRESS_FUNCTION();
   void SET_SHORT_PRESS_FUNCTION();  
   void tempRiseCheckUpdate();
   void SSR_fail_show_ticker_update();
 
};

enum 
{   
  Time_Mode,            //---------------0  
  Auto_Resume_Enable,   //---------------1
  Safe_Value_ON_OFF,    //---------------2  
  Calibration_Selection,//---------------3  
  OnePointCalibration,  //---------------4

  blank,
//  Energy_Mode_Selection              //---------------5
  
  Cal2LowTemp_Set_Display,           //---------------6
  Cal2HighTemp_Set_Display ,         //---------------7
  Cal2LowTemp_Enter_Heat,            //---------------8
  Cal2LowTemp_Enter_Maintain,        //---------------9
  Cal2HighTemp_Enter_Heat,           //---------------10
  Cal2HighTemp_Enter_Maintain,       //---------------11
  Cal2_Complete_Display,             //---------------12

  
  Energy_Mode_Selection,      //---------------??

  FactoryAndSure_Reset,       //---------------13
  ResetDone,                  //---------------14

  blank1,
  SafeValue                   //---------------16  
  
}LongPressSelection;


extern Pclass Proces;

#endif
