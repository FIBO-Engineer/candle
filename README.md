### MD80 - USB-CAN driver
This driver will make it simple to implement MD80 into existing projects. The driver will let the user change MD80's 
configuration as well as control the MD80 with most/all of its functionality.

## Latency and bandwith
Communication frequency is currently fixed at 100Hz. This is true for both transmitting (PC->CANdle) and receiving 
(CANdle->PC). The CANdle will haandle low level FDCAN communications with the MD80s, as rate sufficient to update 
all states (and send all commands), faster that the update rate with PC, regardless of selected FDCAN baudrate.

## Building
Make sure you are in md80_driver main directory. 
```
mkdir build
cd build 
cmake ..
make
```
This will trigger the build of the library and examples. The library will be placed in libs/ directory, 
include files in include/ directory, and examples in examples/directory

## Examples
When building a set of examples will be built. The example executables are placed in 
```
build/Candle/examples/
```
while the source files are in
```
Candle/examples/
```
Examples feature a simple use cases for most of the methods supplied with the library as well as comments guiding user 
around the code and implementation.