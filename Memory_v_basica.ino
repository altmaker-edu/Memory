// Versão: Memory - versão básica
// O código a seguir implementa um jogo de sequência de luzes e sons.
// O jogo gera uma sequência aleatória de luzes e notas musicais, 
// e o jogador deve repetir a sequência pressionando os botões correspondentes.
// Se o jogador acertar, a sequência aumenta; se errar, o jogo indica "Game Over"
// com uma sequência especial de luzes e sons.

// Definição de notas musicais
#define NOTE_D4  294 
#define NOTE_G4  392 
#define NOTE_A4  440 
#define NOTE_A5  880   

// Declaração de variáveis globais
int tons[4] = { NOTE_A5, NOTE_A4, NOTE_G4, NOTE_D4 }; 
int sequencia[100] = {}; 
int rodada_atual = 0; 
int passo_atual_na_sequencia = 0;   
int pinoAudio = 4; 
int pinosLeds[4] = {9, 10, 11, 12}; 
int pinosBotoes[4] = {5, 6, 7, 8};   
int botao_pressionado = 0; 
boolean perdeu_o_jogo = false;   

// Configuração inicial
void setup() {   
  // Configurar pinos dos LEDs como saídas
  for (int i = 0; i < 4; i++) {     
    pinMode(pinosLeds[i], OUTPUT); 
  } 
  // Configurar pinos dos botões como entradas com pull-up
  for (int i = 0; i < 4; i++) {     
    pinMode(pinosBotoes[i], INPUT_PULLUP); 
  } 
  // Configurar pino do alto-falante como saída
  pinMode(pinoAudio, OUTPUT); 
  // Inicializar a semente para números aleatórios
  randomSeed(analogRead(0)); 
}   

// Loop principal
void loop() {   
  // Se o jogador perdeu, reiniciar o jogo
  if (perdeu_o_jogo) {     
    reiniciarJogo(); 
  }     
  
  // Se for a primeira rodada, tocar o som de início
  if (rodada_atual == 0) {     
    tocarSomDeInicio(); 
    delay(500); 
  }     
  
  // Avançar para a próxima rodada
  proximaRodada();   
  
  // Reproduzir a sequência para o jogador
  reproduzirSequencia();   
  
  // Aguardar a jogada do jogador
  aguardarJogador();     
  
  // Aguardar um curto intervalo antes da próxima rodada
  delay(1000); 
}   

// Função para gerar a próxima rodada
void proximaRodada() {   
  int numero_sorteado = random(0, 4);   
  sequencia[rodada_atual++] = numero_sorteado; 
}   

// Função para reproduzir a sequência para o jogador
void reproduzirSequencia() {   
  for (int i = 0; i < rodada_atual; i++) {     
    // Tocar a nota musical
    tone(pinoAudio, tons[sequencia[i]]);     
    // Acender o LED correspondente
    digitalWrite(pinosLeds[sequencia[i]], HIGH);     
    delay(500);     
    // Desligar a nota musical e o LED
    noTone(pinoAudio);     
    digitalWrite(pinosLeds[sequencia[i]], LOW);     
    delay(100); 
  }   
  // Desligar a nota musical no final da sequência
  noTone(pinoAudio); 
}   

// Função para aguardar a jogada do jogador
void aguardarJogador() {   
  for (int i = 0; i < rodada_atual; i++) {     
    aguardarJogada();     
    verificarJogada();       
    // Se o jogador perdeu, encerrar o loop
    if (perdeu_o_jogo) {       
      break;     
    }       
    passo_atual_na_sequencia++; 
  }     
  // Reiniciar o passo_atual_na_sequencia para a próxima rodada
  passo_atual_na_sequencia = 0; 
}   

// Função para aguardar a jogada do jogador
void aguardarJogada() {   
  boolean jogada_efetuada = false;   
  // Loop até que o jogador faça uma jogada
  while (!jogada_efetuada) {     
    for (int i = 0; i < 4; i++) {       
      // Verificar se um botão foi pressionado
      if (digitalRead(pinosBotoes[i]) == LOW) {       
        botao_pressionado = i;           
        // Tocar a nota musical correspondente ao botão
        tone(pinoAudio, tons[i]);         
        // Acender o LED correspondente ao botão
        digitalWrite(pinosLeds[i], HIGH);         
        delay(300);         
        // Desligar o LED e a nota musical
        digitalWrite(pinosLeds[i], LOW);         
        noTone(pinoAudio);           
        // A jogada foi efetuada
        jogada_efetuada = true; 
      }     
    }     
    // Pequeno atraso para evitar leituras falsas
    delay(10); 
  } 
}   

// Função para verificar a jogada do jogador
void verificarJogada() {   
  if (sequencia[passo_atual_na_sequencia] != botao_pressionado) {     
    // Jogador errou, indicar "Game Over"
    indicarGameOver(); 
  } 
}  

// Função para tocar o som de início
void tocarSomDeInicio() {   
  // Tocar a primeira nota musical
  tone(pinoAudio, tons[0]);   
  // Acender todos os LEDs
  for (int i = 0; i < 4; i++) {   
    digitalWrite(pinosLeds[i], HIGH);   
  }   
  delay(500);   
  // Desligar todos os LEDs
  for (int i = 0; i < 4; i++) {   
    digitalWrite(pinosLeds[i], LOW);   
  }   
  delay(500);   
  // Desligar a nota musical
  noTone(pinoAudio); 
}

// Função para reiniciar o jogo
void reiniciarJogo() {   
  // Limpar a sequência e reiniciar variáveis
  int sequencia[100] = {};   
  rodada_atual = 0;   
  passo_atual_na_sequencia = 0;   
  perdeu_o_jogo = false; 
}

// Função para indicar "Game Over"
void indicarGameOver() {   
  // Tocar sequência de notas indicando "Game Over"
  for (int i = 0; i < 4; i++) {     
    tone(pinoAudio, tons[i]);     
    digitalWrite(pinosLeds[i], HIGH);     
    delay(200);     
    digitalWrite(pinosLeds[i], LOW);

     
    noTone(pinoAudio);   
  }   
  // Tocar uma nota mais longa indicando "Game Over"
  tone(pinoAudio, tons[3]);   
  // Acender todos os LEDs
  for (int i = 0; i < 4; i++) {     
    digitalWrite(pinosLeds[i], HIGH);   
  }   
  delay(100);   
  // Desligar todos os LEDs
  for (int i = 0; i < 4; i++) {     
    digitalWrite(pinosLeds[i], LOW);   
  }   
  delay(100);   
  // Desligar a nota musical
  noTone(pinoAudio);   
  // Indicar que o jogador perdeu o jogo
  perdeu_o_jogo = true; 
}