//CHIARA VIGNA

#ifndef DEVICEFIXEDCYCLE_H
#define DEVICEFIXEDCYCLE_H

//inclusione delle librerie necessarie
#include "../include/Device.h"
#include "../include/Time.h"

class DeviceFixedCycle : public Device { //classe derivata di Device
    Time duration; //orario in formato di Time

public:
    //costruttore
    DeviceFixedCycle(std::string DEVICENAME, int ID, double power, Time duration);
   
    //funzione per calcolare il consumo
    double calculateDeviceConsumption() const;
    
    //funzioni get e set
    std::string getType() const;
    void setTimer(Time start);
    
    //funzione per accendere il dispositivo
    void turnOn(const Time& currentTime) override; //override della funzione Device turnOn

    // funzione per ottenere la durata del ciclo prefissato
    Time getDuration() const;


};

#endif //DEVICEFIXEDCYCLE_H