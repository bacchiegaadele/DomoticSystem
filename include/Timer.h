//CHIARA VIGNA

#ifndef TIMER_H
#define TIMER_H


//inclusione delle librerie necessarie 
#include "../include/Time.h"
#include <string>
#include <iostream>

//classe per la gestione di timer per dispositivi manuali e a CP

class Timer {
private:
    std::string deviceName;  // Nome del dispositivo associato al timer
    Time startTime;          // Orario di inizio
    Time stopTime;           // Orario di fine (opzionale)
    bool hasStopTime;        // Indica se esiste un orario di spegnimento
                             //(solo per dispositivi manuali)
    bool startProcessed;     // Indica se il dispositivo è stato già acceso
    bool stopProcessed;      // Indica se il dispositivo è stato già spento

public:
    // Costruttore per timer con solo orario di inizio (per dispositivi CP)
    Timer(const std::string& deviceName, const Time& startTime);

    // Costruttore per timer con orario di inizio e fine (per dispositivi manuali)
    Timer(const std::string& deviceName, const Time& startTime, const Time& stopTime);

    //Funzioni getter
    const std::string& getDeviceName() const;
    Time getStartTime() const;
    Time getStopTime() const;

    // Funzione per verificare se il dispositivo deve accendersi
    bool shouldTurnOn(const Time& currentTime) const;

    // Funzione per verificare se il dispositivo deve spegnersi (solo se esiste uno stopTime)
    bool shouldTurnOff(const Time& currentTime) const;

    // Funzione per stampare il timer
    void printTimer() const;
   
    //Funzioni per tenere traccia dello stato del timer ed eveitare spegnimenti/accensioni ripetuti/e

    //la funzione controlla se isStartProcessed è false,
    //se lo e' si accende il dispositivo e
    //si imposta startProcessed = true
    bool isStartProcessed() const { return startProcessed; } 
    void setStartProcessed(bool processed) { startProcessed = processed; }

    //la funzione controlla se isStopProcessed è false,  
    //se lo è, spegne il dispositivo e imposta stopProcessed = true
    bool isStopProcessed() const { return stopProcessed; } 
    void setStopProcessed(bool processed) { stopProcessed = processed; }
};

#endif // TIMER_H