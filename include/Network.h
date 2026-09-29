//ADELE BACCHIEGA

//include guards
#ifndef NETWORK_H
#define NETWORK_H

//inclusione librerie e header necessari
#include "Device.h"
#include "DeviceFixedCycle.h"
#include "DeviceManual.h"
#include "Time.h"
#include "Timer.h"
#include "LoggerTime.h"
#include <vector>
#include <string>

// Forward declaration di LoggerTime
class LoggerTime;

class Network{
private:
    //variabili private
    std::vector<Device*> net;
    std::vector<Timer> netTimers;
    int nTimer = 0;
    const double MAX_POWER = 3.5;
    double powerProduced = MAX_POWER;
    double powerConsumed = 0;
    Time currentTime;
    Time duration = Time(0,0);
    Time lastCheckedTime;  // Tiene traccia dell'ultimo orario controllato

    //funzione membro per creare il vettore con i device
    void create_net();

public:
    //costruttore
    Network();
    
    //distruttore
    ~Network() = default;
    
    //funzioni get
    double getPowerProduced() const { return powerProduced; }
    double getPowerConsumed() const { return powerConsumed; }
    Time getCurrentTime() const { return currentTime; }
    
    //funzione membro per aggiungere/togliere potenza
    void add_powerProduced(double pw);
    void add_powerConsumed(double pw);
    
    //funzione membro per settare il nuovo orario corrente
    void setTime(Time newTime);
    
    //funzione membro per resettare il sistema all'orario 00:00
    void resetTime();
    
    //funzione membro per creare un nuovo timer
    void createTimer(std::string DEVICENAME, Time start);
    void createTimer(std::string DEVICENAME, Time start, Time stop);
    
    //funzione membro per rimuovere il timer da un preciso dispositivo
    void removeTimer(std::string DEVICENAME);
    
    //funzione membro per rimuovere tutti i timer
    void removeAllTimer();
       
    //funzione membro per resettare il sistema (sia orario che timer)
    void resetAll();
    
    //funzione membro per mostrare il consumo di un determinato device
    std::string showOneDevice(const std::string DEVICENAME) const;
    
    //funzione membro per mostrare la lista di tutti dispositivi con il loro
    //consumo dalle 00:00 a currentTime
    std::string showAllPower() const;

    //funzione membro per modificare se:
    //1. la potenza consumata superi quella prodotta
    //2. spegnere i dispositivi in ordine inverso rispetto all'accensione per riequilibrare la potenza
    //3. evitare di spegnere dispositivi essenziali (per es il frigorifero)
    void checkPower();

    //funzione membro sper verificare quando spegnere e accendere un dispositivo a
    //seconda del timer impostato
    void checkTimers(LoggerTime& logger, const Time& lastTime, const Time& newTime);
};

#endif
