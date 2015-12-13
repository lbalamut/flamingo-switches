/* 
Based on great work of 

Created by Karl-Heinz Wind - karl-heinz.wind@web.de
Copyright 2015 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/

#include "FlamingoSwitch.h"
#include "codes.h"

FlamingoSwitch Switch;

int RX_PIN = 1; // PIN3 rx needs to be in IRQ pin!
int TX_PIN = 4; // PIN4

const int CODE_COUNT = 4;
int counter = 0;
int deviceCounter = 0;

//for scaner
uint32_t codes[CODE_COUNT] = { 0, 0, 0, 0 };
String bins[CODE_COUNT] = { "", "", "", "" };

                  
void send(int device, int command)
{
	Serial.print("Sending device: ");
	Serial.print(device);
	Serial.print(" command: ");
	Serial.println(command);

  for (unsigned int i = 0; i< CODES_PER_COMMAND; i++) {
    uint32_t sendBuffer = DEVICE_CODES[device][command][i];
    Switch.send(sendBuffer);
  }
}

void setup() 
{
	Serial.begin(9600);

	Switch.enableReceive(RX_PIN);  // Receiver on interrupt 0 => that is pin #2
	Switch.enableTransmit(TX_PIN);
}

void loop() 
{
	// detect code from remote control
	if (Switch.available())
	{
		output(Switch.getReceivedValue(), Switch.getReceivedBitlength(), Switch.getReceivedDelay(), Switch.getReceivedRawdata());
		Switch.resetAvailable();
	}

//  Switch.send((uint32_t)0x24AC25A);
	// Example for sending the codes
  for (unsigned int i = 0; i< DEVICES; i++) {
    send(i, ON);
    send(i, OFF);  
  }
}

void output(unsigned long code, unsigned int length, unsigned int delay, unsigned int* raw)
{
  counter++;
  if (code != 0)
  {
    char* b = code2bin(code, length);
    String bin(b);

    if (deviceCounter == 0)
    {
      Serial.println("Scan started.");
      codes[0] = code;
      bins[0] = bin;
      deviceCounter++;
    }

    boolean found = false;
    for (int i = 0; i < deviceCounter; i++)
    {
      if (code == codes[i])
      {
        found = true;
      }
    }

    if (!found)
    {
      codes[deviceCounter] = code;
      bins[deviceCounter] = bin;
      deviceCounter++;
    }

    Serial.println();
    Serial.print(" Bin: ");
    Serial.println(b);
    Serial.println();
    for (int i = 0; i < deviceCounter; i++)
    {
      Serial.print("  0x");
      Serial.print(codes[i], HEX);
      Serial.print("0, // ");
      Serial.print(i);
      Serial.print(": Code: ");
      Serial.print(codes[i]);
      Serial.print(" Bin: ");
      Serial.println(bins[i]);
    }

    if (deviceCounter == CODE_COUNT)
    {
      Serial.println("Scan complete.");
      deviceCounter = 0;
      for (int i = 0; i < deviceCounter; i++)
      {
        bins[i] = "";
        codes[i] = 0;
      }
    }
  }
  else
  {
    Serial.print("Unknown encoding.");
  }
  Serial.println();
  Serial.println();
}

static char * code2bin(unsigned long code, unsigned int bitLength)
{
	static char bin[64];
	unsigned int i = 0;

	while (code > 0)
	{
		bin[32 + i++] = (code & 1 > 0) ? '1' : '0';
		code = code >> 1;
	}

	for (unsigned int j = 0; j< bitLength; j++)
	{
		if (j >= bitLength - i)
		{
			bin[j] = bin[31 + i - (j - (bitLength - i))];
		}
		else
		{
			bin[j] = '0';
		}
	}
	bin[bitLength] = '\0';

	return bin;
}
