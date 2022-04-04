#define PIN_POWER 0
#define PIN_SIGNAL 13
//#define PIN_SDA 12
//#define PIN_SCL 14
//#define PZEM_RX_PIN 4
//#define PZEM_TX_PIN 5

#define PIN_SDA 4
#define PIN_SCL 5
#define PZEM_RX_PIN 12
#define PZEM_TX_PIN 14

#include "SparkFun_SHTC3.h" // Click here to get the library: http://librarymanager/All#SparkFun_SHTC3
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>



SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial);


SHTC3 mySHTC3;              // Declare an instance of the SHTC3 class

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_POWER, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_SIGNAL, INPUT);
  
  Serial.begin(115200);
  while(Serial == false){};// Begin Serial 
  Serial.println("SHTC3 Example 1 - Basic Readings");
  Wire.begin(PIN_SDA, PIN_SCL);
  Serial.print("Beginning sensor. Result = ");           // Most SHTC3 functions return a variable of the type "SHTC3_Status_TypeDef" to indicate the status of their execution 
  errorDecoder(mySHTC3.begin());                              // To start the sensor you must call "begin()", the default settings use Wire (default Arduino I2C port)
  Serial.println();
  Serial.println("\n\n");
  Serial.println("Waiting for 5 seconds so you can read this info ^^^");

  delay(5000);  
}

void loop() {
  Serial.print("PIN Status : ");
  Serial.println(digitalRead(PIN_SIGNAL));
  SHTC3_Status_TypeDef result = mySHTC3.update();             // Call "update()" to command a measurement, wait for measurement to complete, and update the RH and T members of the object
  printInfo();                                                // This function is used to print a nice little line of info to the serial port

      Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);

    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    // Check if the data is valid
    if(isnan(voltage)){
        Serial.println("Error reading voltage");
    } else if (isnan(current)) {
        Serial.println("Error reading current");
    } else if (isnan(power)) {
        Serial.println("Error reading power");
    } else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else {

        // Print the values to the Serial console
        Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("PF: ");           Serial.println(pf);
    }

    Serial.println();

    
  delay(190);
  // put your main code here, to run repeatedly:
  digitalWrite(PIN_POWER, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(PIN_POWER, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  delay(4000);
}

///////////////////////
// Utility Functions //
///////////////////////
void printInfo()
{
  if(mySHTC3.lastStatus == SHTC3_Status_Nominal)              // You can also assess the status of the last command by checking the ".lastStatus" member of the object
  {
    Serial.print("RH = "); 
    Serial.print(mySHTC3.toPercent());                        // "toPercent" returns the percent humidity as a floating point number
    Serial.print("%, T = "); 
    Serial.print(mySHTC3.toDegC());                           // "toDegF" and "toDegC" return the temperature as a flaoting point number in deg F and deg C respectively 
    Serial.println(" deg C"); 
  }
  else
  {
    Serial.print("Update failed, error: "); 
    errorDecoder(mySHTC3.lastStatus);
    Serial.println();
  }
}

void errorDecoder(SHTC3_Status_TypeDef message)                             // The errorDecoder function prints "SHTC3_Status_TypeDef" resultsin a human-friendly way
{
  switch(message)
  {
    case SHTC3_Status_Nominal : Serial.print("Nominal"); break;
    case SHTC3_Status_Error : Serial.print("Error"); break;
    case SHTC3_Status_CRC_Fail : Serial.print("CRC Fail"); break;
    default : Serial.print("Unknown return code"); break;
  }
}
