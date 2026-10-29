#include <SPI.h>
#include <math.h>
#include <SD.h>

const int thermistor_output_1 = A1;
const int thermistor_output_2 = A4;
const int chipSelect = 10; 

float session_timer=0;
File dataFile;

void setup() {

  Serial.begin(9600);

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
 Serial.println("Card initialized.");
}

void loop() {
  int thermistor_adc_val_1;
  int thermistor_adc_val_2;

  double output_voltage_1, thermistor_resistance_1, therm_res_ln_1, temperature_1; 
  double output_voltage_2, thermistor_resistance_2, therm_res_ln_2, temperature_2; 

  thermistor_adc_val_1 = analogRead(thermistor_output_1);
  output_voltage_1 = ( (thermistor_adc_val_1 * 5.0) / 1023.0 );
  thermistor_resistance_1 = ( ( 5 * ( 10.0 / output_voltage_1 ) ) - 10 ); //Resistance in kilo ohms
  thermistor_resistance_1 = thermistor_resistance_1 * 1000 ; // ohms
  therm_res_ln_1 = log(thermistor_resistance_1);

  thermistor_adc_val_2 = analogRead(thermistor_output_2);
  output_voltage_2 = ( (thermistor_adc_val_2 * 5.0) / 1023.0 );
  thermistor_resistance_2 = ( ( 5 * ( 10.0 / output_voltage_2 ) ) - 10 ); //Resistance in kilo ohms
  thermistor_resistance_2 = thermistor_resistance_2 * 1000 ; // ohms
  therm_res_ln_2 = log(thermistor_resistance_2);
 
  /*  Steinhart-Hart Thermistor Equation
  /*  Temperature = 1 / (A + B[ln(R)] + C[ln(R)]^3)   */
  /*  A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  */

  temperature_1 = ( 1 / ( 0.001129148 + ( 0.000234125 * therm_res_ln_1 ) + ( 0.0000000876741 * therm_res_ln_1 * therm_res_ln_1 * therm_res_ln_1 ) ) ); // Kelvin 
  temperature_1 = temperature_1 - 273.15; // Celsius 

  temperature_2 = ( 1 / ( 0.001129148 + ( 0.000234125 * therm_res_ln_2 ) + ( 0.0000000876741 * therm_res_ln_2 * therm_res_ln_2 * therm_res_ln_2 ) ) ); // Kelvin 
  temperature_2 = temperature_2 - 273.15; // Celsius 

  dataFile = SD.open("datalog.txt", FILE_WRITE);

if (session_timer==0){
    dataFile.print("New Session Begin");
}

  if (dataFile) {

    dataFile.print("\n\n");
    dataFile.print("Session Time (minutes) = ");
    dataFile.print(session_timer);
    dataFile.print("\t\t");

    dataFile.print("Temperature 1 = ");
    dataFile.print(temperature_1);
    dataFile.print("\t\t");
    dataFile.print("Resistance 1= ");
    dataFile.print(thermistor_resistance_1);
    dataFile.print("\t\t");

    dataFile.print("Temperature 2 = ");
    dataFile.print(temperature_2);
    dataFile.print("\t\t");
    dataFile.print("Resistance 2 = ");
    dataFile.print(thermistor_resistance_2);
    dataFile.print("\n\n");

    dataFile.close();
  } else {
    Serial.println("Error opening datalog.txt");
  }


  Serial.print("Temperature 1 = ");
  Serial.print(temperature_1);
  Serial.print("\t\t");
  Serial.print("Resistance 1= ");
  Serial.print(thermistor_resistance_1);
  Serial.print("\n\n");

  Serial.print("Temperature 2 = ");
  Serial.print(temperature_2);
  Serial.print("\t\t");
  Serial.print("Resistance 2 = ");
  Serial.print(thermistor_resistance_2);
  Serial.print("\n\n");

  //Serial.print("Resistance Error = ");
  //Serial.print(thermistor_resistance_2 - thermistor_resistance_1);
  //Serial.print("\n\n");

  session_timer=session_timer+15;


  delay(90000);
}
