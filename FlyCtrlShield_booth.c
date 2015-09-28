/* FILE NAME: FlyCtrlShield_booth.c
 * The MIT License (MIT)
 * 
 * Copyright:	(c) 2015 Lapis Semiconductor
 * Author: 		Naotaka Saito
 * Division:	New Business Development Project
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

unsigned long prev_time;
#define MON	A3

#define LED				26			// pin number of Blue LED
#define ORANGE_LED 		25			// pin number of Blue LED
#define SUBGHZ_CH		24			// channel number (frequency)
#define PANID			0xABCD		// panid
#define HOST_ADDRESS	0x10		// distination address

short data_packet[4];

void setup(void)
{
	int i;
	SUBGHZ_PARAM param;
	for(i=3;i<=8;i++)
	{
		pinMode(i,INPUT_PULLUP);
	}
	digitalWrite(MON,HIGH);
	pinMode(MON,OUTPUT);
	prev_time = 0;
	
	SubGHz.init();					// initializing Sub-GHz
	SubGHz.getSendMode(&param);
	param.addrType = 4;
	SubGHz.setSendMode(&param);

	pinMode(LED,OUTPUT);			// setting of LED
	pinMode(ORANGE_LED,OUTPUT);			// setting of LED
	digitalWrite(LED,HIGH);			// setting of LED
	Serial.begin(115200);
}



void loop(void)
{
	unsigned long current_time;
	short rx;
	short ry;
	short rz;
	unsigned char sw;
	int i;
	SUBGHZ_MSG msg;
	
	digitalWrite(MON,LOW);
	data_packet[0]=analogRead(A0) - 512;
	data_packet[1]=analogRead(A1) - 512;
	data_packet[2]= (analogRead(A2)>=512)? 512:0;
	
	sw=0;
	for(i=3;i<=8;i++)
	{
		sw = (sw << 1) + ((digitalRead(i) == LOW) ? 0 : 1);
	}
	sw = (~sw)&0x3F;
	data_packet[3]=(short)sw;
	
	SubGHz.begin(SUBGHZ_CH, PANID,  SUBGHZ_100KBPS, SUBGHZ_PWR_20MW);			// start Sub-GHz
	
	// preparing data
	digitalWrite(LED,LOW);														// LED ON
	while(1)
	{
		msg = SubGHz.send(PANID, HOST_ADDRESS, (unsigned char*)&data_packet, sizeof(data_packet),NULL);		// send data
		if(msg == SUBGHZ_TX_CCA_FAIL)
		{
			digitalWrite(ORANGE_LED,LOW);
		}
		else
		{
			digitalWrite(ORANGE_LED,HIGH);
			break;
		}
	}
	digitalWrite(LED,HIGH);														// LED off
	Serial.print_long((long)data_packet[0],DEC);
	Serial.print(",");
	Serial.print_long((long)data_packet[1],DEC);
	Serial.print(",");
	Serial.print_long((long)data_packet[2],DEC);
	Serial.print(",");
	Serial.print_long((long)data_packet[3],DEC);
	Serial.println("");
	// close
	SubGHz.close();																// Sub-GHz module sets into power down mode.
	delay(50);

}
