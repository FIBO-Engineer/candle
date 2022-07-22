#include "candle.hpp"
#include <iostream>
#include <unistd.h>
#include <string.h>

#define FIRST_ID    300
#define SECOND_ID   350

int main()
{
    /* change the priority */
    struct sched_param sp;
    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = 99;
    sched_setscheduler(0, SCHED_FIFO, &sp);

    //Create CANdle object and set FDCAN baudrate to 8Mbps
    mab::Candle candle(mab::CAN_BAUD_8M, true, mab::CANdleFastMode_E::FAST2, true, mab::BusType_E::SPI);

    candle.addMd80(FIRST_ID);
    candle.addMd80(SECOND_ID);

    //Now we shall loop over all found drives to change control mode and enable them one by one
    for(auto &md : candle.md80s)
    {
        candle.controlMd80SetEncoderZero(md);       // Reset encoder at current position
        candle.controlMd80Mode(md, mab::Md80Mode_E::IMPEDANCE);     //Set mode to impedance control
        candle.controlMd80Enable(md, true);     //Enable the drive
    }

    candle.md80s[0].setImpedanceControllerParams(1.0,0.02);
    candle.md80s[1].setImpedanceControllerParams(4.0,0.02);

    //Begin update loop (it starts in the background)
    candle.begin();

    float pos_first = 0;
    float pos_second = 0;

    float vel_first = 0;
    float vel_second = 0;

    float gearbox = 0.2;

    while(1)
    {

        pos_first = pos_first * 0.9f + candle.md80s[0].getPosition()*0.1f;
        pos_second = pos_second * 0.9f + candle.md80s[1].getPosition()*0.1f;

        vel_first = vel_first * 0.99f + candle.md80s[0].getVelocity()*0.01f;
        vel_second = vel_second * 0.99f + candle.md80s[1].getVelocity()*0.01f;

        float pos_avg = (pos_first + pos_second)/2.0f;
        float vel_avg = (vel_first + vel_second)/2.0f;

        candle.md80s[1].setTargetPosition(gearbox*pos_first);
        candle.md80s[0].setTargetPosition((1.0f/gearbox)*pos_second);

        candle.md80s[1].setTargetVelocity(gearbox*vel_first);
        candle.md80s[0].setTargetVelocity((1.0f/gearbox)*vel_second);

        usleep(10);
    }

    candle.end();

    return EXIT_SUCCESS;
}