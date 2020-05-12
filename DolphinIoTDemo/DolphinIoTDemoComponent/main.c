/**
 * @file
 *
 * This app reads all the Register on BQ24196 or BQ24296 Battery Chargers. It also writes output
 * voltage for battery charger to 3.7V.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless, Inc. Use of this work is subject to license.
 */

#include "legato.h"
#include "interfaces.h"

//Resource path to show the counter of UART frames received
static const char ReceivedUARTFramesResPath[] = "ReceivedUARTFrames";
//Counter of received UART frames from Dolphin
static double ReceivedUARTFramesCounter = 0; 

// Sync byte to indicate the start of a serial packet
#define SYNC_PACKET_START   0x55
// Minimum len of a serial frame (Sync packet start + Header + HeaderCRC)
#define SERIAL_SYNC_MIN_LEN    6 
//--------------------------------------------------------------------------------------------------
/**
 * Main program starts below
 */
//--------------------------------------------------------------------------------------------------
COMPONENT_INIT
{
  LE_INFO("== Starting Dolphin UART communication Demo ==");

  int fd = -1;
  char readBuffer[80]= "";
  int bufferLen = 0;

  LE_ASSERT_OK(dhubIO_CreateInput(ReceivedUARTFramesResPath, DHUBIO_DATA_TYPE_NUMERIC, ""));
  dhubIO_SetNumericDefault(ReceivedUARTFramesResPath, (double)ReceivedUARTFramesCounter);

  fd = le_tty_Open("/dev/ttyHS0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd != -1)  
  {
	  LE_INFO("ttyHSL1 port successfully opened");
	}
	le_tty_SetBaudRate(fd, LE_TTY_SPEED_57600);
	le_tty_SetFraming(fd, 'N', 8, 1);
	le_tty_SetFlowControl(fd, LE_TTY_FLOW_CONTROL_NONE);
	le_tty_SetRaw(fd, 8, 1000.0);
  LE_INFO("== UART STARTED READING ==");

  memset(readBuffer,0,80);

  while(1)
  {
    bufferLen = read(fd, readBuffer, 32);

    if(bufferLen > SERIAL_SYNC_MIN_LEN && readBuffer[0] == SYNC_PACKET_START)
    {      
      char stringbuf[100];
      ssize_t i;
      char* buf2 = stringbuf;
      char* endofbuf = stringbuf + sizeof(stringbuf);
      
      memset(stringbuf,0,100);

      //Format the serial command to see it in hex formats.
      for (i = 0; i < bufferLen; i++)
      {
          if (buf2 + 5 < endofbuf)
          {
              if (i > 0)
              {
                  buf2 += sprintf(buf2, ":");
              }
              buf2 += sprintf(buf2, "%02X", readBuffer[i]);
          }
      }
      buf2 += sprintf(buf2, "\n");

      ReceivedUARTFramesCounter++;

      LE_INFO("Buffer Len = %d \nReceived bytes from UART2 : %s \n.",bufferLen, stringbuf);
      LE_INFO("Number of counted UART frames %d\n.", (int)ReceivedUARTFramesCounter);
      
      dhubIO_PushNumeric(ReceivedUARTFramesResPath, DHUBIO_NOW, (double)ReceivedUARTFramesCounter);
    }
  }
}



