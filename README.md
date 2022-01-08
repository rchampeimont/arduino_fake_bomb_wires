# arduino_fake_bomb_wires
A prop bomb where you need to cut the right wires. This was used as part of an escape game we organized for our family.

General idea: To "defuse" the "bomb", the players need to cut the right wires (so you need to solder new wires for every game). When the bomb is defused, it displays a secret code (which could for example allow to open a lock sealing the bomb or anything else in an escape game).

Features:
* 6 wires in which you have to cut the right ones
* Countdown to 0
* When no time is left, screen goes red and displays "BOOM"
* For each correct wire that is cut, an asterisk displays on the screen
* When all the correct wires are cut, timer stops, screen goes green and displays a secret code 
* As long as an incorect wire is cut, time goes down twice as fast and screen is red
* The bomb can still be defused if an incorrect wire has been cut, provided the wire is reconnected (you could provide players crocodile clips or even solder & iron)

You can power it the way you want, in my case I used a 9V battery (connected to Vin and GND).

## Schematic
![Schematic](fake_bomb_schematic.png?raw=true)

## Example breadboard setup
The idea is to have the wires to cut accessible from outside the bomb while the rest of the circuit is sealed. The wires are soldered to prevent the players from just pulling the wires instead of cutting them.
![Breadboard](fake_bomb_breadboard.png?raw=true)

## Photos
![Photo of LCD screen](photo_LCD.jpg?raw=true)
