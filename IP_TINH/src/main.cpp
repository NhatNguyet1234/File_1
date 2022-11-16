#include <Arduino.h>

#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include <wed.h>
#include <EEPROM.h>

ESP8266WebServer server(80);
const char* ten_wifi = "nguyet123";
const char* pass_wifi = "12345679";

IPAddress local_IP(192, 168, 43, 105);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
float kh;
#define den D5
unsigned int h1,h2;
String myStr, gop1,gop2;
String t1,t2,so1,so2,so3,so4;
void nhdo()
  {
   
    int t= random(9,80);
        String k = String(t);
        server.send(200,"text/plain",k);
  }

  void write_EEPROM(){
  String ssid = server.arg("ssid");
  Serial.println("Clear EEPROM!");
  for (int i = 0; i < 4; ++i) {
    EEPROM.write(i, 0);           
    delay(10);
  }
  for (int i = 0; i < 4; ++i) {
    EEPROM.write(i, ssid[i]);
  }
  EEPROM.commit();
  Serial.println("Writed to EEPROM!");
  Serial.print("SSID: ");
  Serial.println(ssid);
  
   ESP.reset();
}
void read_EEPROM()
{
  Serial.println("read data");
   String ssid = "";
    for (int i=0; i<4; ++i){
      ssid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(ssid);
    kh = ssid.toFloat();
    Serial.println(kh);
    }
      
void ketnoi(){
  String s = MAIN_page;// chua noi dung trang wed
  server.send(200,"text/html",s);// gui noi dung duoi dang html
  }
void setup(void)
{
   EEPROM.begin(512);
  Serial.begin(9600);
  Serial.println();
  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println(" dang ket noi:");
  WiFi.begin(ten_wifi,pass_wifi);
  Serial.println("Connecting");
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  // kiem tra no cos ket noi ko neu ko in dau.....
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("...");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    }
    Serial. println("Connect Success");//ket noi thanh cong
    Serial.print("ESP Address is: ");// dia chi cua esp la
    Serial.println(WiFi.localIP());      // in dia chi IP
    digitalWrite(LED_BUILTIN, LOW);
    read_EEPROM();
    server.on("/",ketnoi);
     
  server.on("/read",read_EEPROM);
  server.on("/writeEEPROM",write_EEPROM);
  server.on("/laynhietdo",nhdo);
  
    server.begin();
  
}

void loop() {
  server.handleClient();// xu li yeu cau tu clinet
}