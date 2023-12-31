#include <U8glib.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 17

// OLED
#define CS 8
#define DC 9
#define RES 10
#define MOSI 11
#define CLK 12

// OLED Resolution
#define L 128
#define W 64

//Sensors
int MQ_3 = A1;
int MQ_2 = A2;
int tempPin = A3;

//Initialize Variables
float temperatureC = 0;
int alc = 0;
int smoke = 0; 

// LED
#define RED 7
#define GREEN 6

// Buzzer
int BUZZER = A0;

// Thresholds
#define thres_temp 80
#define thres_alc 1000
#define thres_smoke 400

// Objects
U8GLIB_SH1106_128X64 u8g(CLK, MOSI, CS, DC, RES);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const uint8_t bit_map [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x98, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x70, 0x00, 0x00,
0x00, 0x00, 0x00, 0x84, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x30, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0x11, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x94, 0x91, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x90,
0x9F, 0xE0, 0x00, 0x00, 0x00, 0x01, 0x90, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x82, 0x3F, 0xE0,
0x00, 0x00, 0x00, 0x00, 0x40, 0x67, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x09, 0x07, 0xE0, 0x00, 0x00,
0x00, 0x00, 0x04, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);

  // Initialize sensors
  sensors.begin();

  // Set pin modes
  pinMode(MQ_2, INPUT);
  pinMode(MQ_3, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  tone(BUZZER, 1000);
  delay(5000);
}

void readSensorValue() {

  u8g.setFont(u8g_font_unifont);

  char buf_one[8];
  u8g.drawStr(0, 21, "Temp: ");
  u8g.setPrintPos(70, 21); // Set the print position
  u8g.print((int)temperatureC);  // Print the temperature value as a float
  u8g.drawStr(90, 21, " C");

  char buf_two[8];
  u8g.drawStr(0, 42, "Ethanol: ");
  u8g.setPrintPos(70, 42);
  u8g.print(alc);
  u8g.drawStr(90, 42, " PPM");
  

  char buf_three[8];
  u8g.drawStr(0, 63, "Smoke: ");
  u8g.setPrintPos(70, 63);
  u8g.print(smoke);
  u8g.drawStr(90, 63, " PPM");
  
  if ((smoke > thres_smoke) || (alc > thres_alc) || (temperatureC > thres_temp)) {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    tone(BUZZER, 1000);
  } else {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
    noTone(BUZZER);
  }
}

void loop() {
  temperatureC = sensors.getTempCByIndex(0);  
  alc = analogRead(MQ_3);
  smoke = analogRead(MQ_2);

  sensors.requestTemperatures();

  u8g.firstPage();
  do {
    readSensorValue();

  } while (u8g.nextPage());

  delay(1000);
}
