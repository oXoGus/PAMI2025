#ifndef ULTRASON_H
#define ULTRASON_H

#define trig 11 // Pin Trigger
#define echo 12    // Pin Echo

void setup_ultrason() {
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  pinMode(echo, INPUT);
}

long mesurerDistance() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Attendre un temps raisonnable pour capturer le retour d'écho
  long lecture_echo = pulseIn(echo, HIGH);

  // Vérifier si une valeur valide a été reçue
  if (lecture_echo == 0) {
    Serial.println("Erreur: Pas de signal Echo détecté.");
    return -1;  // Valeur d'erreur, indique que la mesure a échoué
  }

  long distance = lecture_echo * (340.0 / 1000.0) / 2;  // Calcul de la distance en mm
  //Serial.print("Distance en mm: ");
  //Serial.println(distance);

  return distance;
}

#endif // ULTRASON_H
