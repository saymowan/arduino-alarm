#include <LiquidCrystal.h> // Biblioteca LCD

LiquidCrystal lcd(12, 13, 5, 4, 3, 2); // Pinos do LCD
int trigPin = 8;  // Pino do Trigger do sensor ultrassônico
int echoPin = 7;  // Pino do Echo do sensor ultrassônico
int vermelhoLedPin = 9;
int verdeLedPin = 10;
int azulLedPin = 11;
int buzzerPin = 6; // Pino do buzzer
int pirPin = 4;    // Pino do sensor PIR
int botaoAtivarAlarmePin = 2;    // Pino do botão de ativar alarme
int botaoDesativarAlarmePin = 3; // Pino do botão de desativar alarme

bool alarmeAtivado = false;  // Estado do alarme
bool movimentoDetectado = false;  // Estado do sensor PIR

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial
  lcd.begin(16, 2);
  lcd.print("ALARME CASA MANEZINHA");

  // Configurações dos pinos
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);   
  pinMode(vermelhoLedPin, OUTPUT); 
  pinMode(verdeLedPin, OUTPUT); 
  pinMode(azulLedPin, OUTPUT); 
  pinMode(pirPin, INPUT);   
  pinMode(buzzerPin, OUTPUT); 
  pinMode(botaoAtivarAlarmePin, INPUT); 
  pinMode(botaoDesativarAlarmePin, INPUT); 
}

void loop() {
  // Verifica o estado dos botões de ativação e desativação do alarme
  if (digitalRead(botaoAtivarAlarmePin) == HIGH) {
    alarmeAtivado = true;
    escreveLCD("Alarme Ativado", 0);
  } 
  else if (digitalRead(botaoDesativarAlarmePin) == HIGH) {
    alarmeAtivado = false;
    resetaAlarme();
  }

  // Se o alarme está ativado, verifica o sensor PIR para detectar movimento
  if (alarmeAtivado) {
    movimentoDetectado = detectaMovimentoPIR();
    
    if (movimentoDetectado) {
      disparaAlarme();
    } 
    else {
      piscaLed(vermelhoLedPin);  // Pisca o LED vermelho enquanto não há movimento
    }
  }
}

void defineCorLed(int vermelho, int verde, int azul) {
    analogWrite(vermelhoLedPin, vermelho);
    analogWrite(verdeLedPin, verde);
    analogWrite(azulLedPin, azul);
}

void disparaAlarme() {
  escreveLCD("Casa Invadida!", 1);
  while (alarmeAtivado && movimentoDetectado) {  // Continua o alarme até desativação
    tone(buzzerPin, 1000); // Toca uma nota de 1000 Hz
    piscaLed(vermelhoLedPin); // Pisca o LED vermelho
    delay(500);
  }
}

void resetaAlarme() {
  noTone(buzzerPin); // Desliga o som
  defineCorLed(0, 255, 0); // LED verde para indicar sistema pronto
  escreveLCD("Alarme Desativado", 0);
  escreveLCD("                   ", 1); // Limpa segunda linha do LCD
}

bool detectaMovimentoPIR() {
  delay(200);  // Evita leituras muito rápidas
  int sensorValue = digitalRead(pirPin); // Lê o valor do sensor PIR
  
  if (sensorValue == HIGH) { // Movimento detectado
    return true;
  }
  return false;
}

void escreveLCD(String mensagem, int linha) {
  lcd.setCursor(0, linha);
  lcd.print(mensagem);
}

void piscaLed(int ledPin) {
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
}
