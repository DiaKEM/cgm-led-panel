#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include "WiFi.h"
#include <ArduinoJson.h>
#include <time.h>
#include <NTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 14
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
                                               NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
                                                   NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                                               NEO_GRB + NEO_KHZ800);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 0, 5000);

const char *ssid = "WLAN_SSID";
const char *password = "WLAN_PASSWORD";

const char *serverName = "https://my-diakem-instance/api/v1/graphql";
String appToken = "MY_APP_TOKEN";

void setupMatrix()
{
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(10);
}

u_int16_t green()
{
  return matrix.Color(71, 240, 65);
}

u_int16_t yellow()
{
  return matrix.Color(245, 255, 69);
}

u_int16_t orange()
{
  return matrix.Color(255, 156, 25);
}

u_int16_t red()
{
  return matrix.Color(245, 71, 71);
}

u_int16_t blue()
{
  return matrix.Color(71, 93, 237);
}

String getTrend(String trend)
{
  if (trend.equalsIgnoreCase("DoubleDown"))
  {
    return "uu";
  }
  else if (trend.equalsIgnoreCase("SingleDown"))
  {
    return "u";
  }
  else if (trend.equalsIgnoreCase("FortyFiveDown"))
  {
    return "u>";
  }
  else if (trend.equalsIgnoreCase("Flat"))
  {
    return ">";
  }
  else if (trend.equalsIgnoreCase("FortyFiveUp"))
  {
    return "^>";
  }
  else if (trend.equalsIgnoreCase("SingleUp"))
  {
    return "^";
  }
  else if (trend.equalsIgnoreCase("DoubleUp"))
  {
    return "^^";
  }
  else
  {
    return "?";
  }
}

u_int16_t getTrendColor(String trend)
{
  if (trend.equalsIgnoreCase("DoubleDown"))
  {
    return red();
  }
  else if (trend.equalsIgnoreCase("SingleDown"))
  {
    return orange();
  }
  else if (trend.equalsIgnoreCase("FortyFiveDown"))
  {
    return yellow();
  }
  else if (trend.equalsIgnoreCase("Flat"))
  {
    return green();
  }
  else if (trend.equalsIgnoreCase("FortyFiveUp"))
  {
    return yellow();
  }
  else if (trend.equalsIgnoreCase("SingleUp"))
  {
    return orange();
  }
  else if (trend.equalsIgnoreCase("DoubleUp"))
  {
    return red();
  }
  else
  {
    return blue();
  }
}

int getTimeDifference(const char *date)
{
  timeClient.update();
  struct tm tm;
  strptime(date, "%FT%X.000Z", &tm);
  time_t x = mktime(&tm);

  return timeClient.getEpochTime() - x;
}

u_int16_t getColor(int value)
{
  if (value <= 60)
  {
    return red();
  }
  else if (value <= 80)
  {
    return orange();
  }
  else if (value <= 130)
  {
    return green();
  }
  else if (value <= 160)
  {
    return yellow();
  }
  else if (value <= 200)
  {
    return orange();
  }
  else
  {
    return red();
  }
}

u_int16_t getValueDifferenceColor(int valueDifference)
{
  int positive = valueDifference < 0 ? valueDifference * -1 : valueDifference;
  // ROT: 245, 71, 71
  // ORANGE: 255, 156, 25
  // GELB: 245, 255, 69
  // GRÜN: 71, 240, 65
  if (positive <= 5)
  {
    return green();
  }
  else if (positive <= 10)
  {
    return yellow();
  }
  else if (positive <= 15)
  {
    return orange();
  }
  else
  {
    return red();
  }
}

void freeze()
{
  delay(2000);
}

void resetMatrix() {
  matrix.fillScreen(0);
  matrix.setCursor(0, 0);
  matrix.setTextWrap(false);
}

void showError(String msg)
{
  resetMatrix();
  matrix.setTextColor(red());
  matrix.print("X " + msg);
  matrix.show();
}

void showApiProblem()
{
  showError("API-Error...");
}

void showWlanProblem()
{
  showError("WLAN-Error...");
}

void showTimeDifference(int timeDifference)
{
  resetMatrix();
  matrix.setTextWrap(true);
  matrix.setTextColor(blue());
  matrix.print(timeDifference);
  matrix.print("s");
  matrix.show();
}

void showBloodSugarDifference(int valueDifference, String valueSign)
{
  resetMatrix();
  matrix.setTextColor(getValueDifferenceColor(valueDifference));
  matrix.print(valueSign + valueDifference);
  matrix.show();
}

void showBloodSugar(int value, String trend)
{
  resetMatrix();
  matrix.setTextColor(getColor(value));
  matrix.print(value);
  matrix.setTextColor(getTrendColor(trend));
  matrix.print(getTrend(trend));
  matrix.show();
}

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  timeClient.begin();
  setupMatrix();

  resetMatrix();
  showError("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to the WiFi network");
}


void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-API-KEY", appToken);
    String httpRequestData = "{\"query\": \"query Query {bloodSugarAlternativeEventMany(limit: 2, sort: DATE_DESC) {date value trend}}\"}";

    // Send HTTP POST request
    int code = http.POST(httpRequestData);

    if (code != 200)
    {
      showApiProblem();
      freeze();
      return;
    }

    String response = http.getString();
    http.end();

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, response);
    int value = doc["data"]["bloodSugarAlternativeEventMany"][0]["value"];
    int valueOld = doc["data"]["bloodSugarAlternativeEventMany"][1]["value"];
    int valueDifference = value - valueOld;
    String valueSign = valueDifference == 0 ? "+/-" : valueDifference > 0 ? "+"
                                                                          : "";
    String trend = doc["data"]["bloodSugarAlternativeEventMany"][0]["trend"];
    int timeDifference = getTimeDifference(doc["data"]["bloodSugarAlternativeEventMany"][0]["date"]);
    
    showBloodSugar(value, trend);
    freeze();
    showBloodSugarDifference(valueDifference, valueSign);
    freeze();
    showTimeDifference(timeDifference);
    freeze();
  }
  else
  {
    showWlanProblem();
    freeze();
    return;
  }
}