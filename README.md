# Simple Wireless Power System

## Introduction 
This project is a simple two-board wireless power system that can transmit up to 150W at high efficiency. Originally it was designed as a small part of a subsea resident autonomous underwater vehicle senior design project. The goal was to have a contactless method to charge the vehicles battery in an underwater docking station, versus a contact method like a wetmate connector. This reduces the chance for failure in a harsh ocean environment. Included in this repository are schematics, PCB design files, LTspice simulations, and embedded code.

![TX Board Powered](https://user-images.githubusercontent.com/32495259/119278762-008b0780-bc02-11eb-916a-1963d50e4152.jpg)

## Design Topology
At the start of my wireless power investigation, it was initially decided to create a type of "coreless transformer" to keep things simple. This type of design would transfer power by simply placing two identical coils as close as possible to each other. The closer the coils, the higher their mutal inductance and coupling coefficient. The coupling coefficient directly equals the maximum achievable efficiency; k=0.7 would mean 70% efficiency for example. 

![coreless_transformer](https://user-images.githubusercontent.com/32495259/119279035-b4d95d80-bc03-11eb-8f76-d7799d2d53da.PNG#center)
> Coreless transformer equivalent circuit.

However, since a lot of work was already being placed into research and simulation anyways, it was decided to try and create a resonant system. Using capactiors, resonant circuits can be created with the inductive transmitter and reciever coils, increasing efficiency accross the wireless link. This is the de facto inductive wireless power transfer method used in industry, and it turned out to be simpler than thought. The topology chosen is called "series-series" resonant inductive wireless power transfer, which means that the compensating capacitors are placed in series with both the transmit and recieve coils. Other topologys exist such as "series-parallel" or "parallel-parallel" which place the capacitors in parallel with the coils as well. Each topology was simulated, and it was concluded that the series-series design is not as senstive to frequency changes and therefore more easily tuned by adjusting the control frequency.

![series_series](https://user-images.githubusercontent.com/32495259/119279042-b86ce480-bc03-11eb-8be5-fae0d67579dd.PNG)
> Series-series topology circuit.

## Hardware
The system consists of a transmitter (TX) PCB and a reciever (RX) PCB. Both are 4 layer 1oz copper boards that are 100x45mm. The designs were completed using KiCAD EDA (http://kicad-pcb.org/), a free and open source PCB design software that is highly recommended. 

### TX Board
The transmitter board's primary purpose is to take a DC input voltage and pass it through a full bridge inverter to create a high frequency sinusoidal voltage. This sinusoidal voltage passes through the compensating capacitors and transmitter coil, and creates an alternating magnetic field that couples to the reciever coil. The H-Bridge is designed around an operating point of 24VDC input, 100kHz switching frequency, and up to 150W of transfer power. The main limiter for power transfer is the rated voltage and current of the coils and compensating capacitors. The MOSFETs of the H-Bridge are over-speced to minimize losses. Short circuit protection circuitry is included on the TX board, since if the TX coil is open-loaded during operation it will appear as a short to the input power supply.

![TX Board](https://user-images.githubusercontent.com/32495259/119278773-0bde3300-bc02-11eb-87d0-157e233e9202.PNG)

The TX board also has an embedded SAMD21 microntroller which generates the nessecary PWM signal for the gate drivers. Further, the board includes a CAN controller and transceiver so that it could potentially be commanded on/off and relay power transfer telemetry to a remote control computer via CANBus.

![TX Schematic](https://user-images.githubusercontent.com/32495259/119279338-a4c27d80-bc05-11eb-9b8b-24d108d0815e.PNG)

### RX Board
The RX board is much simpler than the TX board. It has no microcontroller, and uses passive schottky diodes in a full bridge rectifier configuration to convert the recieved AC voltage back to a usable DC voltage. An simple overvoltage protection scheme is incorporated in the RX board to prevent an overvoltage event downstream. A schmitt trigger circuit watches the output voltage, and if it is too high, connects the output to a bank of bleeder resistors. The extra load on the output subsequently draws down the voltage before it reaches a harmful level. 
                                                                                   
![RX Board](https://user-images.githubusercontent.com/32495259/119278781-13054100-bc02-11eb-832a-217c8b096a45.PNG)

![RX Schematic](https://user-images.githubusercontent.com/32495259/119279342-a9873180-bc05-11eb-946f-feee2292f1fa.PNG)

### Key Components

#### Coils
The transmitter and reciever coils are identical. They were sourced on Digi-Key from Wurth Electronics, PN 732-9670-ND. They have an inductance of 5.8uH, and a very low series resistance, allowing for high current rating. I think these are probably the highest current rated coils you can find on Digikey when I selected them! As well, Wurth was kind enough to provide ample documentation for the coils. A key piece of data was a graph of their Quality factor (Q) versus frequency. The higher the quality factor, the better the efficiency. Therefore, since the Q peaked at around 100-101kHz, that was the selected operating frequency for the entire system.

![Coils](https://user-images.githubusercontent.com/32495259/119278785-17c9f500-bc02-11eb-8290-07031c8aff2d.png)
> Coils encased and epoxyed in a custom mount for underwater use in the original AUV application. 

#### Series Capacitors
Four 100nF capacitors were placed in parallel to achieve the desired ~380-400nF series capacitance. Large 2220 package ceramic capacitors were chosen for their desirable low series resistance. The final parts were chosen from Murata Electronics and sourced on Digikey, PN 490-16696-1-ND. These capacitors were compared against others, again looking at their quality factor over frequency. These caps had the best that could be found at the time, but if someone knows of a better choice, let me know! 

#### MOSFETs
The MOSFETs chosen are low-loss N-Channel devices. The final parts chosen are from Infineon and sourced on Digi-Key, PN 448-IAUC70N08S5N074ATMA1CT-ND. These FETs were selected for the lowest possible loss, taking into account both resistive and switching losses. 

#### Diodes
The diodes for the full-bridge rectifier were chosen again for as low of loss as possible. Really, the low voltage and high current environment is probably better suited for active rectification, but to keep things simple passive rectification was chosen. The best diodes for this are schottky due to their low forward voltage. The final parts chosen are from ON Semi and sourced from Digi-Key, PN FSV12100VCT-ND. A few different diode models were selected and simulated in the final circuit, and this model ended up producing the lowest loss. 

#### Micro Controller
The chosen microcontroller is the SAMD21G18A, PN ATSAMD21G18A-MF-ND. This is the same microcontroller that is on the Arduino Zero dev board, and our senior design team already knew how to easily program and flash it. Perhaps someone could modify this design and retrofit a more popular microcontroller, like an STM32.

## Software
The current software developed for this project is the embedded code on the TX board's SAMD21 microcontroller. Code was developed using the Arduino IDE for simplicity. A .bin file was generated and then uploaded to the microcontroller using a J-Link EDU programmer. The code is about as simple as it gets, just generating a 50% duty cycle, 100kHz PWM signal. No feedback is incorporated since it is assumed that there is no communication between TX and RX boards in the original underwater application. 

## Simulation
The entire end-to-end wireless power system was simulated in LTspice. The simulation aided in component selection, and gave an initial estimate of efficiency greater than 90%. It's hard to say how accurate the simulation is, since it's not clear what coupling factor, k, will be present in a real world system. Any tips on WPT system simulation in LTspice would be greatly appreciated! 

![Sim Schematic](https://user-images.githubusercontent.com/32495259/119278790-1c8ea900-bc02-11eb-931d-c2eacffef1e0.PNG)

## Testing Results
The system was tested with surpising results. At a gap distance of about 1-2mm, as close as we could get, the system was able to transfer power in excess of 100W at a high efficiency of 96.6%. The efficiency could even be higher if the frequency is tuned, as the test only looked at a static frequency of 100kHz. The coils were also submerged in saltwater to check efficiency versus the in-air test. There was no measurable difference in water, but its suspected that as the coil gap increases there should be a noticeable difference of 2-3% due to field losses in the slightly conductive saltwater.

![Lab Test Setup](https://user-images.githubusercontent.com/32495259/119278795-20bac680-bc02-11eb-9807-420d85a31efa.jpg)

![Water Setup](https://user-images.githubusercontent.com/32495259/119278801-257f7a80-bc02-11eb-9ca2-66115d665621.PNG)

Another interesting outcome was shown in the thermal images during testing. The hottest components in the system were the series capacitors, reaching upwards of 70C degrees after 5 mins of operation. No heatsink or airflow meant that these are probably worst case numbers, but still perhaps the capacitors could be underspec'd for the application. Any ideas on how to decrease their losses would be helpful! 

![capacitor_thermals](https://user-images.githubusercontent.com/32495259/119279590-426a7c80-bc07-11eb-95e8-bb5e12374c14.PNG)


### To Be Continued...
More testing will be completed soon! An efficiency vs coil gap size graph will be completed, as well as efficiency for different delievered powers, and perhaps some frequency tuning. Safety features will also be verified such as overcurrent protection on the TX board and overvoltage protection on the RX board.

## Discussion
This was a fun project and a small part of a much larger senior design project. Always open for discussion or any tips on how this could have been done better!
