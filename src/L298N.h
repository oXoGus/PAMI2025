//Pin Setup
#define ENA 13
#define IN1 14
#define IN2 15
#define IN3 16
#define IN4 17
#define ENB 18

// PWM settings

// un chanel par pwm 
#define PWM_CHANNEL_IN1 0
#define PWM_CHANNEL_IN2 1
#define PWM_CHANNEL_IN3 2
#define PWM_CHANNEL_IN4 3
#define PWM_FREQ 1000
#define PWM_RESOLUTION 8  // 0-255
#define RAMPE_V_START 100

void setup_moteur(){
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // on setup chaque chanel 
  // puis on l'attache au pin
  ledcSetup(PWM_CHANNEL_IN1, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN1, PWM_CHANNEL_IN1);

  ledcSetup(PWM_CHANNEL_IN2, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN2, PWM_CHANNEL_IN2);

  ledcSetup(PWM_CHANNEL_IN3, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN3, PWM_CHANNEL_IN3);

  ledcSetup(PWM_CHANNEL_IN4, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN4, PWM_CHANNEL_IN4);
  
}

void activer_moteur(){
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void desactiver_moteur(){
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
}

void avancerMD(char v){
  ledcWrite(PWM_CHANNEL_IN2, (v - 15 < 0 ? 0 : v - 15));
  digitalWrite(IN1, LOW);
}

void avancerMG(char v){
  digitalWrite(IN4, LOW);
  ledcWrite(PWM_CHANNEL_IN3, v);
}

void reculerMG(int v){
  digitalWrite(IN1, LOW);
  ledcWrite(PWM_CHANNEL_IN2, v);
}

void reculerMD(int v){
  ledcWrite(PWM_CHANNEL_IN3, v);
  digitalWrite(IN4, LOW);
}

// rampe PWM 
void rampePWM(uint8_t canalA, uint8_t canalB, int start, int end, int t) {
  int steps = abs(end - start);
  int delayTime = t / max(steps, 1);

  if (start < end) {
    for (int pwm = start; pwm <= end; pwm++) {
      ledcWrite(canalA, pwm);
      ledcWrite(canalB, pwm);
      delay(delayTime);
    }
  } else {
    for (int pwm = start; pwm >= end; pwm--) {
      ledcWrite(canalA, pwm);
      ledcWrite(canalB, pwm);
      delay(delayTime);
    }
  }
}

// démarer tout droit avec une rampe d'accel
// avec une vitesse cible v en un temps t en ms
void rampeAccelToutDroit(int v, int t){
  // on set IN2 et IN3 a 0 pour le sens du moteur 
  digitalWrite(IN1, 0);
  digitalWrite(IN4, 0);

  // IN1 et 4 pour avancer 
  rampePWM(PWM_CHANNEL_IN2, PWM_CHANNEL_IN3, RAMPE_V_START, v, t);
}



void reculer(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
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


void stop(){
  avancerMD(0);
  avancerMG(0);
}