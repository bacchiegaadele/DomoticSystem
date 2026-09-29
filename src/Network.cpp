//ADELE BACCHIEGA

//inclusione librerie e header necessari
#include "../include/Network.h"
#include "../include/Time.h"
#include "../include/Timer.h"
#include "../include/Device.h"
#include "../include/DeviceFixedCycle.h"
#include "../include/DeviceManual.h"
#include "../include/LoggerTime.h"
#include <vector>

//costruttore
Network::Network(){
   powerProduced = MAX_POWER;
   lastCheckedTime = Time(0, 0);
   currentTime = Time(0,0);
   create_net();
}

//funzione membro per creare la rete domotica
void Network::create_net(){
   //creazione dei device e inserimento nel vector net

   net.push_back(new DeviceManual("impianto_fotovoltaico", 1, 1.5));

   duration.changeTime(1,50);
   net.push_back(new DeviceFixedCycle("lavatrice", 2, 2, duration));
   
   duration.changeTime(3,15);
   net.push_back(new DeviceFixedCycle("lavastoviglie", 3, 1.5, duration));
   
   net.push_back(new DeviceManual("pompa_di_calore+termostato", 4, 2));
   
   duration.changeTime(0,1);
   net.push_back(new DeviceFixedCycle("tapparelle_elettriche", 5, 0.3, duration));

   net.push_back(new DeviceManual("scaldabagno", 6, 1));

   net.push_back(new DeviceManual("frigorifero", 7, 0.4));

   duration.changeTime(0,2);
   net.push_back(new DeviceFixedCycle("forno_a_microonde", 8, 0.8, duration));

   duration.changeTime(1,0);
   net.push_back(new DeviceFixedCycle("asciugatrice", 9, 0.5, duration));
   
   duration.changeTime(1,0);
   net.push_back(new DeviceFixedCycle("televisore", 10, 0.2, duration));
}

//funzione membro per aggiungere/togliere potenza consumata/prodotta
void Network::add_powerProduced(double pw){
    powerProduced += pw;
}
void Network::add_powerConsumed(double pw){
    powerConsumed += pw;
}

//funzione membro per settare il nuovo orario corrente
void Network::setTime(Time newTime){
   int h = newTime.getHours();
   int m = newTime.getMinutes();
   currentTime.changeTime(h,m);
}
    
//funzione membro per resettare il sistema all'orario 00:00
void Network::resetTime(){
   currentTime.changeTime(0,0);
}

//funzione membro per creare un nuovo timer nel caso di dispositivi a CP
void Network::createTimer(std::string DEVICENAME, Time start) {
    for (auto& device : net) {
        if (device->getName() == DEVICENAME && device->isFixedCycle() == true) {
            Timer nTimers = Timer(DEVICENAME, start);
            netTimers.push_back(nTimers);
            nTimer++;
        }
    }
}

//funzione membro per creare un nuovo timer nel caso di dispositivi M
void Network::createTimer(std::string DEVICENAME, Time start, Time stop) {
    for (auto& device : net) {
        if (device->getName() == DEVICENAME && device->isManual() == true) {
            Timer nTimers = Timer(DEVICENAME, start, stop);
            netTimers.push_back(nTimers);
            nTimer++;
        }
    }
}
    
//funzione membro per rimuovere il timer da un preciso dispositivo
void Network::removeTimer(std::string DEVICENAME){
   for (int i=0; i<netTimers.size(); i++){
      if (netTimers[i].getDeviceName() == DEVICENAME){
         if(netTimers[i].getStartTime() < currentTime){
            std::cout << "Impossibile rimuover il timer dal dispositivo " << DEVICENAME << " perche' gia' terminato\n";
         }
         else{
            netTimers.erase(netTimers.begin() + i);
            //std::cout << "Rimosso il timer dal dispositivo " << DEVICENAME << "\n";
         }
      }   
   }
}
    
//funzione membro per rimuovere tutti i timer
void Network::removeAllTimer(){
   for (int i=0; i<netTimers.size(); i++){
      if(netTimers[i].getStartTime() < currentTime){
         std::cout << "Impossibile rimuover il timer dal dispositivo " << netTimers[i].getDeviceName() << " perche' gia' terminato\n";
      }
      else{
         netTimers.erase(netTimers.begin() + i);
         //std::cout << "Rimosso il timer dal dispositivo " << netTimers[i].getDeviceName() <<"\n";
      }   
   }
}
    
//funzione membro per resettare il sistema
void Network::resetAll(){
   resetTime();
   removeAllTimer();
   powerProduced = MAX_POWER;
   powerConsumed = 0;
   for (int i = 0; i < net.size(); i++){
      net[i]->setPower(0.0);
   }
   for (auto& timer : netTimers) {
        timer.setStartProcessed(false);
        timer.setStopProcessed(false);
    } 
}

//funzione membro per vedere il consumo di un dispositivo
std::string Network::showOneDevice(const std::string deviceName) const{
   std::string str;
   for(const auto& device : net){
      if(device->getName()==deviceName){
         str = device->showOneDevice(currentTime);
         return str;
      }
   }
   return str = "Errore: Il dispositivo '" + deviceName + "' non è stato trovato";
}

