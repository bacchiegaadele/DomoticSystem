//CHIARA VIGNA

//inclusione librerie necessarie
#include "../include/DeviceFixedCycle.h"
#include "../include/Time.h"
#include "../include/Device.h"
#include <iostream>

// Costruttore
DeviceFixedCycle::DeviceFixedCycle(std::string DEVICENAME, int ID, double power, Time duration) 
                : Device(DEVICENAME, ID, power) {
    this->duration = duration;
}

// Imposta il timer per il dispositivo CP
void DeviceFixedCycle::setTimer(Time start) {
    switchOnTime = start;
    switchOffTime = start;
    switchOffTime.addTime(duration.getHours(), duration.getMinutes()); //si somma il tempo di durata al tempo di accensione
    //std::cout << "Impostato un timer per il dispositivo " + getName() + " dalle " + switchOnTime.printTime() + " alle " + switchOffTime.printTime() + ".\n";
}

// Calcola il consumo energetico del dispositivo
double DeviceFixedCycle::calculateDeviceConsumption() const {
    // Converti la durata in ore decimali
    double durataOre = this->duration.getHours() + this->duration.getMinutes() / 60.0;

    // Calcola e restituisce il consumo energetico
    return power * durataOre;
}

// Restituisce il tipo di dispositivo
std::string DeviceFixedCycle::getType() const {
    return "Ciclo Prefissato";
}

// Accende il dispositivo e imposta lo spegnimento automatico
void DeviceFixedCycle::turnOn(const Time& currentTime) {
    state = true;
    switchOnTime = currentTime;
    switchOffTime = currentTime;
    switchOffTime.addTime(duration.getHours(), duration.getMinutes()); //si somma all'orario di accnsione il corrispondente
                                                                    //orario di spegnimento automatico

    //std::cout << currentTime.printTime() + "  Il dispositivo " + getName() + " è acceso e si spegnerà alle " + switchOffTime.printTime() + ".\n";
}

//Funzione per restituire la durata del ciclo prefissato
Time DeviceFixedCycle::getDuration() const {
    return this->duration;
}
