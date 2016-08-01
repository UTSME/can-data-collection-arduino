#include <SPI.h>
#include "mcp_can.h"

const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN); 

unsigned char flagRecv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];

void setup()
{
    Serial.begin(115200);
    while (CAN_OK != CAN.begin(CAN_1000KBPS))
    {
        Serial.println("CAN BUS INIT FAIL");
        delay(1000);
    }
    Serial.println("CAN BUS INIT OK");

    //Connect MCP2515 interrupt to digital pin 2 on arduino due
    attachInterrupt(2, MCP2515_ISR, FALLING);
}

void MCP2515_ISR()
{
    flagRecv = 1;
}

void loop()
{
    if(flagRecv) 
    {
        flagRecv = 0;

        // iterate over all pending messages
        // If either the bus is saturated or the MCU is busy,
        // both RX buffers may be in use and reading a single
        // message does not clear the IRQ conditon.
        while (CAN_MSGAVAIL == CAN.checkReceive()) 
        {
            // read data,  len: data length, buf: data buf
            CAN.readMsgBuf(&len, buf);

            //Print the id
            Serial.print(CAN.getCanId());
            Serial.print(":");

            // print the data
            for(int i = 0; i<len; i++)
            {
                Serial.print(buf[i]);
                Serial.print(",");
            }
            Serial.println();
        }
    }
}
