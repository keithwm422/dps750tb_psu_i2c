/* -*- tab-width: 2; mode: c; -*-
 * 
 * Program to extract some basic data from a power supply that supports PMBus and present it. 
 *
 * Copyright (c) 2020, Steve Jack.
 *
 * MIT Licence.
 *
 * Use at your own risk.
 * Keith McBride forking for use with TM4C123 launchpad for projects 
 * 23/09    TM4C123 support and remove other items
 * 22/12    Raspberry Pi Pico W support.
 * 
 * 21/01/15 The ESP32 sometimes loses the WiFi connection (see issues below).
 *          Diagnostics added. Code added to loop() to check for a connection.
 * 
 *
 * Issues
 * 
 * Notes
 *
 */

#pragma GCC diagnostic warning "-Wunused-variable"

#include <Arduino.h>
#include <Wire.h>

#include "pmbus.h"

#define OUTPUT_DIRECTION  0
#define DebugSerial    Serial
//static const int blink_do = PC13, PSON_do = PB14, I2C_enable_do = PB15;
void yield(void);

//static const int blink_do =  2, PSON_do =  5, I2C_enable_do = 15;
static const int blink_do = 20, PSON_do = 21, I2C_enable_do = 22;
//static const int blink_do = 13, PSON_do = 12, I2C_enable_do = 11;
static const char     *title = "PMBus", *build_date = __DATE__;
static PMBus           psu;

void setup() {

  int  status, i;
  char text[128];

  text[0] =
  status  = i = 0;
  pinMode(blink_do,OUTPUT);
  Wire.begin(); // which wire?
  // Using Wire1 on 12 & 14 crashes the program.
  //Wire1.begin(27,26,100000);
  DebugSerial.begin(115200);
  Wire.setClock(100000);
  DebugSerial.print("\r\n");
  DebugSerial.print((char *) title);
  DebugSerial.print("\r\n");
  DebugSerial.print((char *) build_date);
  DebugSerial.print("\r\n\n");
  delay(100);

  //psu.init(PSON_do,I2C_enable_do,OUTPUT_DIRECTION,0x58,&DebugSerial,&Wire1);
  yield();

  psu.on();

  return;
}

/*
 * 
 */

void loop() {

  int              i;
  char             text[128];
  uint32_t         run_msecs, run_secs;
  static int       blink = 0;
  static char      I_out_s[8] = {0}, V_out_s[8] = {0};
  static uint32_t  last_display_update = 0, last_scan = 0;

  text[0] = i = 0; // Because I can't be bothered to sort out #if's

  //

  run_msecs = millis(); // run time
  run_secs  = run_msecs / 1000;

  if (psu.scan()) {
    digitalWrite(blink_do,blink ^= 1);
    last_scan = run_secs;
    dtostrf(psu.V_out,5,2,V_out_s);
    dtostrf(psu.I_out,5,2,I_out_s);
  }
  yield();

      //sprintf(lcd_line_0,"%4sV %4sA   ",V_out_s,I_out_s);
        //(int) psu.T[0],(int) psu.T[1],(int) psu.fan[0]
        for (i = 0; i < 16; ++i) {
          //((psu.status_word >> i) & 0x01) ? '1': '0';
        }

      //sprintf(text,"   %3dV  %4dW  ",(int) psu.V_in,(int) psu.W_out);
      //sprintf(text," %sV %sA  ",V_out_s,I_out_s);
      //sprintf(text,"  %2dC  %4drpm  ",(int) psu.T[0],(int) psu.fan[0]);
      for (i = 0; i < 16; ++i) {
        //text[15 - i] = ((psu.status_word >> i) & 0x01) ? '1': '0';
      }
      //sprintf(text," %sV ",V_out_s);
      //sprintf(text," %sA ",I_out_s);
      //sprintf(text,"%2dC %4d",(int) psu.T[0],(int) psu.fan[0]);
      for (i = 0; i < 8; ++i) {
        //text[7 - i] = ((psu.status_word >> i) & 0x01) ? '1': '0';
      }
  return;
}

