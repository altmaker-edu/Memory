// Versao: Memory - versao LCD
// Este código implementa um jogo de memória com luzes, sons e um display LCD.
// O jogo exibe uma sequência de luzes e notas musicais que o jogador deve memorizar e repetir.
// O jogador pressiona botões correspondentes aos LEDs para repetir a sequência.
// Se a sequência estiver correta, o jogo avança para a próxima rodada; 
// se errar, o jogo indica "Game Over" com uma sequência especial de luzes e sons.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C display1(0x3F, 16, 2); // Inicialização do display LCD I2C

#define NOTE_D4 294
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_A5 880

int tons[4] = {NOTE_A5, NOTE_A4, NOTE_G4, NOTE_D4}; // Array para os sons sorteados
int sequencia[100] = {};                           // Sequência inicialmente vazia
int rodada_atual = 0;                               // Rodada atual do jogo
int passo_atual_na_sequencia = 0;                  // Passo atual enquanto a sequência é reproduzida
int pinoAudio = 4;
int pinosLeds[4] = {9, 10, 11, 12};
int pinosBotoes[4] = {5, 6, 7, 8};
int botao_pressionado = 0;    // Botão pressionado durante o loop principal
int perdeu_o_jogo = false;    // Indica se o jogo acabou
int acertos = 0;              // Conta os acertos do jogador

void setup() {
  // Inicialização do display LCD
  display1.init();
  display1.backlight();
  display1.clear();
  display1.print("Iniciando Memory");
  display1.setCursor(0, 1);
  display1.print(" ABR TECNOLOGIA ");

  Serial.begin(9600);

  // Configuração dos LEDs e botões
  for (int i = 0; i <= 3; i++) {
    pinMode(pinosLeds[i], OUTPUT);
  }

  for (int i = 0; i <= 3; i++) {
    pinMode(pinosBotoes[i], INPUT_PULLUP);
  }

  pinMode(pinoAudio, OUTPUT);

  randomSeed(analogRead(0));

  // Toca o som de início e aguarda
  tocarSomDeInicio();
  delay(500);
}

void loop() {
  // Configuração do display para a rodada atual
  display1.setBacklight(HIGH);
  display1.clear();
  display1.setCursor(0, 0);
  display1.print(" JOGANDO MEMORY ");
  display1.setCursor(0, 1);
  display1.print("JOGADA: ");
  display1.print(rodada_atual + 1);

  delay(100);

  // Reinicializa as variáveis se o jogador perdeu o jogo
  if (perdeu_o_jogo) {
    int sequencia[100] = {};
    rodada_atual = 0;
    passo_atual_na_sequencia = 0;
    acertos = 0;
    perdeu_o_jogo = false;
    tocarSomDeInicio();
  } else {
    acertos++;
  }

  proximaRodada();
  reproduzirSequencia();
  aguardarJogador();

  delay(500);
}

// Adiciona um novo item à sequência
void proximaRodada() {
  int numero_sorteado = random(0, 4);
  sequencia[rodada_atual++] = numero_sorteado;
}

// Reproduz a sequência para o jogador
void reproduzirSequencia() {
  for (int i = 0; i < rodada_atual; i++) {
    tone(pinoAudio, tons[sequencia[i]]);
    digitalWrite(pinosLeds[sequencia[i]], HIGH);
    delay(500);
    noTone(pinoAudio);
    digitalWrite(pinosLeds[sequencia[i]], LOW);
    delay(100);
  }
  noTone(pinoAudio);
}

// Aguarda o jogador iniciar sua jogada
void aguardarJogador() {
  for (int i = 0; i < rodada_atual; i++) {
    aguardarJogada();
    verificarJogada();
    if (perdeu_o_jogo) {
      break;
    }
    passo_atual_na_sequencia++;
  }
  passo_atual_na_sequencia = 0;
}

// Aguarda a jogada do jogador
void aguardarJogada() {
  boolean jogada_efetuada = false;
  while (!jogada_efetuada) {
    for (int i = 0; i <= 3; i++) {
      if (digitalRead(pinosBotoes[i]) == 0) {
        botao_pressionado = i;
        tone(pinoAudio, tons[i]);
        digitalWrite(pinosLeds[i], HIGH);
        delay
