# Simple Wireless Power

WIP!

## Introduction 
This project is a simple two-board wireless power system that can transmit up to 150W at high efficiency. Originally it was designed as a small part of a subsea resident autonomous underwater vehicle senior design project. The goal was to have a contactless method to charge the vehicles battery in an underwater docking station, versus a contact method like a wetmate connector. This reduces the chance for failure in a harsh ocean environment. Included in this repository are schematics, PCB design files, LTspice simulations, and embedded code.

## Design Topology
At the start of my wireless power investigation, I initially decided to create a type of "coreless transformer" to keep things simple. This type of design would transfer power by simply placing two identical coils as close as possible to each other. The closer the coils, the higher their mutal inductance and coupling coefficient. The coupling coefficient directly equals the maximum achievable efficiency; k=0.7 would mean 70% efficiency for example. 

However, since I was putting a lot of work into research and simulation anyways, I decided to try and create a resonant system. Using capactiors, resonant circuits can be created with the inductive transmitter and reciever coils. This is the de facto inductive wireless power transfer method used in industry, and it turned out to be simpler than I thought. I ended up choosing a topology called "series-series" resonant inductive wireless power transfer, which means that the compensating capacitors are placed in series with both the transmit and recieve coils. Other topologys exist such as "series-parallel" or "parallel-parallel" which place the capacitors in parallel with the coils as well. I attempted to simulate each topology, and concluded that the series-series design is not as senstive to frequency changes and therefore more easily tuned by adjusting the control frequency.


## Hardware
The system consists of a transmitter (TX) PCB and a reciever (RX) PCB. Both are 4 layer 1oz copper boards that are 100x45mm. The designs were completed using KiCAD EDA (http://kicad-pcb.org/), a free and open source PCB design software that I highly recommend. 

### TX Board

The TX board has an embedded SAMD21 microntroller which generates the nessecary PWM signal for the gate drivers. It also has a CAN controller and transciever connected so that it could potentially be commanded on/off and relay power transfer telemetry to a remote control computer.

### RX Board

### Key Components

## Software
The current software developed for this project is the embedded code on the TX board's SAMD21 microcontroller. Code was developed using the Arduino IDE for simplicity. A .bin file was generated and then uploaded to the microcontroller using a J-Link EDU programmer. 

## Simulation

## Testing Results

### To Be Continued...
More testing will be completed soon! An efficiency vs coil gap size graph will be completed, as well as efficiency for different delievered powers. Safety features will also be verified such as overcurrent protection on the TX board and overvoltage protection on the RX board.
