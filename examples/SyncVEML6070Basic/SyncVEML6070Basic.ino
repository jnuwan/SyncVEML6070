#include <SyncVEML6070.h>
/**
**written by Synclones (http://synclones.com) pvt Ltd.
**Reduced power consumption
**As per test sleeping mode current consumption <1uA
*/

SyncVEML6070 uv = SyncVEML6070();

void setup() {
  Serial.begin(9600);
  Serial.println("VEML6070 Test");
	uv.begin(SyncVEML6070::VEML6070_1_T); // T=1
}

void loop() {
	Serial.print("UV light level: ");
	Serial.println(uv.getUV());
	delay(5000);
}