//funzione membro per lista di tutti dispositivi con power da 00:00 a currentTime
std::string Network::showAllPower() const {
   std::string str = "Attualmente il sistema ha prodotto " + std::to_string(powerProduced) + " kWh e ha consumato " + std::to_string(powerConsumed) + " kWh. Nello specifico:\n";
   for (int i = 0; i < net.size(); i++){
      str = str + "- " + net[i]->showOneDevice(currentTime);
   }
   return str;
}

//funzione che verifica se la potenza consumata supera quella disponibile e  
//e spegne i dispositivi necessario per avere un bilancio
void Network::checkPower() {
   double totalCurrPower = powerProduced + 3.5; // Somma della produzione fotovoltaica 
                                                //e del limite massimo di 3,5 kW
   double totalPowerConsumed = 0.0;
   //calcola la potenza totale attualmente consumata
   for (const auto& device : net) {
      if (device->isOn()) {
         //se il dispositivo è acceso, aggiungi la sua potenza al consumo totale
         if(device->getName()=="impianto_fotovoltaico")
            totalCurrPower += device->getPower();
         else
            totalPowerConsumed += device->getPower();
      }
   }
   //se la potenza consumata supera quella disponibile, inizia a spegnere dispositivi
   if (totalPowerConsumed > totalCurrPower) {
      std::cout << "Consumo eccessivo! Inizio spegnimento dispositivi\n";
      // Spegni i dispositivi in ordine inverso rispetto all'accensione
      for (auto i = net.rbegin(); i != net.rend(); ++i) {
         if ((*i)->getName() == "frigorifero" || (*i)->getName() == "scaldabagno") { 
            //i dispositivi 'frigorifero' e 'scaldabagno' continuano ad essere accesi
            continue;  
         }
         if ((*i)->isOn()) {
            (*i)->turnOff(currentTime);
            totalPowerConsumed -= (*i)->getPower();
            std::cout << "Spento il dispositivo '" << (*i)->getName() << "'.\n";
         }
         //interrompi il ciclo se la potenza consumata è ora <= a quella disponibile
         if (totalPowerConsumed <= totalCurrPower)
            break;
      }
   }
}

//funzione membro per verificare quando spegnere e accendere un dispositivo a
//seconda del timer impostato
void Network::checkTimers(LoggerTime& logger, const Time& startTime, const Time& endTime){
    for (auto& timer : netTimers) {
        for (auto& device : net) {
            if (device->getName() == timer.getDeviceName()) {
                //se il dispositivo è manuale
                if (device->isManual()==true) {
                    //accendi il dispositivo se necessario
                    if (startTime <= timer.getStartTime() && endTime >= timer.getStartTime() && !device->isOn()) {
                        device->turnOn(timer.getStartTime());
                        logger.write("Acceso il dispositivo manuale '" + device->getName() + "' alle " + timer.getStartTime().printTime(), timer.getStartTime());
                    } 
                    //spegni il dispositivo se necessario, evitando duplicati
                    if (endTime >= timer.getStopTime() && device->isOn()) {
                        device->turnOff(timer.getStopTime());
                        logger.write("Spento il dispositivo manuale '" + device->getName() + "' alle " + timer.getStopTime().printTime(), timer.getStopTime());
                    }
                    if (device->getName() == "impianto_fotovoltaico")
                        powerProduced += device->getPower();
                    else
                        powerConsumed += device->getPower();
                }
                //se il dispositivo è a ciclo prefissato
                else if(device->isFixedCycle()==true){
                    Time duration = Time(0,0);
                    if (device->getName() == "lavatrice") {
                        duration.changeTime(1, 50);
                    } else if (device->getName() == "lavastoviglie") {
                        duration.changeTime(3, 15);
                    } else if (device->getName() == "tapparelle_elettriche") {
                        duration.changeTime(0, 1);
                    } else if (device->getName() == "forno_a_microonde") {
                        duration.changeTime(0, 2);
                    } else if (device->getName() == "asciugatrice") {
                        duration.changeTime(1, 0);
                    } else {
                        // Caso "televisore"
                        duration.changeTime(1, 0);
                    }

                    Time stopTime = Time(timer.getStartTime().getHours(), timer.getStartTime().getMinutes());
                    stopTime.addTime(duration.getHours(), duration.getMinutes());

                     if (startTime <= timer.getStartTime() && endTime >= timer.getStartTime() && !device->isOn()) {
                         device->turnOn(timer.getStartTime());
                         logger.write("Acceso il dispositivo a ciclo prefissato '" + device->getName() + "' alle " + timer.getStartTime().printTime(), timer.getStartTime());
                     }
                     if (endTime >= stopTime && device->isOn()) {
                         device->turnOff(stopTime);
                         logger.write("Spento il dispositivo a ciclo prefissato '" + device->getName() + "' alle " + stopTime.printTime(), stopTime);
                     }
                     powerConsumed += device->getPower();
                }
             }
          }
      }
}