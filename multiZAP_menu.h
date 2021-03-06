/*
 * multiZAP_menu.h
 *
 *  Created on: 31 dec 2017
 *      Author: elektros230
 */

#ifndef MULTIZAP_MENU_H_
#define MULTIZAP_MENU_H_


//#include "EEPROM.h"
//#include "LiquidCrystal_I2C.h"
//#include <Arduino.h>
#include "multiZAP_def.h"
//#include "multiZAP_calib.h"
#include "multiZAP_lcd.h"
#include "bioZAP_func.h"






void key_A(){
    //Freq = inputVal("Input frequency", 100000);
    Freq = inputVal(12, 100000);
    if (Freq>0) {

    	unsigned int period = inputVal(3, 10);

    	line="freq ";
    	line+=String(Freq);
    	line+=" ";
    	line+=String(period);
    	message(line,1);

		if (calib()) {
			message(9);
			delay(3000);
		}

    	freq(Freq, period);
    }

}

/*void key_B(){
	TODO: Configuration parameters
	 * ARM STM32 Cortex M3 only
	 * vmin - EEPROM
	 * vampl - EEPROM
	 * auto_turn_off_time - EEPROM
	 * AD985x 0/1 - EEPROM
	 * default_program - EEPROM
	 * multiZAP -/+/++ - EEPROM
	 * auto off after script therapy end EEPROM


}*/


void key_C(){
	// Calibrate - set new vmin and vampl and saving in EEPROM

		//long currentFreq = ad9850.get_frequency();
		get_v_EEPROM();

		//int vampl = inputVal("Input vampl", last_v_ampl,3);
		int vampl = inputVal(4, last_v_ampl,3);
		//int vmin = inputVal("Input vmin", last_v_min, 3);
		int vmin = inputVal(5, last_v_min, 3);

		//Freq = 100000; //inputVal("Input freq", 100000);

		//message("Calibrating...");
		message(0);
		if (Freq){
			//during working
			wiper0 = calib_gain_wiper_ampl(vampl, Freq);
		} else {
			//not working
			wiper0 = calib_gain_wiper_ampl(vampl, 100000);
		}
		wiper1 = calib_setp_wiper_vmin(vmin);

		if ( wiper0 * wiper1 ){
			//Save last vampl and vmin
			set_v_EEPROM();
			//message("Saved in memory",0);
			message(6,0);
		} else {
			//message("Wiper's error!",0);
			message(7,0);
		}
		//Off if at calibration begun from wipers off
		if (!Freq) wipersOFF();



		message("w0:");
		lcd.print(wiper0);
		lcd.print("  w1:");
		lcd.print(wiper1);
		delay(5000);

}


void key_D(){
	//SD card express scan
	/*
	 * ARM ST32 Cortex M3 only
	 */

}

void key_asterix(){
//Manually turn device off
	//int ret=inputVal("Off?  1-yes 0-no", 1);
	int ret=inputVal(8, 0);
	if (ret) digitalWrite(powerPin, LOW);

}



void key_0(){
	if (currentProgram==-1){
		if ((byte)EEPROM.read(0)!=255 && (byte)EEPROM.read(0)!=0) {
			//User program execute

			message(10); //"User program ...",
			delay(1000);
			currentProgram=0;

			if (calib()) {
				message(9);
				delay(3000);
			}

			programStartMillis=millis();

		} else {

			message(11); //"No program in memory!"
		}
	}
}
void keys_1_9(byte prog){

		if (currentProgram==-1){

			line="Program: ";
			line+=prog;
			message(line);
			delay(1000);
			adr=0;
			currentProgram = prog;

			if (calib()) {
				message(9);
				delay(3000);
			}

			programStartMillis=millis();
		}


/*		if ( !( (wiper0 = calib_gain_wiper_ampl(last_v_ampl, 100000)) > 0  &&
				(wiper1 = calib_setp_wiper_vmin(last_v_min)) > 0                  ) ) {

			message(9); //"Error calibration"
			delay(3000);

			//to do callibration wrong
		} else {
			delay(1000);

			adr=0; //?
			currentProgram = prog;
			programStartMillis=millis();
		}*/

}



void keyPressed(char key){

	switch (key){

		  case 'A':
			  if (!programStopMillis && !freqStopMillis){
				  key_A();
			  //key=NO_KEY;
			  }
			  break;

/*		  case 'B':
			  key_B();
			  break;*/

		  case 'C':
			  key_C();
			  break;

		  case 'D':
			  key_D();
			  break;

		  case '*':
			  key_asterix();
			  break;

		  case '1' ... '9':

		  	  keys_1_9(int(key)-48);
			  break;

		  case '0':
			  if (!programStopMillis && !freqStopMillis){
				  key_0();
			  }
			  break;

	}
}

#endif /* MULTIZAP_MENU_H_ */
