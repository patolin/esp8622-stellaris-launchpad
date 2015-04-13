#include "Energia.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_sysctl.h"


// Sensor de Temperatura Interno
unsigned long ulADC0Value[4];
volatile unsigned long ulTempAvg;
volatile unsigned long ulTempValueC;
volatile unsigned long ulTempValueF;

void setup() {
  Serial.begin(115200);
  //mySerial.selectModule(3);
  Serial3.begin(115200);
  
  // habilitamos los leds
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  
  // habilitamos el sensor de temperatura interno del stellaris
  SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC0;  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
  SysCtlADCSpeedSet(SYSCTL_ADCSPEED_125KSPS); // 250
  ADCSequenceDisable(ADC0_BASE, 1);
  ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
  ADCSequenceEnable(ADC0_BASE, 1);


  digitalWrite(RED_LED,1);
  delay(250);
  digitalWrite(RED_LED,0);
  delay(250);
}

void loop() {
  if (Serial3.available()) {
    char c=Serial3.read();
    if (c=='0') {
       // apagado 
       digitalWrite(RED_LED,0);
       digitalWrite(GREEN_LED,0);
       digitalWrite(BLUE_LED,0);
    }
    if (c=='1') {
       // apagado 
       digitalWrite(RED_LED,1);
       digitalWrite(GREEN_LED,0);
       digitalWrite(BLUE_LED,0);
    }
    if (c=='2') {
       // apagado 
       digitalWrite(RED_LED,0);
       digitalWrite(GREEN_LED,1);
       digitalWrite(BLUE_LED,0);
    }
    if (c=='3') {
       // apagado 
       digitalWrite(RED_LED,0);
       digitalWrite(GREEN_LED,0);
       digitalWrite(BLUE_LED,1);
    }
    if (c=='s') {
       // enviamos el dato de temperatura
              ADCIntClear(ADC0_BASE, 1);
              ADCProcessorTrigger(ADC0_BASE, 1);
              
              while(!ADCIntStatus(ADC0_BASE, 1, false))
              {    
              }
            
              ADCSequenceDataGet(ADC0_BASE, 1, ulADC0Value);
              ulTempAvg = (ulADC0Value[0] + ulADC0Value[1] + ulADC0Value[2] + ulADC0Value[3] + 2)/4;
              ulTempValueC = (1475 - ((2475 * ulTempAvg)) / 4096)/10;
              ulTempValueF = ((ulTempValueC * 9) + 160) / 5;
              Serial3.print(char(ulTempValueC));
    }
    

  }
  
  /*
  if (Serial.available()) {
    char c=Serial.read();
    Serial3.print(c); 
  }
  */
  
  
}
