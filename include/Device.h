//CHIARA VIGNA

#ifndef DEVICE_H
#define DEVICE_H 

//inclusione librerie necessarie
#include "../include/Time.h"
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

class Device {
public:

    Device(std::string DEVICENAME, int ID, double power); //costruttore 

    bool isValidName() const; //verifica se il nome del dispositivo e' tra quelli disponibili
    bool isFixedCycle() const; //verifica se e' a ciclo prefissato
    bool isManual() const; //verifica se e' un dispositivo manuale
    bool isOn() const; //verifica se lo stato del device è acceso

    virtual ~Device() = default; //distruttore
    
    //funzioni getter
    std::string getName() const; //restituisce il nome
    int getID() const; //restituisce ID
    double getPower() const; //restituisce la potenza

    //funzione per settare la potenza    
    void setPower(double n){ power = n;};

    //funzione per accendere il dispositivo
    virtual void turnOn(const Time& currentTime);
    //funzione per spegnere il dispositivo
    void turnOff(const Time& currentTime); 
    
    //funzione per mostrare il consumo di un solo dispositivo
    std::string showOneDevice(const Time& currentTime) const; 

protected:
    //variabili private ma visualizzabili dalle classi derivate

    std::string DEVICENAME;
    int ID;
    double power; //in kWh
    bool state; //stato del dispositivo (acceso/spento)
    Time switchOnTime;
    Time switchOffTime;

    //lista dei dispositivi disponibili
    static const std::vector<std::string> validDeviceNames; 
    //lista dei dispositivi a ciclo prefissato
    static const std::vector<std::string> fixedCycleDeviceNames;
    //lista dei dispositivi manuali
    static const std::vector<std::string> manualDeviceNames;
};

#endif // DEVICE_H