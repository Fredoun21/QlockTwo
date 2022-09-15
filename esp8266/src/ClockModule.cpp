//
//  ClockModule.cpp
//  esp8266
//
//  Created by <author> on 22/02/2018.
//
//

#include "ClockModule.h"
#include <Arduino.h>

#ifndef countof
#define countof(array) (sizeof(array) / sizeof(array[0]))
#endif

/**
 * ClockModule gère le RTC et le maintient à jour via NTP.
 * @param _rtc Arduino Wire object
 * @param _updateInterval Intervalle de mise à jour RTC en secondes
 * @param _ntpServerName Adresse du serveur NTP.
 */
ClockModule::ClockModule(RtcDS1307<TwoWire> _rtc, Timezone _localTZ, String _ntpServerName) : rtc(_rtc), localTZ(_localTZ), ntpServerName(_ntpServerName) {}

ClockModule::~ClockModule() {}

/**
 * Configurez le client NTP et la connexion RTC.
 */
void ClockModule::setup()
{
    ntpUDP.begin(localPort);

    rtc.Begin();

    // ne présumez jamais que le Rtc a été configuré pour la dernière fois par vous, donc
    // effacez-le simplement dans l'état dont vous avez besoin
    // rtc.Enable32kHzPin(false); //uniquement DS3231
    rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);
}

/**
 * Soit vrai si l'heure rtc n'est pas valide ou si l'intervalle de mise à jour est atteint.
 * @return
 */
bool ClockModule::isDateTimeValid()
{
    Serial.printf("Date valide: %i\n",rtc.IsDateTimeValid());
    return rtc.IsDateTimeValid();
}

/**
 * Afficher RtcDateTime sur Serial.
 * @param dt
 */
void printDateTime(const RtcDateTime &dt)
{
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(),
               dt.Day(),
               dt.Year(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    Serial.println(datestring);
}

/**
 * Afficher time_t sur Serial.
 * @param dt
 */
void printDateTime(const time_t &dt)
{
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               month(dt),
               day(dt),
               year(dt),
               hour(dt),
               minute(dt),
               second(dt));
    Serial.println(datestring);
}

/**
 * Obtenez l'heure actuelle du serveur NTP et mettez à jour RTC.
 * @return vrai si la mise à jour a réussi
 */
void ClockModule::update()
{
    Serial.println("\n ClockModule: Mise à jour de l'horloge.");

    int ntpAttempt = 0;
    time_t ntpTime = 0;
    while (ntpTime == 0 && ntpAttempt < 10)
    {
        ntpTime = getNtpTime();
        ntpAttempt++;
    }

    if (ntpTime == 0)
    {
        Serial.println("ClockModule: Echec de la mise à jour de l'horloge.");
        return;
    }

    printDateTime(ntpTime);
    Serial.println(ntpTime);

    RtcDateTime ntpRtcDateTime;
    ntpRtcDateTime.InitWithEpoch32Time(ntpTime);
    Serial.print("Convertion NTPtime: ");
    printDateTime(ntpRtcDateTime);

    rtc.SetDateTime(ntpRtcDateTime);
}

/**
 * Convertir time_t en SimpleTime
 * @param time
 * @return Converted SimpleTime
 */
SimpleTime convertToSimpleTime(const time_t &time)
{
    return SimpleTime(hour(time), minute(time));
}

/**
 * Obtenez l'heure actuelle de RTC en UTC.
 * @return Heure actuelle en tant que time_t en UTC.
 */
time_t ClockModule::getUtcTime()
{
    return rtc.GetDateTime().Epoch32Time();
}

/**
 * Obtenez l'heure actuelle du RTC en heure locale. Avec réglage du fuseau horaire et de l'heure d'été.
 * @return Current Local Time as SimpleTime.
 */
SimpleTime ClockModule::getLocalSimpleTime()
{
    return convertToSimpleTime(localTZ.toLocal(getUtcTime()));
}

/*-------- NTP code ----------*/

/**
 * Obtenez l'heure NTP à partir du serveur NTP défini via UDP.
 * @return
 */
time_t ClockModule::getNtpTime()
{
    IPAddress ntpServerIP; // Adresse IP du serveur NTP

    while (ntpUDP.parsePacket() > 0)
        ; // rejeter tous les paquets précédemment reçus

    Serial.println("Transmit NTP Request");
    // obtenir un serveur aléatoire du pool
    WiFi.hostByName(ntpServerName.c_str(), ntpServerIP);
    Serial.print(ntpServerName);
    Serial.print(": ");
    Serial.println(ntpServerIP);
    this->sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500)
    {
        int size = ntpUDP.parsePacket();
        if (size >= NTP_PACKET_SIZE)
        {
            Serial.println("Receive NTP Response");
            ntpUDP.read(packetBuffer, NTP_PACKET_SIZE); // lire le paquet dans la mémoire tampon
            unsigned long secsSince1900;
            // convertir quatre octets commençant à l'emplacement 40 en un entier long
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            return secsSince1900 - 2208988800UL;
        }
    }
    Serial.println("No NTP Response :-(");
    return 0; // renvoie 0 si impossible d'obtenir l'heure
}

/**
 * Envoyer une requête NTP au serveur de temps à l'adresse donnée
 * @param address IP address
 */
void ClockModule::sendNTPpacket(IPAddress &address)
{
    // mettre tous les octets du tampon à 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialiser les valeurs nécessaires pour former la requête NTP
    // (voir URL ci-dessus pour plus de détails sur les paquets)
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0;          // Stratum, or type of clock
    packetBuffer[2] = 6;          // Polling Interval
    packetBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 octets de zéro pour Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    // tous les champs NTP ont reçu des valeurs, maintenant
    // vous pouvez envoyer un paquet demandant un horodatage :
    ntpUDP.beginPacket(address, 123); // Les requêtes NTP sont destinées au port 123
    ntpUDP.write(packetBuffer, NTP_PACKET_SIZE);
    ntpUDP.endPacket();
}