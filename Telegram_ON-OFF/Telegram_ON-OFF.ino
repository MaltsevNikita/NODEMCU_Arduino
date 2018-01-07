/*******************************************************************
 *  An example of how to use a SendPhoto.       *
 *                                                                 *
 *                                                                 *
 *  written by Vadim Sinitski                                      *
 *******************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Initialize Wifi connection to the router
char ssid[] = "ZyXEL_KEENETIC_4AB1EC";     // your network SSID (name)
char password[] = "homesweethome"; // your network key

//char ssid[] = "Gradient WiFi";     // your network SSID (name)
//char password[] = "gradient"; // your network key


//======================for LED variables=======
const int ledPin = 13;
int ledStatus = 0;
//==============================================


// Initialize Telegram BOT
#define BOTtoken "263561827:AAG9j1Fd_n86tHWPNbJzMZnNE2XyCuGeOdk"  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

String test_photo_url = "https://www.arduino.cc/en/uploads/Trademark/ArduinoCommunityLogo.png";

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/get_test_photo") {
      bot.sendPhoto(chat_id, test_photo_url, "Caption is optional, you may not use photo caption");
    }

    if (text == "/start") {
      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      welcome += "This is Send Image From URL example.\n\n";
      welcome += "/get_test_photo : getting test photo\n";

      bot.sendMessage(chat_id, welcome, "");
    }

     if (text == "/ledon") {
      digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
      ledStatus = 1;
      bot.sendMessage(chat_id, "Led is ON", "");
    }

    if (text == "/ledoff") {
      ledStatus = 0;
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Led is OFF", "");
    }

    
  }
}

void setup() {
  Serial.begin(115200);
//================================SET led pin mode================================
  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(ledPin, HIGH); // initialize pin as off
//================================================================================
  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
