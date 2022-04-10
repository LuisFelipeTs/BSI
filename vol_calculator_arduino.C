
const int sensorA_PIN = 10;
const int sensorB_PIN = 11;
const int sensorC_PIN = 12;
const int buzz_PIN = 13;

float sensorA_dist;
float sensorB_dist;
float sensorC_dist;

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
int cylinder_height = 1000;
int cylinder_radius = 50;
int critical_height = 120;
int extravasion_height = 850;
int err_margin = cylinder_height * 0.10;

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
int sensorProbl(int sA, int sB, int sC){
  int compareAB = abs(sB - sA);
  int compareBC = abs(sB - sC);
  
  if (compareAB >= err_margin && compareBC < err_margin) { return 1;} 
  else if (compareAB >= err_margin && compareBC >= err_margin) {return 2;} 
  else if (compareBC >= err_margin && compareAB < err_margin){return 3;} 
  else {return 0;}
}
  
//Interpretar a resposta do código
void interpResponse() {
  if(response == 0){
  } else if(response == 1){
    buzzControl(870,250,250);
  } else if(response == 2){
    buzzControl(550,450,250);
  } else if(response == 3){
    buzzControl(1110,1000,1);
  } else if(response == 4){
    buzzControl(1110,1000,1);
  } else if(response == 5){
    buzzControl(1110,1000,1);
  }
}

void setup()
{
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
  //response = getResponse(sensorA_dist, sensorB_dist, sensorC_dist);
  interpResponse();
  delay(800);
  Serial.println( );
}
