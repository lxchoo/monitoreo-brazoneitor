#include <Servo.h>
int pos1=80, pos2=60, pos3=130, pos4=0; 
//Ángulos inicio ejes
int inicioBase=90, cierraPinza = 0, inicioExtiende = 45, inicioAltura=0; 
//Ángulos recoger
int abrePinza = 75, extiendeBrazo=0, bajaBrazo= 45;
//Ángulos drop
int recogeBrazo=20, alturaDrop=25;
//Medicion sensor
long tiempo1;
//Estados
bool estadoTest=false,estadoBusqueda=false,estadoDeteccion=false,estadoAgarre=false,estadoDrop=false, estadoPos1=false,estadoPos0=true;
//Pines Botones
int pinStart=7, pinReinicio=8; 
//Pines Sensores
int Trigger1=A5, Echo1=A4;    
//Inicia Servos
Servo servo_pinza;
Servo servo_extiende;
Servo servo_lados;
Servo servo_altura;

void setup()
{
  Serial.begin(9600);
  servo_lados.attach(A1, 500, 2500);
  servo_extiende.attach(A3, 500, 2500);
  servo_altura.attach(6, 500, 2500);
  servo_pinza.attach(9, 500, 2500);

  pinMode(Trigger1, OUTPUT);
  pinMode(Echo1, INPUT);

  pinMode(pinStart, INPUT);
  pinMode(pinReinicio, INPUT);
}
//Funcion mueve servo suave
void moverServo(Servo&servo, int anguloFinal, int velocidad) {
  int anguloInicial = servo.read();

  if (anguloInicial < anguloFinal) {
    for (int angulo = anguloInicial; angulo <= anguloFinal; angulo++) {
      servo.write(angulo);
      delay(velocidad);
    }
  } else {
    for (int angulo = anguloInicial; angulo >= anguloFinal; angulo--) {
      servo.write(angulo);
      delay(velocidad);
    }
  }
}

void posicionOriginalSuave() {
  moverServo(servo_lados, inicioBase, 20); 
  delay(250);
  moverServo(servo_pinza, cierraPinza, 20); 
  delay(250);
  moverServo(servo_extiende, inicioExtiende, 20);
  delay(250);
  moverServo(servo_altura, inicioAltura, 20); 
  delay(250);
}

void inicioTest(){
  moverServo(servo_pinza, pos4, 20); 
  delay(250);
  moverServo(servo_extiende, pos2, 20);
  delay(250);
  moverServo(servo_altura, pos3, 20); 
  delay(250);
  moverServo(servo_pinza,cierraPinza,20);
  delay(250);
  moverServo(servo_extiende, inicioExtiende, 20);
  delay(250);
  moverServo(servo_altura, inicioAltura, 20);
}



void sueltaObjeto(){
  moverServo(servo_altura, alturaDrop, 20);
  delay(1000);
  moverServo(servo_pinza, abrePinza, 20);
}

void tomaObjeto(){
  moverServo(servo_pinza,cierraPinza,20);
  delay(1500);
  sueltaObjeto();
}

void objetoSensor(){
  Serial.println("Objeto detectado");
  delay(1000);
  moverServo(servo_pinza,abrePinza,20);
  delay(250);
  moverServo(servo_extiende, extiendeBrazo, 20);
  delay(250);
  moverServo(servo_altura,bajaBrazo,20);
  delay(1500);
  tomaObjeto();
}

void medirSensor(){
  digitalWrite(Trigger1,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger1,LOW);

  tiempo1=pulseIn(Echo1,HIGH);

  Serial.println(tiempo1);
  if (tiempo1>450 && tiempo1<600){
    estadoBusqueda=false;
    estadoDeteccion=true;
    Serial.println("estadoDeteccion");
  }

  delay(500);
}

void loop() {
  if (estadoPos0){
    Serial.println("estadoPos0");
    posicionOriginalSuave();
    int botonStart = digitalRead(pinStart);
    if (botonStart==HIGH){
      estadoPos0=false;
      estadoTest=true;
    }
    //estadoPos0=false;
    //estadoTest=true;    ////DEJAR SOLO POS0 FALSE Y SI SE PRESIONA EL BOTON TEST HACER ESTADOTEST TRUE
  }
  if (estadoTest){
    Serial.println("estadoTest");
    inicioTest();
    estadoTest=false;
    estadoPos1=true;
  }
  if (estadoPos1){
    Serial.println("EstadoPos1");
    posicionOriginalSuave();
    estadoPos1=false;
    estadoBusqueda=true;
  }
  if (estadoBusqueda){
    medirSensor();
  }
  if (estadoDeteccion){
    objetoSensor();
    estadoDeteccion=false;
    estadoDrop=true;
  }
  if (estadoDrop){
    Serial.println("estadoDrop");
    sueltaObjeto();
    //estadoDrop=false;
    //estadoPos1=true;    //DEJAR SOLO DROP FALSE Y SI SE PRESIONA EL BOTON REINICIO HACER ESTADOPOS1 TRUE
    int botonReinicio = digitalRead(pinReinicio);
    if (botonReinicio==HIGH){
      estadoDrop=false;
      estadoPos1=true;
    }
  }
  

}