/*
  if ((request_s[5] == 'o')&&(request_s[6] == 'n')) {

    psu.on();

  } else if ((request_s[5] == 'o')&&(request_s[6] == 'f')&&(request_s[7] == 'f')) {

    psu.standby();
    
  } else if (strncmp(&request_s[5],"clear",5) == 0) {

    psu.clear_faults();
    
  } else if (strncmp(&request_s[5],"model",5) == 0) {

    psu.check_model();
  }

  //
  if (psu.W_in > 0.001) {

    efficiency = (int) (100.0 * psu.W_out / psu.W_in);
  }

  W_in2  = psu.V_in  * psu.I_in;
  W_out2 = psu.V_out * psu.I_out;

  if (W_in2 > 0.001) {

    efficiency2 = (int) (100.0 * W_out2 / W_in2);
  }  

  sprintf(text,"<h3 align=\"center\">%s</h3>\n",
          (psu.mfr_model[0]) ? &psu.mfr_model[1]: title);
  dtostrf(psu.I_in,5,2,text3);
  dtostrf(psu.W_in,6,1,text4);
  dtostrf(W_in2,6,1,text5);
  sprintf(text,"<tr><td>Input (V/A/W/VA)</td><td>%d</td><td>%s</td><td>%d</td><td>%d</td></tr>\n",
          (int) psu.V_in,text3,
          (int) (psu.W_in + 0.5),(int) (W_in2 + 0.5));
          // text4,text5);

  dtostrf(psu.V_out,5,2,text2);
  dtostrf(psu.I_out,5,2,text3);
  dtostrf(psu.W_out,6,1,text4);
  dtostrf(W_out2,6,1,text5);
  sprintf(text,"<tr><td>Output (V/A/W/VA)</td><td>%s</td><td>%s</td><td>%d</td><td>%d</td></tr>\n",
          text2,text3,
          (int) (psu.W_out + 0.5),(int) (W_out2 + 0.5));
          // text4,text5);

  sprintf(text,"<tr><td>Efficiency (%%)</td><td colspan=\"2\"></td><td>%d</td><!--<td>%d</td>--></tr>\n",
          efficiency,efficiency2);

  dtostrf(psu.T[0],4,1,text2);
  dtostrf(psu.T[1],4,1,text3);
  dtostrf(psu.T[2],4,1,text4);
  sprintf(text,"<tr><td>Temperatures (C)</td><td>%s</td><td>%s</td></tr>\n",
          text2,text3);

  sprintf(text,"<tr><td>Fan (rpm)</td><td>%d</td></tr>\n",(int) psu.fan[0]);


  for (i = 0, u16 = psu.status_word; i < 16; ++i) {

    sprintf(text,"<tr><td>%s</td><td>%s</td>",
            status_word_s[i],((u16 & 0x01) ? on_s: off_s));
    text[0] = 0;

    switch (i) {

    case  0:

      sprintf(text,"<td>%02x</td>",psu.status_other);
      break;      

    case  1:

      sprintf(text,"<td>%02x</td>",psu.status_cml);
      break;      

    case  2:

      sprintf(text,"<td>%02x</td>",psu.status_temperature);
      break;      

    case  3:
    case 13:

      sprintf(text,"<td>%02x</td>",psu.status_input);
      break;      

    case  4:
    case 14:

      sprintf(text,"<td>%02x</td>",psu.status_iout);
      break;      

    case 5:
    case 15:

      sprintf(text,"<td>%02x</td>",psu.status_vout);
      break;      

    case 12:

      sprintf(text,"<td>%02x</td>",psu.status_mfr_specific);
      break;      
    }

    if (text[0]) {

      client.print(text);
    }
    
    sprintf(text,"</tr>\n");
    client.print(text);

    u16 >>= 1;
  }

  client.print("</table>\n");

  sprintf(text,"<!-- pmbus revision  %02x -->\n",psu.pmbus_revision);
  client.print(text);
  sprintf(text,"<!-- vout mode       %02x -->\n",psu.vout_mode);
  client.print(text);
  sprintf(text,"<!-- vout command    %04x -->\n",psu.vout_command);
  client.print(text);
  sprintf(text,"<!-- model           \'%s\' -->\n",&psu.mfr_model[1]);
  client.print(text);
  sprintf(text,"<!-- revision        \'%s\' -->\n",&psu.mfr_revision[1]);
  client.print(text);
#if 0
  days = psu.total_power_on / 86400l;
  sprintf(text,"<!-- total power on  %lu s, %u day%s -->\n",
          psu.total_power_on,days,((days == 1) ? "": "s"));
  client.print(text);
#endif
#if defined(ARDUINO_ARCH_ESP32)
  sprintf(text,"<!-- wifi reconnects %d -->\n",wifi_reconnects);
  client.print(text);
  sprintf(text,"<!-- Free heap       %u bytes -->\n",ESP.getFreeHeap());
  client.print(text);
  sprintf(text,"<!-- Stack           0x%08x -->\n",&i);
  client.print(text);  
#endif
  secs = (unsigned long int) (millis() / 1000L);
  days = secs / 86400L;
  sprintf(text,"<!-- Run time        %lu secs (%u day%s) -->\n",
          secs,days,((days == 1) ? "": "s"));
  client.print(text);
  sprintf(text,"<!-- Build date      %s -->\n",build_date);
  client.print(text);
  sprintf(text,"<!-- \'%s\' -->\n",request_s);
  client.print(text);

  for (i = 0; html_footer[i]; ++i) {

    client.print(html_footer[i]);
  }

  return;
}

#endif


  //sprintf(buffer,"<166>1 %s: %s",
  //        ((psu.mfr_model[0]) ? &psu.mfr_model[1]: "pmbus"),message);

/*
 *
 */

 void yield(void) {

  return;
}
