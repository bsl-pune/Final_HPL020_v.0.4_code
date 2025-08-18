#ifndef led_h
#define led_h

class Eclass
{
   public :
   Eclass();
   void SETUP();
   void cathode_high1();
   void cathode_high2();
   void DISPLAY_NUMBER1();
   void DISPLAY_NUMBER2();  
   void BREAK_NUMBER1(long num);
   void BREAK_NUMBER2(long num2);
   void DISPLAY_DIGIT2(int DIG2_1, int DIG2_2, int DIG2_3, int DIG2_4);
   void DISPLAY_DIGIT1(int DIG1_1, int DIG1_2, int DIG1_3, int DIG1_4);

};
extern Eclass led;
#endif
