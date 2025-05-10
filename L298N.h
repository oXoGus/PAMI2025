//Pin Setup
#define ENA 1
#define IN1 2
#define IN2 3
#define IN3 37
#define IN4 36
#define ENB 35
#define PIN_ENCODEUR_G 13

#define PERIM 326.725635973
#define IMP_PAR_TOUR 12




void setup_moteur(){
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}



void activer_moteur(){
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
}



void avancerToutDroit(char v){
  activer_moteur();

  avancerMG(v - 13);
  avancerMD(v);
}

void courbeDroite(char v){
  activer_moteur();

  avancerMG(v);
  avancerMD(v);
}

void avancerMG(int v){
  digitalWrite(IN1, LOW);
  analogWrite(IN2, v - 13); // MG va plus vite que MD
}

void avncerMD(int v){
  digitalWrite(IN3, LOW);
  analogWrite(IN4, v);
}

void stop(){
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void reculer(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void gauche(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void droite(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}


void asserv(int dist){
  // aservisement pid avec une trajectoire en ligne droite
  
}

