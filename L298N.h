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

// Déclaration de la variable externe pour l'écran OLED
extern Adafruit_SSD1306 display;

// Compteur d'impulsions pour le moteur gauche
volatile unsigned long impulsionsGauche = 0; 

// Fonction pour incrémenter le compteur du moteur gauche
void IRAM_ATTR compteurImpulsion() {
  impulsionsGauche++;
}



void setup_moteur(){
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // odométrie setup
  pinMode(PIN_ENCODEUR_G, INPUT_PULLUP);  // Encodeur moteur gauche

  // Attacher l'interruption pour l'encodeur
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODEUR_G), compteurImpulsion, RISING);
}



void activer_moteur(){
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
}

void stop(){
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}


// Fonction pour afficher les impulsions sur l'écran OLED
void afficherImpulsions(unsigned long impulsions) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Compteur impulsions:");
  display.print("Gauche: ");
  display.println(impulsions);
  display.display();
}


void avancerToutDroit(char v, int dist){

  // on reset le compteur
  impulsionsGauche = 0;

  // calcul du nombre d'impultion pour parcourir la distance dist
  // l'encodeur envoie 12 impultion par tour 
  // nombre de tours moteur pour parcourir dist
  double nbRotaMoteur = dist/PERIM;
  long impulsionsCible = IMP_PAR_TOUR * nbRotaMoteur;
  

  // annlogWrite pour le PWM
  digitalWrite(IN1, LOW);
  analogWrite(IN2, v);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, v);
  activer_moteur();

  while (impulsionsGauche < impulsionsCible){
    afficherImpulsions(impulsionsGauche);
  }
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

