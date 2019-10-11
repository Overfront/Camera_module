# Camera_module
Arduino based not original rear view camera emulator for screen Ford MFD5 "(type AM5T, BM5T, EM5T and FM5T)  
It consist of Arduino Nano and MCP2515 CAN Bus module.  
### View  
![alt text](https://github.com/Overfront/Camera_module/blob/master/20191008_162455.png?raw=true)

### How it work  
Sketch used MCP2515 library which right masks and filters https://github.com/Overfront/MCP_CAN_lib.  
Before compiling sketch instal it to your Arduino IDE.  
Code is simple. Arduino listen fords MM_CAM and wait package from id 0x433 (rear gear indicator).
When it recieved compare byte number four then send package to id 303 to change screen mode.
If it's [0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00] -> reverse turn off, send to id 0x303: [0x00 0x00 0x20 0x00 0x00 0x00 0x00 0x00] normal mode.  
or if [0x00 0x00 0x00 0x02 0x00 0x00 0x00 0x00] -> reverse turn on, send to id 0x303: [0x00 0x00 0x40 0x00 0x00 0x00 0x00 0x00] camera view mode.
Masks and filters are set to the only one id 0x433.

### Connection
The device is connect to audio module connector pins  
Kl.15 (pin 3) - Vin Arduino nano  
Ground (pin 13) - GND Arduino nano  
MM_CAN High (pin 14) - CAN H mcp2515  
MM_CAN Low (pin 15) - CAN L mcp2515  
 
### Remember
And don't forget so that it work right you should change central vehicle configuration car (byte 99 to with rear camera) and reinitialise module screen FCDIM.
