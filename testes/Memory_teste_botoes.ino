// Versao - Teste de botões do Memory
// Este programa lê o estado de botões conectados aos pinos de 3 a 11.
// Quando um botão é pressionado, uma mensagem é exibida no console Serial.

void setup() {
  // Inicialização da comunicação serial com uma taxa de transmissão de 9600 bps.
  Serial.begin(9600);

  // Configuração dos pinos de 3 a 11 como entrada com pull-up interno.
  // Isso significa que os pinos são mantidos em nível lógico alto (1) por padrão,
  // e serão lidos como nível lógico baixo (0) quando um botão for pressionado.
  for (int i = 3; i <= 11; i++) {     
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  // Loop principal para verificar o estado dos botões.
  for (int i = 3; i <= 11; i++) {
    // Verifica se o botão no pino atual está pressionado (nível lógico baixo).
    if (digitalRead(i) == 0) {
      // Se o botão foi pressionado, exibe uma mensagem indicando o número do botão.
      Serial.print("### Botão ");
      // O número do botão é calculado subtraindo 2 do número do pino.
      Serial.print(i - 2);
      Serial.println(" ###");
      
      // Aguarda por 1000 milissegundos (1 segundo) para evitar leituras múltiplas
      // enquanto o botão permanece pressionado.
      delay(1000);
    }
  }
}
