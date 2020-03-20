/*
  ESP8266 LPG Leakage Email Sender

  Monitors the LPG sensor output 500 times and takes the average of the sampling to reduce reading noise,
  and if the value is greater than the threshold value, it sends email to the recipient .

  Before uploading the code make to sure to change in the Gsender header file:
    const int SMTP_PORT = 465;// STMP port change it if you use other smtp
    const char* SMTP_SERVER = "smtp.gmail.com";// gmail smtp server change if you use other smtp
    const char* EMAILBASE64_LOGIN = "YOUR EMAIL ID ENCODED WITH BASE64";// your email must me encoded with base64
    const char* EMAILBASE64_PASSWORD = "YOUR PASSWORD ENCODED WITH BASE64";// your password must be encoded with base64
    const char* FROM = "youremail@gmail.com";//your email address without encoding 
  
  Change these lines as per your need:
    const int sensorPin = A0; //connect sensor A0 output to this pin
    const char *ssid = "**********";    // WIFI network name
    const char *password = "*********"; // WIFI network password
    uint8_t connection_state = 0;       // Connected to WIFI or not
    uint8_t nSampling = 500;            //change as per your need
    uint16_t threshold = 200;           // change as per your need

  Parts list:
  NodeMCU :
  https://www.amazon.in/Generic-Nodemcu-Esp8266-Internet-Development/dp/B07262H53W/ref=sr_1_1?keywords=nodemcu&qid=1583588225&sr=8-1
  Gas Sensor:
  https://www.amazon.in/REES52-Arduino-Compatible-Sensor-Methane/dp/B018GW70G0/ref=sr_1_2?crid=2O01SJT84RKL&keywords=mq4+gas+detector+sensor&qid=1583588254&sprefix=mq4+%2Caps%2C390&sr=8-2
  Jumper Wires:
  https://www.amazon.in/Electrobot-Jumper-Wires-120-Pieces/dp/B071VQLQQQ/ref=sr_1_5?keywords=jumper+wire&qid=1583588298&sr=8-5


  Project made and maintained by Kumar Aditya


  The source code along with libraries can be found at:
  https://github.com/rahuladitya303/programs/tree/master/ESP8266_LPG_Leakage_Email_Sender

  Base64 Encode: https://www.base64encode.org/
*/

#include <ESP8266WiFi.h>
#include "Gsender.h"

const int sensorPin = A0;           //connect sensor A0 output to this pin
const char *ssid = "AndroidAPB9CF";    // WIFI network name
const char *password = "rahul12345"; // WIFI network password
uint8_t connection_state = 0;       // Connected to WIFI or not
uint8_t nSampling = 500;            //change as per your need
uint16_t threshold = 200;           // change as per your need

void setup()
{
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  pinMode(16, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  digitalWrite(16, HIGH);
  Serial.begin(115200); // change as per your need

  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to WiFi network");

  while (WiFi.status() != WL_CONNECTED)
  {

    Serial.print(".");
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
  }

  Serial.println("\nConnected to the WiFi network: " + WiFi.SSID() + " and strength is: " + WiFi.RSSI());
}

void loop()
{
  if ((WiFi.status() == WL_CONNECTED))
  {
    float total = 0;
    for (int i = 1; i <= nSampling; i++)
    {

      int x = analogRead(A0);

      total += x;
      delay(10);
    }
    total /= nSampling;
    Serial.println(total);
    if (total >= threshold)
    {
      Gsender *gsender = Gsender::Instance(); // Getting pointer to class instance
      String subject = "Gas Leakage";
      if (gsender->Subject(subject)->Send("recipient@gmail.com", "Gas Leakage found!"))
      { // change recipient with your recipient
        Serial.println("Message send.");
      }
      else
      {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
      }
      digitalWrite(16,LOW);
      delay(300000);
    }
  }
  else if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
  }
  digitalWrite(16,HIGH);
}
