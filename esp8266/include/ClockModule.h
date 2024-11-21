//
//  ClockModule.h
//  esp8266
//
//  Created by <author> on 22/02/2018.
//
//

#ifndef ClockModule_H
#define ClockModule_H

#include <stdio.h>
#include <Wire.h> // doit être inclus ici pour que les références de fichiers d'objets de la bibliothèque Arduino fonctionnent
#include <RtcDS1307.h>
#include <Timezone.h>   // https://github.com/JChristensen/Timezone Créer fichier library.json
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "SimpleTime.h"

/**
 * ClockModule gère le RTC et le maintient à jour via NTP.
 */
class ClockModule
{
public:
  ClockModule(RtcDS1307<TwoWire> _rtc, Timezone _localTZ, String _ntpServerName);
  ~ClockModule();
  void setup();
  bool isDateTimeValid();
  void update();
  int getMonth();
  time_t getUtcTime();
  SimpleTime getLocalSimpleTime();

private:
  RtcDS1307<TwoWire> rtc;
  Timezone localTZ;
  WiFiUDP ntpUDP;
  String ntpServerName;
  static const int NTP_PACKET_SIZE = 48; // L'heure NTP est dans les 48 premiers octets du message
  byte packetBuffer[NTP_PACKET_SIZE];    // tampon pour contenir les paquets entrants et sortants
  unsigned int localPort = 8888;         // port local pour écouter les paquets UDP
  time_t getNtpTime();
  void sendNTPpacket(IPAddress &address);
  // void printDateTime(const RtcDateTime &dt);
  // SimpleTime convertToSimpleTime(const time_t &time);

protected:
};

#endif /* ClockModule_h */
