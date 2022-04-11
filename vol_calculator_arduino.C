// LUÍS FELIPE DO CARMO COSTA TAVARES
// UFRPE
// Internet das Coisas - IOT

#include <stdio.h> 
#include <LiquidCrystal.h>
LiquidCrystal display_lcd = LiquidCrystal(7, 6, 5, 4, 3, 2);

const int sensorA_PIN = 10;
const int sensorB_PIN = 11;
const int sensorC_PIN = 12;
const int buzz_PIN = 13;

float sensorA_dist;
float sensorB_dist;
float sensorC_dist;

float actual_vol;
int response;
//-- Sumário da variável response --
// 0 - Nenhuma constatação
// 1 - Nível Crítico
// 2 - Nível Extravasão
// 3 - Sensor 1 falhando
// 4 - Sensor 2 falhando
// 5 - Sensor 3 falhando
//----------------------------------

//Parâmetros Configuráveis
const uint16_t cylinder_height = 50;
const uint16_t cylinder_radius = 2;
const uint16_t critical_height = 10;
const uint16_t extravasion_height = 40;
const int err_margin = cylinder_height * 0.10;
const float pi = 3.1415;

//Ler a distância 
float readUDist(int sensor_PIN) {
  pinMode(sensor_PIN, OUTPUT);  
  digitalWrite(sensor_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(sensor_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(sensor_PIN, LOW);
  pinMode(sensor_PIN, INPUT);
  float partial_result = 0.01723 * pulseIn(sensor_PIN, HIGH);
  float result = cylinder_height * partial_result/335;
  return result;
}

//controlar o som do buzzer
void buzzControl(int alert_freq, int wait_1, int wait_2) {
  tone(buzz_PIN,alert_freq); 
  delay(wait_1);
  noTone(buzz_PIN);
  delay(wait_2);
}

//interpreta as distâncias(Main sensor - sensorB)
int getResponse(int sensorA, int sensorB, int sensorC) {
  int sensor_return = sensorProbl(sensorA, sensorB, sensorC);
  if (sensor_return == 0) {
    float sensor_ave = (sensorA + sensorB + sensorC)/3;
    if (sensor_ave > extravasion_height){return 2;}
    else if (sensor_ave < critical_height) {return 1;}
    else {return 0;}
  }
  else {return sensor_return + 2;}
}

//Checar se os sensores estão ativos
int sensorProbl(int sA, int sB, int sC) {
  int compare_ab = abs(sB - sA);
  int compare_bc = abs(sB - sC);
  if (compare_ab >= err_margin && compare_bc < err_margin) { return 1;} 
  else if (compare_ab >= err_margin && compare_bc >= err_margin) {return 2;} 
  else if (compare_bc >= err_margin && compare_ab < err_margin){return 3;} 
  else {return 0;}
}

//Gerar display do lcd
void generateDisplay(String top_alert, float actual_vol ) {
  char linha1[17];
  char floatString[10];
  display_lcd.setCursor(0,0);
  display_lcd.print(top_alert);
  display_lcd.setCursor(0,1);
  dtostrf(actual_vol, 4, 2, floatString);
  sprintf(linha1, "Volume: %s", floatString);
  display_lcd.print(linha1);
  Serial.println(actual_vol);
}
  
//Interpretar a resposta do código
void interpResponse(int response, float act_vol) {
  Serial.println(response);
  if(response == 0){
	generateDisplay("--Status: OK!!--", act_vol);
  } else if(response == 1){
    generateDisplay(" Nivel Critico! ", act_vol);
    buzzControl(870,250,250);
  } else if(response == 2){
    generateDisplay("Nivel Extravasao", act_vol);
    buzzControl(550,450,250);
  } else if(response == 3){
    generateDisplay("Sensor 1 Falhou!", act_vol);
    buzzControl(1110,1000,1);
  } else if(response == 4){
    generateDisplay("Sensor 2 Falhou!", act_vol);
    buzzControl(1110,1000,1);
  } else if(response == 5){
    generateDisplay("Sensor 3 Falhou!", act_vol);
    buzzControl(1110,1000,1);
  }
}


//retorna o volume das esferas
float getVolavg(int sensorA, int sensorB, int sensorC){
  float sens_ave = (sensorA + sensorB + sensorC)/3; 
  float base_area = pow(cylinder_radius, 2) * pi;
  return base_area * sens_ave;
}

void setup()
{
  display_lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(buzz_PIN,OUTPUT);
  pinMode(sensorA_PIN,INPUT);
  pinMode(sensorB_PIN,INPUT);
  pinMode(sensorC_PIN,INPUT);
}

void loop()
{
  sensorA_dist = readUDist(sensorA_PIN);
  sensorB_dist = readUDist(sensorB_PIN);
  sensorC_dist = readUDist(sensorC_PIN);
  response = getResponse(sensorA_dist, sensorB_dist, sensorC_dist);
  actual_vol = getVolavg(sensorA_dist, sensorB_dist, sensorC_dist);
  interpResponse(response, actual_vol);
  delay(400);
  
}
