#include "Arduino.h"
#include "SPI.h"
#include "SpiUart.h"
#include "WifiS.h"

WifiS::WifiS(){
	
	SpiSerial.begin(115200);
}
/*
Secondo livello: sarebbero i comandi secchi da datasheet
*/
boolean WifiS::check() {
	this->send_command("AT\r");
	return this->riceive(0);
}
boolean WifiS::resetChip(char* mode) {
	char toSend[9];
	strcpy(toSend, "AT+CFUN=");
	strcat(toSend, mode);
	strcat(toSend, "\r");
	Serial.println(toSend);
	this->send_command(toSend); // "AT+CFUN=0\r"
	return this->riceive(2); // becco l'errore 2 perchè NON restituisce OK
}
boolean WifiS::miniAP() {
	this->send_command("AT+S.SCFG=wifi_mode,3\r");
	return this->riceive(0);
}
boolean WifiS::station(){
	this->send_command("AT+S.SCFG=wifi_mode,1\r");
	return this->riceive(0);
}
boolean WifiS::factoryReset() {
	this->send_command("AT&F\r");
	return this->riceive(0);
}
boolean WifiS::saveCurrent() {
	this->send_command("AT&W\r");
	return this->riceive(0);
}
boolean WifiS::wifiEnable() {
	this->send_command("AT+S.WIFI=1\r");
	return this->riceive(0);
}
boolean WifiS::wifiDisable() {
	this->send_command("AT+S.WIFI=0\r");
	return this->riceive(0);
}
/*
Funzioni di primo livello
riceive(); accetta un errore da controllare
lo controlla tramite la funzione riceive_error()

e la funzione send_command(); che si capisce cosa fa :)

*/
boolean WifiS::riceive(int error_user){
  int pos = 0;
  char current_char;

  unsigned long start = millis();
  int Timeout = 7000;
  
  while (millis() < (start + Timeout)) {
    pos = 0;
    if( SpiSerial.read() == '\n' ) { // primo \n
      current_char = SpiSerial.read();
      switch (current_char) {
        case 'O':
          current_char = SpiSerial.read();
          if (current_char == 'K') return true;
          break;
        case '+':
          int errore = this->riceive_error();
          if (errore == error_user) return true;
          break;
      }
    }
    delay(1);
  }

  return false;
}

int WifiS::riceive_error() {
  char buffer_error[2];
  int pos = 0;
  char current_char;

  int error;
  while (SpiSerial.available()) {
    if (SpiSerial.read() == ':'){
      while(true) {
       current_char = SpiSerial.read();
       if (current_char == ':') {
         error=atoi(buffer_error);
         // Serial.print("errore: ");
         // Serial.println(error);
         return error;
       }
       buffer_error[pos] = current_char;
       pos++;
      } 
    }
  }
}

void WifiS::send_command(char* comando) {
	// vuoto il buffer prima di inviare il comando
	while(SpiSerial.available()) {
		SpiSerial.read();
	}
	
	SpiSerial.write(comando);
	delay(20);
} 