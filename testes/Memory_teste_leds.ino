// Este programa acende e apaga LEDs conectados aos pinos de 2 a 12 em sequência.

void setup() {
  // Configuração inicial: define os pinos de 2 a 12 como saídas para controle dos LEDs.
  for (int x = 2; x <= 12; x++) {
    pinMode(x, OUTPUT);
  }
}

void loop() {
  // Loop principal que acende os LEDs em sequência.

  // Acende cada LED em sequência.
  for (int x = 2; x <= 12; x++) {
    digitalWrite(x, HIGH);
  }
  // Aguarda por 100 milissegundos (0,1 segundo).
  delay(100);

  // Apaga cada LED em sequência.
  for (int x = 2; x <= 12; x++) {
    digitalWrite(x, LOW);
  }
  // Aguarda por 100 milissegundos (0,1 segundo).
  delay(100);
}
