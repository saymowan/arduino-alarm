#include <LiquidCrystal.h> // Biblioteca LCD

LiquidCrystal lcd(12, 13, 5, 4, 3, 2); // Pinos do LCD
int vermelhoLedPin = 8;
int verdeLedPin = 9;
int azulLedPin = 10;
int buzzerPin = 6; // Pino do buzzer
int pirPin = 11;    // Pino do sensor PIR
int botaoAtivarAlarmePin = 7;    // Pino do botão de ativar/desativar alarme

bool alarmeAtivado = false;  // Estado do alarme
bool movimentoDetectado = false;  // Estado do sensor PIR
bool movimentoPersistente = false; // Estado persistente do movimento detectado
bool buttonStateAtivarAlarme = false;
bool lastButtonState = LOW; // Estado anterior do botão
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // Tempo de debounce para o botão

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial
  lcd.begin(16, 2);
  lcd.print("ALARME CASA");

  // Configurações dos pinos
  pinMode(vermelhoLedPin, OUTPUT); 
  pinMode(verdeLedPin, OUTPUT); 
  pinMode(azulLedPin, OUTPUT); 
  pinMode(pirPin, INPUT);   
  pinMode(buzzerPin, OUTPUT); 
  pinMode(botaoAtivarAlarmePin, INPUT); 
}

void loop() {
  // Lógica de debounce do botão
  bool reading = digitalRead(botaoAtivarAlarmePin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == HIGH && !buttonStateAtivarAlarme) {
      buttonStateAtivarAlarme = true;

      // Alterna o estado do alarme
      if (!alarmeAtivado) {
        alarmeAtivado = true;
        movimentoPersistente = false;
        escreveLCD("Alarme Ativado   ", 0);
        defineCorLed(0, 0, 255); // LED verde para indicar sistema pronto

      } else {
        alarmeAtivado = false;
        movimentoPersistente = false;
        resetaAlarme();
      }
    } else if (reading == LOW) {
      buttonStateAtivarAlarme = false;
    }
  }

  lastButtonState = reading;

  // Se o alarme está ativado, verifica o sensor PIR para detectar movimento
  if (alarmeAtivado) {
    movimentoDetectado = detectaMovimentoPIR();
    
    // Se houver movimento, ativa o estado persistente de movimento
    if (movimentoDetectado) {
      movimentoPersistente = true;
    }

    // Dispara o alarme se o movimento foi detectado e está persistente
    if (movimentoPersistente) {
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
  tone(buzzerPin, 1000); // Toca uma nota de 1000 Hz enquanto o alarme está ativo
  
  while (alarmeAtivado && movimentoPersistente) {  // Continua o alarme até desativação
    piscaLed(vermelhoLedPin); // Pisca o LED vermelho
    delay(500);

    // Verifica se o botão foi pressionado para desativar o alarme
    if (digitalRead(botaoAtivarAlarmePin) == HIGH) {
      alarmeAtivado = false;
      movimentoPersistente = false; // Reseta o estado de movimento persistente
      resetaAlarme();
      break; // Sai do loop se o alarme for desativado
    }
  }
  
  noTone(buzzerPin); // Desliga o som quando o alarme é desativado
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