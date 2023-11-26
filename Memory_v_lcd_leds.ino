// Este código implementa um jogo de memória com LEDs, sons, um display LCD e efeitos de luzes.
// O jogo exibe uma sequência de luzes, notas musicais e efeitos visuais que o jogador deve memorizar e repetir.
// O jogador pressiona botões correspondentes aos LEDs para repetir a sequência.
// Se a sequência estiver correta, o jogo avança para a próxima rodada;
// se errar, o jogo indica "Game Over" com uma sequência especial de luzes e sons.


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "FastLED.h"

LiquidCrystal_I2C display1(0x3F, 16, 2); // Inicialização do display LCD I2C

// Definição de tons musicais
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
int acertos = 0;

// Configuração de LEDs com a biblioteca FastLED
#define LED_PIN 13
#define NUM_LEDS 8
#define BRIGHTNESS 255
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 60
int i, x;

void setup() {
    delay(3000);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

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

        // Efeitos visuais associados a cada LED
        if (pinosLeds[sequencia[i]] == 9) {
            vermelho();
        }
        if (pinosLeds[sequencia[i]] == 10) {
            azul();
        }
        if (pinosLeds[sequencia[i]] == 11) {
            verde();
        }
        if (pinosLeds[sequencia[i]] == 12) {
            amarelo();
        }

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
                delay(300);
                digitalWrite(pinosLeds[i], LOW);
                noTone(pinoAudio);
                jogada_efetuada = true;
            }
        }
        delay(10);
    }
}

// Verifica se a jogada do jogador está correta
void verificarJogada() {
    if (sequencia[passo_atual_na_sequencia] != botao_pressionado) {
        // GAME OVER
        display1.setCursor(0, 0);
        display1.print("   GAME OVER    ");
        display1.setCursor(0, 1);
        display1.print("ACERTOS: ");
        display1.print(acertos);
        delay(2500);

        display1.setCursor(0, 0);
        display1.print("   GAME OVER    ");
        display1.setCursor(0, 1);
        display1.print("                ");

        acertos = 0;
        rodada_atual = 0;

        // Efeitos visuais e sonoros indicando o fim do jogo
        for (int i = 0; i <= 3; i++) {
            tone(pinoAudio, tons[i]);
            digitalWrite(pinosLeds[i], HIGH);
            delay(200);
            digitalWrite(pinosLeds[i], LOW);
            noTone(pinoAudio);
        }
        tone(pinoAudio, tons[3]);
        for (int i = 0; i <= 3; i++) {
            digitalWrite(pinosLeds[0], HIGH);
            digitalWrite(pinosLeds[1], HIGH);
            digitalWrite(pinosLeds[2], HIGH);
            digitalWrite(pinosLeds[3], HIGH);
            delay(100);
            digitalWrite(pinosLeds[0], LOW);
            digitalWrite(pinosLeds[1], LOW);
            digitalWrite(pinosLeds[2], LOW);
            digitalWrite(pinosLeds[3], LOW);
            delay(100);
        }
        noTone(pinoAudio);
        perdeu_o_jogo = true;
    }
}

// Toca o som de início do jogo
void tocarSomDeInicio() {
    tone(pinoAudio, tons[0]);
    digitalWrite(pinosLeds[0], HIGH);
    digitalWrite(pinosLeds[1], HIGH);
    digitalWrite(pinosLeds[2], HIGH);
    digitalWrite(pinosLeds[3], HIGH);
    delay(500);
    digitalWrite(pinosLeds[0], LOW);
    digitalWrite(pinosLeds[1], LOW);
    digitalWrite(pinosLeds[2], LOW);
    digitalWrite(pinosLeds[3], LOW);
    delay(500);
    noTone(pinoAudio);
    delay(500);
}

// Funções para os efeitos visuais associados a cada cor
void amarelo() {
    for (i = 0; i < 1; i++) {
        for (x = 0; x < 8; x++) {
            leds[x] = CRGB::Yellow;
        }
        FastLED.show();
        delay(500);
        for (x = 0; x < 8; x++) {
            leds[x] = CRGB::Black;
        }
        FastLED.show();
        delay(100);
    }
}

void verde() {
    for (i = 0; i < 1; i++) {
        for (x = 0; x < 8; x++) {
            leds[x] = CRGB::Green;
        }
        FastLED.show();
        delay(500);
        for (x = 0; x < 8; x++) {
            leds[x] = CRGB::Black;
        }
        FastLED.show();
        delay(100);
    }
}

void azul() {
    for (i = 0; i < 1; i++) {
        for (x = 0; x < 8; x++) {
            leds[x] = CRGB::Blue;
        }
        FastLED.show();
        delay(500);
        for (x = 0; x < 8; x++) {
            leds[x] = CRGB::Black;
        }
        FastLED.show();
        delay(100);
    }
}

void vermelho() {
    for (i = 0; i < 1; i++) {
        for (x = 0; x < 8; x++) {
            leds[x] = CRGB::Red;
        }
        FastLED.show();
        delay(500);
        for (x = 0; x < 8; x++) {
            leds[x] = CRGB::Black;
        }
        FastLED.show();
        delay(100);
    }
}
