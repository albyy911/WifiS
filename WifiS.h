/*
*/
#include "Arduino.h"
#include "SPI.h"
#include "SpiUart.h"
extern SpiUartDevice SpiSerial;

class WifiS {
	public:
		SpiUartDevice SpiSerial;
		WifiS();
		// funzioni secondo livello
		boolean check();
		boolean resetChip(char* mode);
		boolean miniAP();
		boolean station();
		boolean factoryReset();
		boolean saveCurrent();
		boolean wifiEnable();
		boolean wifiDisable();
	private:
		boolean riceive(int error_user);
		int riceive_error();
		void send_command(char* comando);
};