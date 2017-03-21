#include <EmonLib.h>

#include "VIP.h"             // Include Emon Library
#include "Wire.h"

// endereco do modulo slave que pode ser um valor de 0 a 255
#define myAdress 0x08


VIP emon1;             // Create an instance


//long numero1 = 123456, numero2 = 112233, numero3 = 445566;
long digito;
int aux;
long aux2;
boolean ledState = LOW;

long realPower;      // = emon1.realPower;        //extract Real Power into variable
long apparentPower; // = emon1.apparentPower;    //extract Apparent Power into variable
float powerFactor;   //  = emon1.powerFactor;      //extract Power Factor into Variable
float Vrms;          // = emon1.Vrms;             //extract Vrms into Variable
float Irms;          //  = emon1.Irms;             //extract Irms into Variable
int sfp;

void receiveEvent();
void requestEvent();
void escrever_I2C(long);

void setup() {
  // ingressa ao barramento I2C com o endereço definido no myAdress (0x08)
  Wire.begin(myAdress);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

// Serial.begin(9600);

  emon1.voltage(0, 142, 1.73);  // Voltage: input pin, calibration, phase_shift
  emon1.current(1, 125);       // Current: input pin, calibration.

  pinMode(13, OUTPUT);  // configura o pino do LED como saida
}

void loop() {

  emon1.calcVI(20, 2000);        // Calculate all. No.of half wavelengths (crossings), time-out

 // realPower       = emon1.realPower;        //extract Real Power into variable
 // apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  powerFactor     = emon1.powerFactor;      //extract Power Factor into Variable
  Vrms            = emon1.Vrms;             //extract Vrms into Variable
  Irms            = emon1.Irms;             //extract Irms into Variable
  sfp            = emon1.sfp;             //extrae o sinal do fator de potencia
 //powerFactor = 0.99;
 // Vrms = 100.00;
 //  Irms = 8.00;
}

// funcao executada sempre que algum dado e recebido no barramento I2C
// vide "void setup()"


void receiveEvent() {


  if (Wire.available()) {
    // le o byte recebido
    aux = Wire.read();
  }
}

void requestEvent() {
  //
  ledState = !ledState;
  digitalWrite(13, ledState);


  switch (aux) {
    case 0:
      aux2 = Vrms * 100;
      digito = aux2;
      break;

    case 1:
      aux2 = Irms * 100;
      digito = aux2;
      break;


    case 2:
      aux2 = powerFactor * 100;
      
      if( aux2 >= 99) aux2 = 100; 
            
      digito = aux2;
      break;

    case 3:
      digito = sfp;
      break;  

  }

  escrever_I2C(digito);

}

void escrever_I2C(long valor) {

  int part1, part2, part3;

  part1 = (valor / 65536); // part1 = 12
  part2 = ((valor % 65536) / 256); // part2 = 34
  part3 = ((valor % 65536) % 256); // par3 =  56

  Wire.write(part1);
  Wire.write(part2);
  Wire.write(part3);


 // Serial.println(valor);
 // Serial.print(part1);
 // Serial.print(" ");
 // Serial.print(part2);
 // Serial.print(" ");
 // Serial.println(part3);


 // emon1.serialprint();
}




