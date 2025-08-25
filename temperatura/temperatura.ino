#include <math.h>
#include "AdafruitIO_WiFi.h"

//configurações da rede WiFi


//Autenticação Adafruit IO

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

#define pinNTC 34
#define pinLed 14 

//Controle de envio de dados
float temp_atual = 0;
float temp_anterior = -1;

// variável / ponteiro para referenciar o feed temperatura do adafruit
AdafruitIO_Feed *temperatura = io.feed("Temperatura");

const float Rfixo = 10000.0;  //resistor do projeto
const float Beta = 3950.0;
const float R0 = 10000.0;        //nominal do ntc
const float T0_kelvin = 298.15;  // 25°C em Kelvin
const float Vcc = 3.3;

void setup() {
  pinMode(pinNTC, INPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(pinNTC, INPUT);

  Serial.begin(115200);

  while (!Serial)
    ;

  Serial.print("Conectando ao Adafruit IO");
  io.connect();

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

//Mostra status da conexão 
Serial.println();
Serial.println(io.statusText());

//configuração do callback, quando o feed receber(atualizar) um valor 
temperatura -> onMessage(handleTemperatura);
//registra a funçaõ de callback 
//ele sera chamada sempre q o feed receber um novo dado 
}

void loop() {

  io.run(); //Manter a conexão com o Adafruit IO ativa

  //publicacao(); //chamada da função publish
  
  delay(3000);
}
