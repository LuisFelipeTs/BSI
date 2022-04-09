const int sensorA_PIN = 10;
const int sensorB_PIN = 11;
const int sensorC_PIN = 12;
const int buzz_PIN = 13;

int sensorA_dist;
int sensorB_dist;
int sensorC_dist;

int response;
//-- Sumário da variável response --
// 0 - Nenhuma constatação
// 1 - Nível Crítico
// 2 - Nível Extravasão
// 3 - Sensor 1 falhando
// 4 - Sensor 2 falhando
// 5 - Sensor 3 falhando
//----------------------------------

//Ler a distância 
long readUDist(int sensor_PIN)
{
  pinMode(sensor_PIN, OUTPUT);  
  digitalWrite(sensor_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(sensor_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(sensor_PIN, LOW);
  pinMode(sensor_PIN, INPUT);
  return 0.01723 * pulseIn(sensor_PIN, HIGH);
}

//controlar o som do buzzer
void buzz_control(int alert_freq, int wait_1, int wait_2){
  tone(buzz_PIN,alert_freq); 
  delay(wait_1);
  noTone(buzz_PIN);
  delay(wait_2);
}

//Interpretar a resposta do código
void interp_response(){
  if(response ==0){
  } else if(response == 1){
    buzz_control(970,250,250);
  } else if(response == 2){
    buzz_control(550,450,250);
  } else if(response == 3){
    buzz_control(1110,1000,1);
  } else if(response == 4){
    buzz_control(1110,1000,1);
  } else if(response == 5){
    buzz_control(1110,1000,1);
  }
}

void setup()
{
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
  
}