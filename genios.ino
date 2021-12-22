#define LED_VERMELHO 8
#define LED_BRANCO 9
#define LED_AMARELO 10
#define LED_AZUL 11
#define INDEFINIDO -1

#define BOTAO_VERMELHO 2
#define BOTAO_BRANCO 3
#define BOTAO_AMARELO 4
#define BOTAO_AZUL 5

#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500

#define TAMANHO_SEQUENCIA 4

enum Estados{
  PRONTO_PARA_PROX_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO,
  JOGO_FINALIZADO_FALHA 
};

int sequenciaLuzes[TAMANHO_SEQUENCIA];

int ledsRespondidos = 0;

int rodada = 0;

void setup() {

  Serial.begin(9600);
  iniciaPortas();
  iniciaJogo();
  
}

void loop() {
  switch(estadoAtual()){
    case PRONTO_PARA_PROX_RODADA:
      preparaNovaRodada();
      break;
    case USUARIO_RESPONDENDO:
      processaRespostaUsuario();
      break;
    case JOGO_FINALIZADO:
      jogoFinalizadoSucesso();
      break;
    case JOGO_FINALIZADO_FALHA:
      jogoFinalizadoComFalha();
      break;
  }
}

void processaRespostaUsuario(){
  int resposta = checaRespostaJogador();

  if(resposta == INDEFINIDO) {
    return;
  }
  
  if(resposta == sequenciaLuzes[ledsRespondidos]){
    ledsRespondidos++;
  } else {
    rodada = TAMANHO_SEQUENCIA + 2;
  }
}

void preparaNovaRodada(){
  rodada++;
  ledsRespondidos = 0;
  if(rodada <= TAMANHO_SEQUENCIA){
     tocaLedsRodada(); 
  }
}

int estadoAtual() {
  if(rodada <= TAMANHO_SEQUENCIA){
    if(ledsRespondidos == rodada){
      return PRONTO_PARA_PROX_RODADA;
    } else {
      return USUARIO_RESPONDENDO;
    }
    
  } else if(rodada == TAMANHO_SEQUENCIA + 1){
    return JOGO_FINALIZADO;    
  }
  else {
    return JOGO_FINALIZADO_FALHA;
  }

}

void  tocaLedsRodada(){
  for (int indice = 0; indice < rodada; indice++){
    piscaLed(sequenciaLuzes[indice]);
  }
}

int checaRespostaJogador(){
  if(digitalRead(BOTAO_AZUL) == LOW) {
    return piscaLed(LED_AZUL);
  }

  if(digitalRead(BOTAO_AMARELO) == LOW) {
    return piscaLed(LED_AMARELO);
  }

  if(digitalRead(BOTAO_BRANCO) == LOW) {
    return piscaLed(LED_BRANCO);
  }

  if(digitalRead(BOTAO_VERMELHO) == LOW) {
    return piscaLed(LED_VERMELHO);
  }

  return INDEFINIDO;
}

void iniciaJogo() {
  int numeroJogo = analogRead(0);
  randomSeed(numeroJogo);
  
  for (int indice = 0; indice < TAMANHO_SEQUENCIA; indice++){
    sequenciaLuzes[indice] = sorteiaCor();  
  }
}

int sorteiaCor(){
  return random(LED_VERMELHO, LED_AZUL + 1);
}

void iniciaPortas(){
  pinMode(LED_BRANCO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  
  pinMode(BOTAO_BRANCO, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);
}

void jogoFinalizadoSucesso(){
  piscaLed(LED_AZUL);
  piscaLed(LED_AMARELO);
  piscaLed(LED_BRANCO);
  piscaLed(LED_VERMELHO);
  delay(MEIO_SEGUNDO);
}

void jogoFinalizadoComFalha(){
  digitalWrite(LED_BRANCO, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(LED_BRANCO, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
  delay(MEIO_SEGUNDO);
}

int piscaLed(int portaLed){
  digitalWrite(portaLed, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(portaLed, LOW);
  return portaLed;
}
