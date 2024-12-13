#include <WiFi.h>
#include <HTTPClient.h>

#define led_azul 9 // Pino utilizado para controle do led azul
#define led_verde 2 // Pino utilizado para controle do led verde
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 39 // Pino utilizado para controle do led amarelo

const int pinoBotao = 18;  // Pino utilizado para leitura do botão
int statusBotao;  // variavel para a leitura do status do botão

const int pinoLdr = 4;  // Pino utilizado para leitura do sensor LDR
int statusLdr; // variavel para leitura dos valores do sensor LDR
int threshold=600;

bool statusVermelho; // variavel para controle de status do led vermelho

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_azul,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo,OUTPUT);

  // Inicialização das entradas
  pinMode(pinoBotao, INPUT); // Define o pino de leitura do botão como INPUT (entrada)
  pinMode(pinoLdr, INPUT); // Define o pino de leitura do sensor LDR como INPUT (entrada)

  // Inicia o código com os leds desligados
  digitalWrite(led_azul, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  statusBotao = digitalRead(pinoBotao);
  if (statusBotao == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("Resposta da requisição HTTP: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Código de erro: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Desconectado");
  }
}

// Função que pisca o led amarelo
void piscaAmarelo(){
    digitalWrite(led_amarelo, HIGH);
    delay(1000);
    digitalWrite(led_amarelo, LOW);
    delay(1000);
}

// Função principal do semafor
void semaforo(){
  statusVermelho = false;
  // Liga o led verde
  digitalWrite(led_verde, HIGH);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, LOW);
  delay(3000);
  statusVermelho = false;
  // Liga o led amarelo
  digitalWrite(led_verde, LOW);
  digitalWrite(led_amarelo, HIGH);
  digitalWrite(led_vermelho, LOW);
  delay(2000);
  statusVermelho = true;
  while((digitalRead(pinoBotao) != 1) && (statusVermelho == true)){
    Serial.println("Vermelho ligado");
    // Liga o led vermelho
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_vermelho, HIGH);
    delay(5000);
    statusVermelho = false;
    break;
  }
  if(digitalRead(pinoBotao) == 1){
    Serial.println("1 SEGUNDO");
      delay(1000);
      digitalWrite(led_verde, HIGH);
      digitalWrite(led_amarelo, LOW);
      digitalWrite(led_vermelho, HIGH);  
  }
}



void loop() {
  int statusLdr=analogRead(pinoLdr);
  int statusBotao = digitalRead(pinoBotao);

    Serial.println(digitalRead(pinoBotao));

  if(statusLdr<=threshold){
    Serial.println("Esta escuro, ligue o led");
    Serial.println(statusLdr);
    piscaAmarelo();

  }else{
    Serial.println("Esta claro, apague a luz");
    Serial.println(statusLdr);
    semaforo();
  }
}