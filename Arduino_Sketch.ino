
#include <Wire.h>                      
#include <LiquidCrystal_I2C.h>           
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_MLX90614.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define SS_PIN D10
#define RST_PIN D9
//wifi
const char* ssid = "Name Of Your Wifi Network!";    
const char* password = "The password!";
const char* host = "script.google.com";
const int httpsPort = 443;
const char* googleRedirHost = "script.googleusercontent.com";
String GAS_ID = "The google sheets script id!";
//student data!
double tempreader;
String names;
int id;
String emails;

WiFiClientSecure client;


const char* fingerprint = "F6 C0 C1 1A 94 6E 56 36 99 CF B0 CA 6A 6A 2C EF 9B 93 3B FB";


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
LiquidCrystal_I2C lcd(0x27,20,4); 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 
void setup() 
{
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();// Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  lcd.init();                    
  lcd.backlight();   
  mlx.begin();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  


}
void Mainscreen(){
  lcd.setCursor(1,1);           
  lcd.print("searching for card");
  lcd.setCursor(8,2);           
  lcd.print(".");
  delay(500);
  lcd.setCursor(9,2);           
  lcd.print(".");
  delay(500);
  lcd.setCursor(10,2);           
  lcd.print(".");
  delay(500);
  lcd.setCursor(8,2);           
  lcd.print(" ");
  lcd.setCursor(9,2);           
  lcd.print(" ");
  lcd.setCursor(10,2);           
  lcd.print(" ");
}
void DataComplete(){
  lcd.setCursor(4,1);           
  lcd.print("Data uploaded");
  lcd.setCursor(4,2); 
  lcd.print("successfully!!");
}
void loop() 
{
  Mainscreen();
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "89 1A AD C1") //change here the UID of the card/cards that you want to give access
  {
    names = "Student Name";
    emails = "Student Email";
    id = Student id number;
    Serial.println("Authorized access");
    lcd.clear();
    lcd.setCursor(3,0);           
    lcd.print("Welcome ");
    lcd.print(names);
    lcd.setCursor(4,1);           
    lcd.print("iD: ");
    lcd.print(id);
    lcd.setCursor(1,2);     
    lcd.print("Waiting For temp..");
    delay(10000);
    tempreader = mlx.readAmbientTempC();
    lcd.setCursor(4,3);     
    lcd.print("temp: ");
    lcd.print(tempreader);
    sendData(names,id,tempreader,emails);
  }
  if (content.substring(1) == "F7 44 66 B4") //change here the UID of the card/cards that you want to give access
  {
    names = "Student Name";
    emails = "Student Email";
    id = Student id number;
    Serial.println("Authorized access");
    lcd.clear();
    lcd.setCursor(3,0);           
    lcd.print("Welcome ");
    lcd.print(names);
    lcd.setCursor(4,1);           
    lcd.print("iD: ");
    lcd.print(id);
    lcd.setCursor(1,2);     
    lcd.print("Waiting For temp..");
    delay(10000);
    tempreader = mlx.readAmbientTempC();
    lcd.setCursor(4,3);     
    lcd.print("temp: ");
    lcd.print(tempreader);
    sendData(names,id,tempreader,emails);
  }
 if (content.substring(1) != "F7 44 66 B4" && content.substring(1) != "89 1A AD C1" ){
    Serial.println(" Access denied");
    delay(500);
    lcd.clear();
    lcd.setCursor(2,2);           
    lcd.print("Access denied");
  }
 delay(10000);
 lcd.clear();
} 
void sendData(String names, int id,double temp,String emails)
{
  client.setInsecure();
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  if (client.verify(fingerprint, host)) {
  Serial.println("certificate matches");
  } else {
  Serial.println("certificate doesn't match");
  }
  String string_names =  String(names); 
  String string_id =  String(id); 
  String string_temperature =  String(temp); 
  String string_emails =  String(emails);
  String url = "/macros/s/" + GAS_ID + "/exec?Names=" + string_names + "&ID=" + string_id + "&Temp=" + string_temperature + "*C" + "&Emails=" + string_emails;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
  String line = client.readStringUntil('\n');
  if (line == "\r") {
    Serial.println("headers received");
    break;
  }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
  Serial.println("esp8266/Arduino CI successfull!");
  } else {
  Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  lcd.clear();
  DataComplete();
} 
