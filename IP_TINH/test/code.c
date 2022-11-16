#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include <EEPROM.h>
#include "TM1637Display.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "wed1.h"
ESP8266WebServer server(80);
char* ssid_ap = "";
char* pass_ap = "";
IPAddress local_IP(192, 168, 43, 145);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

float kh;
#define ONE_WIRE_BUS D7 // sử dụng chân D7
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define den D5
unsigned int h1,h2;
String myStr, gop1,gop2;
String t1,t2,so1,so2,so3,so4;

TM1637Display display(D1,D2);// CLK VÀ DIO

  void nhdo()
  {
   sensors.requestTemperatures(); 
   float t=sensors.getTempCByIndex(0);
    myStr = String(t); // Chuyển đổi số so thành string
  
      so1=myStr.charAt(0);
      so2=myStr.charAt(1);
      so3=myStr.charAt(3);
      so4=myStr.charAt(4);
      gop1 = so1 + so2 + so3 + so4;
      int nhiet1= gop1.toInt (); // Chuyển string thành int
      display.showNumberDecEx (nhiet1, 0b01000000);
      
      if(t>kh){
        digitalWrite(den, LOW);
        } else {
          digitalWrite(den, HIGH);
          }
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
  server.send(200,"text/plain","Đang cài đặt");
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
void setup() {
  
  EEPROM.begin(512);  
  Serial.begin(9600);

  pinMode(den, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println(" dang ket noi:");
  WiFi.begin(ssid_ap,pass_ap);
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
  digitalWrite(den, LOW);
  read_EEPROM();
  sensors.begin();
  display.setBrightness(5);// chỉnh độ sáng  0->7
  display.clear();
  delay(1000);

  server.on("/",ketnoi);
  server.on("/read",read_EEPROM);
  server.on("/writeEEPROM",write_EEPROM);
  server.on("/laynhietdo",nhdo);
  server.begin();
 }
 
void loop(){
  if(millis()-h1>=100)
  {
    server.handleClient();// xu li yeu cau tu clinet
    h1=millis();
  }
  
  
}
  