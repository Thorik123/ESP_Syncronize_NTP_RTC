#include <RTClib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#define DS1307
#define DS3231

const char *ssid = "Polsek Batealit";
const char *password = "thoriktronik";

#ifdef DS1307
RTC_DS1307 rtc;
#elif DS3231
RTC_DS3231 rtc;
#endif

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "id.pool.ntp.org");

char daysOfTheWeek[7][12] = {
  "Minggu",
  "Senin",
  "Selasa",
  "Rabu",
  "Kamis",
  "Jum'at",
  "Sabtu"
};

String weekDays[7] = {
  "Minggu",
  "Senin",
  "Selasa",
  "Rabu",
  "Kamis",
  "Jum'at",
  "Sabtu"
};

String months[12] = {
  "Januari",
  "Februari",
  "Maret",
  "April",
  "Mei",
  "Juni",
  "Juli",
  "Agustus",
  "September",
  "Oktober",
  "November",
  "Desember"
};

unsigned long lastTime;

void setup() {
  Serial.begin(115200);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
#ifdef DS1307
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
#elif DS3231
  if (!rtc.lostPower()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
#endif

  Serial.print("Connecting WiFi to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();
  timeClient.setTimeOffset(25200);
  timeClient.setUpdateInterval(5000);
  timeClient.forceUpdate();
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);

  if (timeClient.isTimeSet()) {
    rtc.adjust(DateTime(ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()));
    Serial.print(F("Setting time success\n"));
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
  } else {
    Serial.print(F("Setting time failed\n"));
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
  }
}

void loop() {
  if (millis() - lastTime >= 1000) {
    showRTCTime();
    showNTPTime();
    lastTime = millis();
  }
}

void showRTCTime() {
  DateTime now = rtc.now();
  Serial.println("RTC Time");
  Serial.println(String("Formatted Time: ") + now.timestamp(DateTime::TIMESTAMP_TIME));
  Serial.println(String("Formatted Date: ") + now.timestamp(DateTime::TIMESTAMP_DATE));
  Serial.println();
}

void showNTPTime() {
  timeClient.update();
  Serial.println("NTP Time");
  time_t epochTime = timeClient.getEpochTime();
  // Serial.print("Epoch Time: ");
  // Serial.println(epochTime);
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);
  int currentHour = timeClient.getHours();
  // Serial.print("Jam: ");
  // Serial.println(currentHour);
  int currentMinute = timeClient.getMinutes();
  // Serial.print("Menit: ");
  // Serial.println(currentMinute);
  int currentSecond = timeClient.getSeconds();
  // Serial.print("Detik: ");
  // Serial.println(currentSecond);
  String weekDay = weekDays[timeClient.getDay()];
  // Serial.print("Hari: ");
  // Serial.println(weekDay);
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  // Serial.print("Tanggal: ");
  // Serial.println(monthDay);
  int currentMonth = ptm->tm_mon + 1;
  // Serial.print("Bulan: ");
  // Serial.println(currentMonth);
  String currentMonthName = months[currentMonth - 1];
  // Serial.print("Nama bulan: ");
  // Serial.println(currentMonthName);
  int currentYear = ptm->tm_year + 1900;
  // Serial.print("Tahun: ");
  // Serial.println(currentYear);
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Formatted Date: ");
  Serial.println(currentDate);
  Serial.println();
}