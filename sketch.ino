#include<OneWire.h>
#include<DallasTemperature.h>
#include<TimerOne.h>

#define One_Wire 2
#define HEATER_PIN 3
#define BUZZER 4

#define TARGET_TEMP 60.0
#define STABILIZING_POINT 3.0
#define OVERHEAT_TEMP 75.0

OneWire oneWire(One_Wire);
DallasTemperature sensors(&oneWire);

typedef enum{
  IDLE,
  HEATING,
  STABILIZING,
  TARGET_REACHED,
  OVERHEAT
}State;

volatile State currentState = IDLE;
volatile float temp =0.0;

void logState(State s);
void controlTask();

void setup(){
  Serial.begin(9600);
  sensors.begin();

  pinMode(HEATER_PIN,OUTPUT);
  pinMode(BUZZER,OUTPUT);

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(controlTask);
}
void loop()
{

}

void controlTask()
{
  sensors.requestTemperatures();
  temp=sensors.getTempCByIndex(0);

  if(temp >= OVERHEAT_TEMP)
  {
    currentState =OVERHEAT;
  }
  else if(temp >= TARGET_TEMP)
  {
    currentState =TARGET_REACHED;
  }
  else if(temp>=TARGET_TEMP -STABILIZING_POINT)
  {
    currentState =STABILIZING;
  }
  else{
  currentState=HEATING;
}


switch(currentState)
{
  case IDLE:
  digitalWrite(HEATER_PIN,LOW);
  noTone(BUZZER);
  break;
  case HEATING:
  digitalWrite(HEATER_PIN,HIGH);
  tone(BUZZER,1000);
  break;
  case STABILIZING:
  digitalWrite(HEATER_PIN,HIGH);
  noTone(BUZZER);
  break;
  case TARGET_REACHED:
  digitalWrite(HEATER_PIN,LOW);
  noTone(BUZZER);
  break;
  case OVERHEAT:
  digitalWrite(HEATER_PIN,LOW);
  tone(BUZZER,2000);
  break;
}
Serial.print("Temperature of Heater:");
Serial.print(temp);
Serial.print("°C, State:");
logState(currentState);
}
void logState(State s)
{
  switch(s)
  {
    case IDLE:Serial.println("IDLE");break;
    case HEATING:Serial.println("HEATING");break;
    case STABILIZING:Serial.println("STABILIZING");break;
    case TARGET_REACHED:Serial.println("TARGER_REACHED");break;
    case OVERHEAT:Serial.println("OVERHEATED");break;
  }
}

