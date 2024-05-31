// ACS712 5A  module --> scaling: 185 mV/Amp
// ACS712 20A module --> scaling: 100 mV/Amp
// ACS712 30A module --> scaling:  66 mV/Amp


// Many measurements in a short time can lead to problems with the WiFi!!
// Because:
// The same ADC is used on the 8266 by WiFi to test the signal level. 
// Hard use of analogRead(A0) influences the readings of the WiFi RD level. 
// This can be set with the "Delay_Time"
// The value 2 works when WiFi is simply connected.
// If you want to work with the WiFiEventHandler, e.g. to detect a disconnect, the value should better be 4
// do_measurements should then be reduced to 25. Of course, this reduces the accuracy


// Using the AD converter of the ESP8266 for the ACs712

// It's not difficult at all, especially if you use the WeMos D1 mini.

// The D1 mini already has an integrated voltage divider, so you are not limited to the 1.0V of the ESP8266 ADC.
// The voltage divider of the D1 mini is designed for 3.3V.
// Now we just have to extend it so that it works with the 5V of the ACS712.
// We do this by simply connecting a 180k resistor in series with the A0 input

#define SUM_MAX 600
#define ADC_MAX 1024
#define Noise_Filter 4
#define Scaling 185.0
#define Delay_Time 2

int count = 0;
int adc_avr = 0;
uint16_t vals[ADC_MAX + 1];
unsigned long time_m;
unsigned long time_mn;
float Ieff;
float P;
float W;


void do_measurements(int n) {
  uint16_t last_adc_read;

  for (; n > 0; n--) {
    count++;
    if (count >= SUM_MAX) {
      float U, Ueff;
      float Sum_U_Square = 0.0;
      count = 0;
      adc_avr = adc_avr / SUM_MAX;

      for (int i = 0; i < ADC_MAX + 1; i++) {

        if (vals[i] != 0) {
          // filter out noise around zero
          if (abs(i - adc_avr) > Noise_Filter) {
            U = ((float)(i - adc_avr)) / 1024 * 5000;
            Sum_U_Square += U * U * vals[i];
          }
          vals[i] = 0;
        }
      }
      Ueff = sqrt(Sum_U_Square / SUM_MAX);
      Ieff = Ueff / Scaling;
      P = Ieff * 233;
      time_mn = millis();
      W += P * (time_mn - time_m) / 3600000.0;
      time_m = time_mn;

      Serial.print(String(adc_avr) + "  ");
      Serial.print(String(Ueff) + "mV  ");
      Serial.print(String(Ieff) + "A  ");
      Serial.print(String(P) + "W ");
      Serial.println(String(W) + "Wh");

      adc_avr = 0;
    }

    last_adc_read = analogRead(A0);
    if (last_adc_read > ADC_MAX) {
      Serial.println("ADC error");
      continue;
    }
    vals[last_adc_read]++;
    adc_avr += last_adc_read;
    delay(Delay_Time);
  }
}


void setup() {
  Serial.begin(115200);
  time_m = millis();
}


void loop() {
  do_measurements(50);
}