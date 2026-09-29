//CHIARA VIGNA

#ifndef DEVICEMANUAL_H
#define DEVICEMANUAL_H

//inclusione delle librerie necessarie
#include "Device.h"
#include "Time.h"
#include <stdexcept>

class DeviceManual : public Device { //classe derivata di Device
public:
    //costruttore
    DeviceManual(const std::string& DEVICENAME, int ID, double power);

    //funzione per calcolare il consumo
    double calculateDeviceConsumption() const;
    
    //funzioni get e set
    void setTimer(Time start, Time stop);
    std::string getType() const; //restituisce il tipo di dispositivo
};

#endif // DEVICEMANUAL_H