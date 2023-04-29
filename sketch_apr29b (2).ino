//библиотеки MQ датчиков
#include <BaseMQ.h> 
#include <MQ135.h> 
#include <MQ2.h> 
#include <MQ3.h> 
#include <MQ4.h> 
#include <MQ5.h> 
#include <MQ6.h> 
#include <MQ7.h> 
#include <MQ8.h> 
#include <MQ9.h> 
#include <TroykaMQ.h> 
 
// переменные для определения водорода 
#define MQ_PIN (0) 
#define RL_VALUE (10) 
#define RO_CLEAN_AIR_FACTOR (9.21) 
#define CALIBARAION_SAMPLE_TIMES (50) 
#define CALIBRATION_SAMPLE_INTERVAL (500) 
#define READ_SAMPLE_INTERVAL (50) 
#define READ_SAMPLE_TIMES (5) 
#define GAS_H2 (0) 
float H2Curve[3] = {2.3, 0.93,-1.44}; 
float Ro = 10; 
//Переменные для определения метана 
#define PIN_MQ2 A1 
#define PIN_MQ2_HEATER 13 
MQ2 mq2(PIN_MQ2, PIN_MQ2_HEATER);
void setup() 
{ 
 //вывод данных о калибровке датчиков 
 Serial.begin(9600); 
 Serial.print("Calibrating...\n"); 
 mq2.heaterPwrHigh(); 
 Serial.println(" CH4 Heated sensor"); 
 Ro = MQCalibration(MQ_PIN); 
 Serial.print("Calibration is done...\n"); 
 Serial.print("Ro="); 
 Serial.print(Ro); 
 Serial.print("kohm"); 
 Serial.print("\n"); 
// диджитал-порты для светодиодов
 pinMode(5, OUTPUT); 
 pinMode(6, OUTPUT); 
 pinMode(7, OUTPUT); 
 pinMode(8, OUTPUT); 
 pinMode(9, OUTPUT); 
 pinMode(4, OUTPUT); 
 pinMode(3, OUTPUT); 
 pinMode(10, OUTPUT); 
 digitalWrite(5, 0); 
 digitalWrite(6, 0); 
 digitalWrite(7, 1); 
 digitalWrite(8, 0); 
 digitalWrite(9, 0); 
 digitalWrite(4, 1); 
 digitalWrite(10, 0); 
 digitalWrite(3, 0); 
//диджитал-порт для пьезоэлемента 
 tone(11, 1000, 1000); 
} 
void loop() 
{ 
 // вывод данных о концентрациях газов
 Serial.print("H2:"); 
 Serial.print(MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_H2) ); 
 Serial.print( "ppm" ); 
 Serial.print("\n"); 
 delay(200); 
 //для метана 
 if (!mq2.isCalibrated() && mq2.heatingCompleted()) { 
 mq2.calibrate(); 
 Serial.print("Ro = "); 
 Serial.println(mq2.getRo()); 
 } 
 if (mq2.isCalibrated() && mq2.heatingCompleted()) { 
 Serial.print("Ratio: "); 
 Serial.print(mq2.readRatio()); 
 Serial.print(" Methane: "); 
 Serial.print(mq2.readMethane()); 
 Serial.print(" ppm "); 
 delay(200); 
 } 
// условные функции для вывода цветов на светодиодах 
if(mq2.readMethane() > 30 && mq2.readMethane() < 60) 
{ 
digitalWrite(5, LOW); 
digitalWrite(6, LOW); 
digitalWrite(3, HIGH); 
} 
else if(mq2.readMethane() > 60 && mq2.readMethane() < 90) 
{ 
digitalWrite(5, HIGH); 
digitalWrite(6, LOW); 
digitalWrite(3, HIGH); 
} 
else if(mq2.readMethane() > 90) 
{ 
digitalWrite(5, HIGH); 
digitalWrite(6, HIGH); 
digitalWrite(3, HIGH); 
tone(11, 1500, 1000); 
delay(500); 
} 
else 
{ 
digitalWrite(5, LOW); 
digitalWrite(6, LOW); 
digitalWrite(3, LOW); 
}
if(MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_H2) > 50 && 
MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_H2) < 90 ) 
{ 
digitalWrite(8, LOW); 
digitalWrite(9, LOW); 
digitalWrite(10, HIGH); 
} 
else if(MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_H2) > 90 && 
MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_H2) < 130) 
{ 
digitalWrite(8, HIGH); 
digitalWrite(9, LOW); 
digitalWrite(10, HIGH); 
} 
else if(MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_H2) > 130) 
{ 
digitalWrite(8, HIGH); 
digitalWrite(9, HIGH); 
digitalWrite(10, HIGH); 
tone(11, 1500, 1000); 
delay(500); 
} 
else 
{ 
digitalWrite(8, LOW); 
digitalWrite(9, LOW); 
digitalWrite(10, LOW); 
} 
} 
// функция для определения концентрации водорода
float MQResistanceCalculation(int raw_adc) 
{
 return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc)); 
} 
float MQCalibration(int mq_pin) 
{ 
 int i; 
 float val=0; 
 for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) { 
 val += MQResistanceCalculation(analogRead(mq_pin)); 
 delay(CALIBRATION_SAMPLE_INTERVAL); 
 }
