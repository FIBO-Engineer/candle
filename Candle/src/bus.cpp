#include "bus.hpp"
#include "candle_protocol.hpp"
namespace mab
{
    Bus::Bus(mab::BusType_E type)
    {
        busType = type;
    }
    Bus::~Bus()
    {
        switch(busType)
        {
            case BusType_E::USB:
            {
                delete usb;
                break;
            }
            case BusType_E::SPI:
            {
                delete spi;
                break;
            }
            case BusType_E::UART:
            {
                delete uart;
                break;
            }
        }
    }

    mab::BusType_E Bus::getType()
    {
        return busType;
    }

    char* Bus::getRxBuffer(int index)
    {
        return (char*)&rxBuffer[index];
    }   

    bool Bus::transfer(char* buffer, int commandLen, bool waitForResponse, int timeout, int responseLen)
    {
        switch(busType)
        {
            case BusType_E::USB:
            {
                return usb->transmit(buffer,commandLen,waitForResponse,timeout);
                break;
            }
            case BusType_E::SPI:
            {
                if(buffer[0] == BUS_FRAME_UPDATE)
                    return spi->transmitReceive(buffer,commandLen,responseLen);
                else
                    return spi->transmit(buffer,commandLen,waitForResponse,timeout,responseLen);
                break;
            }
            case BusType_E::UART:
            {
                return uart->transmit(buffer,commandLen,waitForResponse,timeout);
                break;
            }
        }
        return false;
    }

    bool Bus::receive(int timeoutMs)
    {
        switch(busType)
        {
            case BusType_E::USB:
            {
                return usb->receive(timeoutMs);
                break;
            }
            case BusType_E::UART:
            {
                return uart->receive(timeoutMs);
                break;
            }
            default:
                break;
        }
        return false;
    }

    int Bus::getBytesReceived()
    {
        switch(busType)
        {
            case BusType_E::USB:
            {
                return usb->getBytesReceived();
                break;
            }
            case BusType_E::SPI:
            {
                return spi->getBytesReceived();
                break;
            }
            case BusType_E::UART:
            {
                return uart->getBytesReceived();
                break;
            }
        }
        return 0;
    }
}
