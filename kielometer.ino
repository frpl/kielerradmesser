/*

  kiel-o-meter Abstandsmesser

  frank.pliquett@gmx.de

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Dieses Programm ist Freie Software: Sie können es unter den Bedingungen
  der GNU General Public License, wie von der Free Software Foundation,
  Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
  veröffentlichten Version, weiter verteilen und/oder modifizieren.

  Dieses Programm wird in der Hoffnung bereitgestellt, dass es nützlich sein wird, jedoch
  OHNE JEDE GEWÄHR, sogar ohne die implizite
  Gewähr der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
  Siehe die GNU General Public License für weitere Einzelheiten.

  Siehe <https://www.gnu.org/licenses/>.

*/

#include <Arduino.h>

// Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)
#include <U8x8lib.h>

// für die Verwendung mit einem Wemos ESP2 mit OLED-Display

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(4, 5);

#include "BluetoothSerial.h" 
BluetoothSerial ESP_BT;

// die beiden Verbindungen zum HC-SR04 Sensor 
// siehe: https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

int trigPin = 15;    // Trigger
int echoPin = 13;    // Echo

unsigned long mtimer;
unsigned duration;
unsigned minimum;
unsigned distance;
unsigned olddist;
char line1[8];
char line2[8];
boolean transmit;

// bis zu dieser Grenze soll gemessen werden (in cm)

#define LIMIT   250


void setup(void)
{
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_victoriabold8_r);
  u8x8.drawString(0, 0, "Kiel-o-Meter");

  u8x8.drawString(0, 3, "ESP32/HC-SR04");
  sprintf (line1, "Limit %d", LIMIT);
  u8x8.drawString(0, 4, line1);
  u8x8.drawString(0, 6, "fp Mai 2019");
  delay (2000);
  u8x8.clear();

  // einmalig auf das Display zu schreiben:
  u8x8.drawString(13, 2, "cm");
  u8x8.drawString(1, 6, "min");
  u8x8.drawString(13, 6, "cm");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  ESP_BT.begin("Kiel_o_Meter"); // Name des bluetooth-Devices
  
  olddist= LIMIT;
  transmit= true;
}

void loop(void)
{

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance
  distance = (duration / 2) / 29.1;   // Divide by 29.1 or multiply by 0.0343
  if (distance > LIMIT) {
    distance = LIMIT;
    u8x8.drawString(14, 1, "x"); // Marker für "Limit überschritten"
  } else {
    u8x8.drawString(14, 1, " ");
  }

  if (distance < minimum) {
    // neues Minimum gefunden, anzeigen, timer setzen
    minimum = distance;
    mtimer = millis() + 3000; // 3s lang "halten"
  }

  // timer für minimum-Anzeige abgelaufen ?
  if (millis() > mtimer) {
    minimum = distance;
  }

  if (distance <LIMIT) {
    sprintf (line1, "%3d", distance);
  } else {
    strcpy (line1, "---");
  }
  if (minimum < LIMIT) {
    sprintf (line2, "%3d", minimum);
  } else {
    strcpy (line2, "---");
  }
  u8x8.draw2x2String(5, 1, line1);
  u8x8.draw2x2String(5, 5, line2);

  if ((distance == LIMIT) && transmit) {
    transmit= 1; // jetzt noch einmal LIMIT senden, dann nicht mehr
  }

  // immer senden, wenn unterhalb des Limits
  if ((distance < LIMIT)) {
    sprintf (line1, "%d;", distance);
    ESP_BT.print (line1);
    transmit= true;
  } else if (transmit) { // oberhalb des Limit nur einmal senden
    strcpy (line1, "---;");
    ESP_BT.print (line1);
    transmit= false; // jetzt nicht mehr

  }

  olddist= distance;
  // ca. 5 Messungen pro Sekunden sind aureichend
  delay (200);
} 
