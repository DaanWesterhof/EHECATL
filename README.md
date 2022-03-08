# EHECATL
A repository for my drone project. I will be designing and building a drone including the flight controller and remote controller.

## Software Design
For the software i wanted to make it easy to add new components/modules to the drone and controller.
That's why i made the communication class.

The communication module can receive messages from nrf and internal modules. It checks the command/identifier and calls callback functions that were
 added to the communication system for the command/identifier received. This way it is easy to have multiple modules react to messages or multiple modules to control another module.
 
## PCB Design
For this project i will be designing 2 PCb's. One FlightController pcb and one RemoteController PCB.

The flight controller will have the following components:
- stm32f401ccu6
- mpu6050
- nrf24l01+ + power amplifier
- motor drivers(to be determined)
- 12v to 5v and 3v regulater  