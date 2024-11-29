#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <SPIFFS.h>
#include <freertos/FreeRTOS.h>

Adafruit_MPU6050 mpu;
Adafruit_BMP280 bmp;

double altitude = 0;
double tempbmp = 0;
double pressao = 0;

double aceleracaoX = 0, aceleracaoY = 0, aceleracaoZ = 0;
double tempmpu = 0;
double gyroX = 0, gyroY = 0, gyroZ = 0;

unsigned long previousMillisBMP = 0;
unsigned long previousMillisMPU = 0;
unsigned long startTime = 0;
const unsigned long intervalBMP = 300;
const unsigned long intervalMPU = 100;
const unsigned long runTime = 20000;

SemaphoreHandle_t fileMutex;  // Mutex para sincronização de acesso aos arquivos
QueueHandle_t bmpDataQueue;   // Fila para dados do BMP280
QueueHandle_t mpuDataQueue;   // Fila para dados do MPU6050

// Contadores para leituras e salvamentos
volatile unsigned int bmpReadCount = 0;
volatile unsigned int bmpSaveCount = 0;
volatile unsigned int mpuReadCount = 0;
volatile unsigned int mpuSaveCount = 0;

// Estrutura de dados para armazenar as leituras
struct BMP280Data {
  double altitude;
  double pressao;
  double temperatura;
};

struct MPU6050Data {
  double aceleracaoX;
  double aceleracaoY;
  double aceleracaoZ;
  double gyroX;
  double gyroY;
  double gyroZ;
  double temperatura;
};

// Função para inicializar os arquivos e o sistema de arquivos
void setupFiles() {
  if (!SPIFFS.begin(true)) {
    Serial.println("Falha ao montar o sistema de arquivos!");
    while (1);
  }
  if (SPIFFS.exists("/bmp280.txt")) SPIFFS.remove("/bmp280.txt");
  if (SPIFFS.exists("/mpu6050.txt")) SPIFFS.remove("/mpu6050.txt");
}

// Função para ler dados do BMP280
void readBMP280(void *parameter) {
  while (true) {
    pressao = bmp.readPressure();
    altitude = bmp.readAltitude(1016);
    tempbmp = bmp.readTemperature();
  
    BMP280Data data = {altitude, pressao, tempbmp};
    xQueueSend(bmpDataQueue, &data, portMAX_DELAY);  // Envia os dados para a fila
    bmpReadCount++;  // Incrementa o contador de leituras

    vTaskDelay(300 / portTICK_PERIOD_MS);  
  }
}

// Função para ler dados do MPU6050
void readMPU6050(void *parameter) {
  while (true) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    MPU6050Data data = {a.acceleration.x, a.acceleration.y, a.acceleration.z, 
                        g.gyro.x, g.gyro.y, g.gyro.z, temp.temperature};
    xQueueSend(mpuDataQueue, &data, portMAX_DELAY);  // Envia os dados para a fila
    mpuReadCount++;  // Incrementa o contador de leituras
    vTaskDelay(100 / portTICK_PERIOD_MS);  
  }
}

// Função para salvar os dados do BMP280
void saveBMP280Data(void *parameter) {
  while (true) {
    if (xSemaphoreTake(fileMutex, portMAX_DELAY) == pdTRUE) {
      BMP280Data data;
      if (xQueueReceive(bmpDataQueue, &data, portMAX_DELAY)) {
        File file = SPIFFS.open("/bmp280.txt", FILE_APPEND);
        if (file) {
          file.printf("Altitude: %.2f m, Pressao: %.2f Pa, Temperatura: %.2f Celsius\n", 
                      data.altitude, data.pressao, data.temperatura);
          file.close();
          bmpSaveCount++;  // Incrementa o contador de salvamentos
        }
      }
      xSemaphoreGive(fileMutex);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);  // Delay pequeno para permitir alternância
  }
}

// Função para salvar os dados do MPU6050
void saveMPU6050Data(void *parameter) {
  while (true) {
    if (xSemaphoreTake(fileMutex, portMAX_DELAY) == pdTRUE) {
      MPU6050Data data;
      if (xQueueReceive(mpuDataQueue, &data, portMAX_DELAY)) {
        File file = SPIFFS.open("/mpu6050.txt", FILE_APPEND);
        if (file) {
          file.printf("Accel: [%.2f, %.2f, %.2f] m/s², Gyro: [%.2f, %.2f, %.2f] rad/s, Temp: %.2f Celsius\n",
                      data.aceleracaoX, data.aceleracaoY, data.aceleracaoZ, 
                      data.gyroX, data.gyroY, data.gyroZ, data.temperatura);
          file.close();
          mpuSaveCount++;  // Incrementa o contador de salvamentos
        }
      }
      xSemaphoreGive(fileMutex);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);  // Delay pequeno para permitir alternância
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
  setupFiles();

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

  // Criação do mutex e das filas
  fileMutex = xSemaphoreCreateMutex();
  bmpDataQueue = xQueueCreate(10, sizeof(BMP280Data));  // Fila de dados do BMP280
  mpuDataQueue = xQueueCreate(10, sizeof(MPU6050Data));  // Fila de dados do MPU6050

  // Criação das tasks
  xTaskCreate(readBMP280, "Read BMP280", 4096, NULL, 1, NULL);
  xTaskCreate(readMPU6050, "Read MPU6050", 4096, NULL, 1, NULL);
  xTaskCreate(saveBMP280Data, "Save BMP280 Data", 4096, NULL, 2, NULL);
  xTaskCreate(saveMPU6050Data, "Save MPU6050 Data", 4096, NULL, 2, NULL);

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

  // Aguarda as tasks terminarem antes de reiniciar o loop
  delay(10);
}
