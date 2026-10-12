#include <SoftwareSerial.h>

SoftwareSerial BTSerial(3,4);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTSerial.begin(115200);
  BTSerial.write("AT+BAUD");
  BTSerial.write('\r');
  BTSerial.write('\n');
  char mesaj[32];
  BTSerial.readBytes(mesaj,32);
  Serial.write(mesaj);
  BTSerial.write("AT+RESET");
  BTSerial.write('\r');
  BTSerial.write('\n');
}

void loop() {
  // put your main code here, to run repeatedly:

}
