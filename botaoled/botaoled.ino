
#include "AdafruitIO_WiFi.h"
#include "NewPing.h"

#define BUZZER_PIN 27
#define LED_ALARME 13
#define BOTAO_FISICO 26
#define TRIG_PIN 12
#define ECHO_PIN 14


//Configuraçãodo ultrassonico
#define MAX_DISTENCE 100
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTENCE);

// suas credenciais Wi-Fi
#define WIFI_SSID       ""
#define WIFI_PASS       ""

// suas credenciais da Adafruit IO
#define IO_USERNAME     ""
#define IO_KEY          ""

// Cria objeto de conexão
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Cria referência ao feed
AdafruitIO_Feed *botaoLED = io.feed("botaoled");

//Definição dos feeds 
AdafruitIO_Feed *botaoalarme = io.feed("botaoalarme");
AdafruitIO_Feed *distanciaultrassom = io.feed("distanciaultrassom");

// Define o pino do LED
#define LED_PIN 14

// Variaveis de controle
bool alarmeAtivo = false;
unsigned int distancia = 0;
const int LIMITE_DISTANCIA = 15;

void setup() {
  // Inicializa comunicação serial
  Serial.begin(115200);

  // Configura o pino do LED como saída
  pinMode(LED_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_ALARME, OUTPUT);
  pinMode(BOTAO_FISICO, INPUT);

  Serial.begin(115200);

  while (!Serial)
  ;

  delay(1000);

  // Conecta ao Adafruit IO
  Serial.print("Conectando ao Adafruit IO");
  io.connect();

  // Aguarda conexão
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("Adafruit Conectado!");

  //Vincula a função callback ao eed
  botaoalarme -> onMessage(handleAlarme);

  Serial.println("Solicitando o estado inicial do alarme: ");
  botaoalarme -> get();

  delay(1000);
}

void loop() {
//   // Mantém conexão ativa
//   io.run();
// }

// // Função chamada sempre que houver alteração no feed
// void handleBotaoLed(AdafruitIO_Data *data) {
//   String valor = data->toString();
//   Serial.print("Recebido do feed: ");
//   Serial.println(valor);

//   if (valor == "true") {
//     digitalWrite(LED_PIN, HIGH);
//     Serial.println("LED LIGADO");
//   } else {
//     digitalWrite(LED_PIN, LOW);
//     Serial.println("LED DESLIGADO");
//   }

  // Serial.print(F("Distancia Lida: "));
  // Serial.println(sonar.ping_cm());
  // delay(500);

  io.run();

  //Leitura do botão fisico
  if (digitalRead(BOTAO_FISICO) == 1){
   delay(200); //Debunce simples
   alarmeAtivo = !alarmeAtivo;

   botaoalarme -> save(String(alarmeAtivo ? "true" : "false"));
   Serial.println(alarmeAtivo ? "Alarme ARMADO pelo botao fisico." : "Alarme DESARMADO pelo botao fisico.");
  }

  distancia = sonar.ping_cm();
  Serial.print("Distancia lida: ");
  Serial.println(distancia);
  Serial.print(" cm");

  if(distancia !=0){
    //só envia distancias válidas
  distanciaultrassom -> save(distancia);
  }

  //Ativação e desativação do alarme
  if(alarmeAtivo && distancia > 0 && distancia < LIMITE_DISTANCIA){
    ativarAlarme();
  }
  else{
    desligarAlarme();
  }

  delay(3000); //Inervalo ideal para o adafruit 


}