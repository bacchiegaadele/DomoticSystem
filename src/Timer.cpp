//CHIARA VIGNA

#include "../include/Timer.h"
#include "../include/Time.h"

// Costruttore per timer con solo orario di inizio (per dispositivi CP)
Timer::Timer(const std::string& deviceName, const Time& startTime)
    : deviceName(deviceName), startTime(startTime), hasStopTime(false), startProcessed(false), stopProcessed(false) {}

// Costruttore per timer con orario di inizio e fine (per dispositivi manuali)
Timer::Timer(const std::string& deviceName, const Time& startTime, const Time& stopTime)
    : deviceName(deviceName), startTime(startTime), stopTime(stopTime), hasStopTime(true), startProcessed(false), stopProcessed(false) {}

// Restituisce il nome del dispositivo
const std::string& Timer::getDeviceName() const {
    return deviceName;
}

// Restituisce l'orario di inizio del timer
Time Timer::getStartTime() const{
   return startTime;
}

//Restituisce orario di fine del Timer
Time Timer::getStopTime() const {
    return stopTime;
}

// Funzione per verificare se il dispositivo deve accendersi
bool Timer::shouldTurnOn(const Time& currentTime) const {
    return currentTime == startTime;
}

// Funzione per verificare se il dispositivo deve spegnersi (solo se esiste uno stopTime)
bool Timer::shouldTurnOff(const Time& currentTime) const {
    return hasStopTime && currentTime == stopTime;
}

// Funzione per stampare il timer
void Timer::printTimer() const {
    std::cout << "Timer per '" << deviceName << ". Inizio: " << startTime.printTime();
    if (hasStopTime) {
        std::cout << ", Fine: " << stopTime.printTime();
    }
    std::cout << "\n";
}