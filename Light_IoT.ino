// BH1750FVI - Version: Latest
#include <BH1750FVI.h>
#include <WiFiNINA.h>
BH1750FVI BH1750(BH1750FVI::k_DevModeContLowRes);

//please enter your sensitive data in the Secret tab
char ssid[] = "********";
char pass[] = "**********";

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";
String queryString = "?value1=57&value2=25";

bool light_available = false;

void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);
  BH1750.begin();

  Serial.begin(9600);
  while (!Serial);

}

void sunlight_received() {
  String PATH   = "/trigger/sunlight_recieved/with/key/your-key"; // change your EVENT-NAME and YOUR-KEY

  // make a HTTP request:
  // send HTTP header
  client.println("GET " + PATH + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println(); // end HTTP header


  while (client.connected()) {
    if (client.available()) {
      // read an incoming byte from the server and print it to serial monitor:
      char c = client.read();
      Serial.print(c);
    }
  }
}

void sunlight_stopped() {
  StringPATH   = "/trigger/sunlight_stopped/with/key/your-key"; // change your EVENT-NAME and YOUR-KEY

  // make a HTTP request:
  // send HTTP header
  client.println("GET " + PATH + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println(); // end HTTP header


  while (client.connected()) {
    if (client.available()) {
      // read an incoming byte from the server and print it to serial monitor:
      char c = client.read();
      Serial.print(c);
    }
  }
}

void loop() {
  int intensity = BH1750.GetLightIntensity();
  int count = 0;

  Serial.println("Light: " + intensity);
  //  Serial.println(lux);

  if (intensity < 100 && light_available == true )
  {
    count++;
    if (count > 5000)
    {
      while (!Serial);

      // connect to web server on port 80:
      if (client.connect(HOST_NAME, 80)) {
        // if connected:
        Serial.println("Connected to server");
      }
      else {// if not connected:
        Serial.println("connection failed");
      }
      sunlight_stopped();
      client.stop();
      Serial.println();
      Serial.println("disconnected");
      light_available = false;
      count = 0;
    }
  }
  else if (intensity > 100 && light_available == false)
  {
    count++;
    if (count > 5000)
    {
      while (!Serial);

      // connect to web server on port 80:
      if (client.connect(HOST_NAME, 80)) {
        // if connected:
        Serial.println("Connected to server");
      }
      else {// if not connected:
        Serial.println("connection failed");
      }
      sunlight_received();
      client.stop();
      Serial.println();
      Serial.println("disconnected");
      light_available = true;
      count = 0;
    }
  }
}
