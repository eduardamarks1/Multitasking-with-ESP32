#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <SPIFFS.h>


// Sensores
Adafruit_MPU6050 mpu;
Adafruit_BMP280 bmp;


// Variáveis para o BMP280
double altitude = 0;
double tempbmp = 0;
double pressao = 0;


// Variáveis para o MPU6050
double aceleracaoX = 0, aceleracaoY = 0, aceleracaoZ = 0;
double tempmpu = 0;
double gyroX = 0, gyroY = 0, gyroZ = 0;


// Contadores de leituras e salvamentos
unsigned int bmpReadCount = 0;
unsigned int bmpSaveCount = 0;
unsigned int mpuReadCount = 0;
unsigned int mpuSaveCount = 0;


// Controle de tempo
unsigned long previousMillisBMP = 0;
unsigned long previousMillisMPU = 0;
unsigned long startTime = 0;
const unsigned long intervalBMP = 2000;  
const unsigned long intervalMPU = 1000;  
const unsigned long runTime = 30000;  




// Função para apagar o conteúdo dos arquivos na flash
void clearFlashFiles() {
  SPIFFS.remove("/bmp280.txt");
  SPIFFS.remove("/mpu6050.txt");
}


// Função para ler dados do BMP280
void readBMP280() {
  pressao = bmp.readPressure();
  altitude = bmp.readAltitude(1016);
  tempbmp = bmp.readTemperature();
  bmpReadCount++;  // Incrementa o contador de leituras
}


// Função para salvar dados do BMP280
void saveBMP280Data() {
  File file = SPIFFS.open("/bmp280.txt", FILE_APPEND);
  if (file) {
    file.printf("Altitude: %.2f m, Pressao: %.2f Pa, Temperatura: %.2f Celsius\n", altitude, pressao, tempbmp);
    file.close();
    bmpSaveCount++;  // Incrementa o contador de salvamentos
  }
}


// Função para ler dados do MPU6050
void readMPU6050() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);


  aceleracaoX = a.acceleration.x;
  aceleracaoY = a.acceleration.y;
  aceleracaoZ = a.acceleration.z;
  tempmpu = temp.temperature;
  gyroX = g.gyro.x;
  gyroY = g.gyro.y;
  gyroZ = g.gyro.z;
  mpuReadCount++;  // Incrementa o contador de leituras
}


// Função para salvar dados do MPU6050
void saveMPU6050Data() {
  File file = SPIFFS.open("/mpu6050.txt", FILE_APPEND);
  if (file) {
    file.printf("Accel: [%.2f, %.2f, %.2f] m/s², Gyro: [%.2f, %.2f, %.2f] rad/s, Temp: %.2f Celsius\n",
                aceleracaoX, aceleracaoY, aceleracaoZ, gyroX, gyroY, gyroZ, tempmpu);
    file.close();
    mpuSaveCount++;  // Incrementa o contador de salvamentos
  }
}


// Função para ler os dados salvos nos arquivos e imprimir no monitor serial
void printSavedData() {
  // Ler dados do BMP280
  File bmpFile = SPIFFS.open("/bmp280.txt", FILE_READ);
  if (bmpFile) {
    Serial.println("\nDados do BMP280:");
    while (bmpFile.available()) {
      Serial.write(bmpFile.read());
    }
    bmpFile.close();
  } else {
    Serial.println("Erro ao abrir bmp280.txt para leitura.");
  }


  // Ler dados do MPU6050
  File mpuFile = SPIFFS.open("/mpu6050.txt", FILE_READ);
  if (mpuFile) {
    Serial.println("\nDados do MPU6050:");
    while (mpuFile.available()) {
      Serial.write(mpuFile.read());
    }
    mpuFile.close();
  } else {
    Serial.println("Erro ao abrir mpu6050.txt para leitura.");
  }
}


void printSummary() {
  Serial.println("\nResumo da execução:");
  Serial.printf("Leituras do BMP280: %u\n", bmpReadCount);
  Serial.printf("Salvamentos do BMP280: %u\n", bmpSaveCount);
  Serial.printf("Leituras do MPU6050: %u\n", mpuReadCount);
  Serial.printf("Salvamentos do MPU6050: %u\n", mpuSaveCount);
}


void setup() {
  Serial.begin(115200);
  Wire.begin();


  // Inicialização do SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Falha ao montar o sistema de arquivos!");
    while (1);
  }


  // Apaga os arquivos anteriores na flash
  clearFlashFiles();


  // Inicialização do MPU6050
  if (!mpu.begin(0x68)) {
    Serial.println("Falha na inicialização do MPU6050!");
    while (1);
  }


  // Inicialização do BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("Falha na inicialização do BMP280!");
    while (1);
  }


  startTime = millis();
}


void loop() {
  unsigned long currentMillis = millis();


  // Verifica o tempo total de execução
  if (currentMillis - startTime > runTime) {
    Serial.println("Execucao concluida. Dados salvos:");
    printSavedData();  // Imprime os dados dos arquivos
    printSummary();    // Exibe o resumo da execução
    while (1);         // Finaliza a execução
  }


  // Leitura e salvamento do BMP280
  if (currentMillis - previousMillisBMP >= intervalBMP) {
    previousMillisBMP = currentMillis;
    readBMP280();
    saveBMP280Data();
  }


  // Leitura e salvamento do MPU6050
  if (currentMillis - previousMillisMPU >= intervalMPU) {
    previousMillisMPU = currentMillis;
    readMPU6050();
    saveMPU6050Data();
  }
}
