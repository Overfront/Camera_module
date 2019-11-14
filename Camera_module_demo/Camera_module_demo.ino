// Camera module for ff3 MDF5 older modification AM5T, BM5T, EM5T и FM5T
// Wait income pakage id433 (masks and filters set)
// If 3 bite data 0x02 - revers on -> send to id 303 pakage 2 bite 0x40
// If 0x00 - revers off -> send 0x20 

#include <avr/wdt.h>
#include <SPI.h>
#include <mcp_can.h>

#define DEBUG

#define reversOff 0x00
#define reversOn 0x02
#define displayModeId 0x303

unsigned char send_camera[8] = {0, 0, 0x40, 0, 0, 0, 0, 0};
unsigned char send_normal[8] = {0, 0, 0x20, 0, 0, 0, 0, 0};

const int SPI_CS_PIN = 10;
const int CAN_INT_PIN = 2;

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
unsigned char flagRecv = 0;

MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  
  Serial.begin(115200);
  while(CAN_OK!=CAN.begin(MCP_STDEXT, CAN_500KBPS, MCP_8MHZ))
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(100);
  }
  Serial.println("CAN BUS Shield init ok!");
  attachInterrupt(digitalPinToInterrupt(CAN_INT_PIN), MCP2515_ISR, FALLING); // start interrupt

  CAN.init_Mask(0,0,0x07FF0000);                // Init first mask...
  CAN.init_Filt(0,0,0x04330000);                // Init first filter...
  CAN.init_Filt(1,0,0x04330000);                // Init second filter...
 
  CAN.init_Mask(1,0,0x07FF0000);                // Init second mask...
  CAN.init_Filt(2,0,0x04330000);                // Init third filter...
  CAN.init_Filt(3,0,0x04330000);                // Init fouth filter...
  CAN.init_Filt(4,0,0x04330000);                // Init fifth filter...
  CAN.init_Filt(5,0,0x04330000);                // Init sixth filter...
  CAN.setMode(MCP_NORMAL);                // Change to normal mode to allow messages to be transmitted

  // watchdog set
  wdt_enable(WDTO_4S);
}

void MCP2515_ISR() {
  flagRecv = 1;
}

void loop()
{
  #ifdef DEBUG
  if (Serial.available() > 0) {
    unsigned char command = Serial.read();
    switch (command) {
        case 'A': 
          CAN.sendMsgBuf(displayModeId, 0, 8, send_normal);
          Serial.println("Normal mode");
          break;
        case 'B': 
          CAN.sendMsgBuf(displayModeId, 0, 8, send_camera);
          Serial.println("Camera mode");
          break;
        default:
          Serial.println("False command");
    }
  }
  #endif
  
  if(flagRecv)                                      // check if get data
  {
    flagRecv = 0;  
    CAN.readMsgBuf(&rxId, &len, rxBuf); // read data, id: data id, len: data length, buf: data buf
      switch (rxBuf[3]) {
        case reversOff: 
         #ifdef DEBUG
            Serial.println("Send Display turned NORMAL mode");
          #endif
          CAN.sendMsgBuf(displayModeId, 0, 8, send_normal); // send package
          break;
        case reversOn: 
          #ifdef DEBUG
            Serial.println("Send Display turned CAMERA mode");
          #endif
          delay(10);
          CAN.sendMsgBuf(displayModeId, 0, 8, send_camera); // send package
          break;
      }   
    #ifdef DEBUG
      Serial.println("-----------------------------");
      Serial.print("Get data from ID: ");
      Serial.println(rxId, HEX);

      for(int i = 0; i<len; i++) // print the data
      {
        Serial.print(rxBuf[i], HEX);
        Serial.print("\t");
      }
      Serial.println();
    #endif
   }
   
   wdt_reset();
}
