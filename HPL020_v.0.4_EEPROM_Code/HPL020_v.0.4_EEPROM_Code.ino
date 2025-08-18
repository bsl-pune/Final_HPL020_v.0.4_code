#include <EEPROM.h>
void setup() 
{
  int eprom_var = 0;
  EEPROM.write(0, eprom_var);   //TIME_SOCK_OR_TOTAL
  EEPROM.write(2, eprom_var);   //SAFE_VALUE_SET_DEG
  EEPROM.write(5, eprom_var);   //CAL_SELECT_VALUE
  EEPROM.write(7, eprom_var);   //SIGN
  EEPROM.write(8, eprom_var);   //ERROR_SET_IN_DEG_FLAG
  EEPROM.write(9, eprom_var);   //ERROR_SET_IN_FAH_FLAG
  EEPROM.write(50, eprom_var);  //RESUME_TIME
  EEPROM.write(55, eprom_var);  //AUTO_RESUM_SET
  EEPROM.write(60, eprom_var);  //RESUME_TEMP
  EEPROM.write(65, eprom_var);  //SAFE_VALUE_SET
  EEPROM.write(70, eprom_var);  //ERROR_CAL1
  EEPROM.write(80, eprom_var);  //CAL2_RAW_LOW_VALUE_CONV
  EEPROM.write(90, eprom_var);  //CAL2_RAW_HIGH_VALUE_CONV
  EEPROM.write(100, eprom_var); //MASTER_LOW_CONV
  EEPROM.write(110, eprom_var); //MASTER_HIGH_CONV
}

void loop()
{}
