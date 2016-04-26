2nd Revision Wireless pendant
-----------------------------

* Switched USB pins to be correct
* Un-mirror ISP so that is on the front
* APA102 LEDs
* Data out from last LED (connect to more!)
* Dot for MPR121 orientation
* Resisters on MISO to radio (to avoid disrupting ISP programmer, see stackexchange article, may be optional 4.7k)
* Removed individual 0.1uf caps for LEDs, replaced with 10UF 1206 Capacitor
* Switch for power
* Replaced regulators' capacitors with 1206 SMD ones
* Change 3.3V to be fed from 5V rather than the raw power, allows USBPWR to be fully separate from RAW, removed diode from USBVCC
* Mounting holes (use for something, since this iteration won't be a good pendant)
* Added AMPWorks stamp
* Added headers for 4 digital pins
* Added headers for 2 digital pins
* Added optional pull-up resistor on RFM69 SS pin
 
TODO
----
* Do not put ground plane behind radio
* Connector for FTDI programmer

TODO After flight
* Figure out how to use hardware SPI for APA102 LEDs
