"daemonbite" adapter for the VS1838B ir sensor, specifically for MiSTer + irconclad setup.
---

The ironclad mister itx board (<https://www.d3fmod.com/product/mini-itx_ironclad_plus/>), comes with an IR sensor which lets you control on/off, osd, user and reset using a remote. This code completes the functionality by providing arrow keys and two buttons which can be mapped for osd navigation. Additionally I mapped 0-3 for quick alt ini selection. Comes in handy when using SNAC adapters.

The code is based on the "daemonbite" (<https://github.com/MickGyver/DaemonBite-Retro-Controllers-USB>) adapter combined with Arduino IRRemote (<https://github.com/Arduino-IRremote/Arduino-IRremote>).

Setup
---
You need a arduino pro micro or similar and a 30k-ish resistor. Prepare the arduino as described in the daemonbite readme (i.e. bridge J1, build and upload arduino code).

The code defines pin 10 as data pin for the IR Sensor. Ground and VCC is already provided to the sensor by the ironclad. Solder the resistor to pin 10 and from there splice on to the signal wire going from the sensor to the ironclad connector. The signal will feed both devices. Connect the arduino to the USB port on the ironclad board to keep it hidden inside the case.

Using the irconclad setup procedure, map the power, osd, usr and reset buttons. On the mister, configure the buttons as per the figure below (or however you like). (0-3 are hard coded by sending the respective arrow + b).

![Intended button configuration](remote.jpg?raw=true "Intended button configuration")

If you have another remote, or want to have a different mapping, change the hex constants in the code.

License
---
Both the USB gamepad support from the daemonbite (which I believe is also a fork) and the IR library are included in the repo because managing arduino dependencies suck. They are GPL3 and MIT licenced respectively.
