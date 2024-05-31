# AC Current with ACS712  and ESP8266 D1!

There are a few pitfalls, but if you know them, it works well.
You have to adjust the voltage divider, pay attention to the read speed of the ACD and adjust the noise suppression.


# Why ACS712

 - cheap
 - small
 - available everywhere
 - safe, due to galvanic separation

## Using the AD converter of the ESP8266 for the ACs712

 It's not difficult at all, especially if you use the WeMos D1 mini.
 The D1 mini already has an integrated voltage divider.
 So you are not limited to the 1.0V of the ESP8266 ADC.

The voltage divider of the D1 mini is designed for **3.3V**.
Now we just have to extend it so that it works with the **5V** of the ACS712.
We do this by simply connecting a **180k resistor** in series with the A0 input

## The WiFi pitfall
**Many measurements in a short time can lead to problems with the WiFi!!**

Because:

 The ADC is used on the 8266 by WiFi to test the signal level, too.
Hard use of analogRead(A0) influences the readings of the WiFi RD level.
This can be set with the "Delay_Time"
 The value 2 works when WiFi is simply connected.
 If you want to work with the WiFiEventHandler, e.g. to detect a disconnect, the value should better be 4,
 because the WiFiEventHandler is no longer triggered incorrectly.
"do_measurements" should then be reduced to 25.
Of course, this reduces the accuracy

## Correct scaling value for the different models
ACS712 5A module --> scaling: 185 mV/Amp

ACS712 20A module --> scaling: 100 mV/Amp

ACS712 30A module --> scaling: 66 mV/Amp
