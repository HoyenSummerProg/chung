#ifndef __b20_H  
#define __b20_H  
  
#define SkipROM 0xCC //skip ROM  
#define SearchROM 0xF0 //search ROM  
#define ReadROM 0x33 //read ROM  
#define MatchROM 0x55 //match ROM  
#define AlarmROM 0xEC //alarm ROM  
  
#define StartConvert 0x44 /*Start temperature conversion,
output 0 on the bus during temperature conversion, output 1 after conversion*/  

#define ReadScratchpad 0xBE //Read the 9 bytes of the scratchpad 

#define WriteScratchpad 0x4E //Write the temperature alarm TH and TL of the scratchpad  

#define CopyScratchpad 0x48 /**Copy the temperature alarm of the scratchpad to the EEPROM,
output 0 on the bus during copying, and output 1 after copying**/

#define RecallEEPROM 0xB8 /**Copy the EEPROM temperature alarm to the scratchpad, 
output 0 during copying, and output 1 after copying  **/

#define ReadPower 0xB4 //Read power supply mode: 0 for parasitic power supply, 1 for external power supply 
  
  
void  ds18b20_init( void );  
float ds18b20_read(void); 
  
  
#endif  
