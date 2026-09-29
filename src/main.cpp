//ELENA PAVANELLO

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include "../include/Device.h"
#include "../include/DeviceManual.h"
#include "../include/DeviceFixedCycle.h"
#include "../include/Timer.h"
#include "../include/Time.h"
#include "../include/LoggerTime.h"
#include "../include/Network.h"

//metodo per stampare tutti i possibili comandi
void printHelp() {
    std::cout << "Possibili comandi:\n";
    std::cout << "- set <device_name> on/off\n";
    std::cout << "- set <device_name> <start_time> [stop_time]\n";
    std::cout << "- rm <device_name>\n";
    std::cout << "- show [<device_name>]\n";
    std::cout << "- set time <time>\n";
    std::cout << "- reset time/timers/all\n";
    std::cout << "- exit\n\n";
}

//metodo per controllare che il device esista nella rete
bool validateDeviceName(const std::string& deviceName) {
    Device tempDevice = Device(deviceName, 0, 0.0);
    if(tempDevice.isValidName())
      return true;
    else
      return false;
}

//metodo per controllare che il device sia manuale
bool validateDeviceManual(const std::string& deviceName) {
    Device tempDevice = Device(deviceName, 0, 0.0);
    if(tempDevice.isManual())
      return true;
    else
      return false;
}

//metodo per controllare che il device sia a ciclo prefissato
bool validateDeviceFixed(const std::string& deviceName) {
    Device tempDevice = Device(deviceName, 0, 0.0);
    if(tempDevice.isFixedCycle())
      return true;
    else
      return false;
}

//metodo per controllare che il tempo sia corretto e che superi l'orario corrente
bool validateTime(int hh, int mm, Network& network) {
    if (hh<24 && mm<60){
      Time tm=Time(hh,mm);
      if (tm > network.getCurrentTime())
         return true;
      else
         return false;
    }
    else
      return false;
}

//metodo per controllare che le azioni siano corrette
bool validateCommand(const std::string& command) {
    std::vector<std::string> validPrefixes = {"set", "show", "rm ", "reset", "help", "exit"};
    for (const auto& prefix : validPrefixes) {
        if (command.rfind(prefix, 0) == 0)//rfind cerca prefix nell'inizio di command 
            return true; //se prefix viene trovato allora true
    }
    return false;
}

// Funzione per gestire i comandi dell'utente del sistema domotico
void Command(Network& network, LoggerTime& logger, const std::string& command) {

    // Logga il comando passato dall'utente
    logger.logCommand(command);
    
    std::istringstream iss(command);
    std::string cmd, deviceName;
    iss >> cmd;

    if (cmd == "set") {
        iss >> deviceName;
        if (deviceName == "time") {
            int hour, minute;
            char colon;
            iss >> hour >> colon >> minute;
            if (validateTime(hour, minute, network)) {
                Time newTime(hour,minute);
                network.setTime(Time(hour, minute));
                // Stampa tutti i log dalla mezzanotte fino all'orario corrente
                logger.printLogsUpToCurrentTime(network, newTime);

                logger.write("L'orario attuale e' " + newTime.printTime(), network);
                // Aggiorna l'orario dell'ultimo log stampato
                logger.updateLastPrintedTime(newTime);

            }
            else
                logger.write("Comando non riconosciuto.", Time(hour,minute));
        } 
        else if (validateDeviceName(deviceName)) {
            std::string action;
            iss >> action;

            // Prima controlla se è un comando 'on' o 'off'
            if (action == "on") {
                network.createTimer(deviceName, network.getCurrentTime());
                if (deviceName == "impianto_fotovoltaico")
                    network.add_powerProduced(network.getPowerProduced());
                else
                    network.add_powerConsumed(network.getPowerConsumed());
                    
                logger.write("Acceso il dispositivo '" + deviceName + "' alle " + network.getCurrentTime().printTime(), network);

                network.checkPower(); // Controllo la potenza e spegnimento eventuali dispositivi in eccesso
            }
            else if (action == "off") {
                network.removeTimer(deviceName);
                if (deviceName == "impianto_fotovoltaico")
                    network.add_powerProduced(-network.getPowerProduced());
                else
                    network.add_powerConsumed(-network.getPowerConsumed());
                    logger.write("Spento il dispositivo '" + deviceName + "'", network);
                    network.checkPower();
            } 
            // Se non è 'on/off', allora prova a interpretare come timer
            else {
                int startHour, startMinute, stopMinute, stopHour;
                char col1, col2;
                
                // Ripristina lo stream per rileggere l'orario
                iss.clear();
                iss.str(command);
                iss >> cmd >> deviceName >> startHour >> col1 >> startMinute;
                
                if (iss && col1 == ':') {
                    if (validateDeviceFixed(deviceName) == true) {
                        // Verifica che non ci siano ulteriori dati nello stream per dispositivi a ciclo prefissato
                        std::string extra;
                        if (iss >> extra) {
                            // Se ci sono altri dati, segnala un errore
                            logger.write("Errore: il dispositivo a ciclo prefissato '" + deviceName + "' deve avere solo un orario di inizio.", network);
                        } else {
                            // Se non ci sono altri dati, imposta correttamente il timer
                            network.createTimer(deviceName, Time(startHour, startMinute));
                            logger.write("Impostato un timer per il dispositivo a ciclo prefissato '" + deviceName + "' dalle " +
                                        Time(startHour, startMinute).printTime() + ".", network);
                            network.checkPower();
                        }
                    } 
                    else if (validateDeviceManual(deviceName) == true) {
                        // Se il dispositivo è manuale, accetta sia orario di inizio che fine
                        if (iss >> stopHour >> col2 >> stopMinute && col2 == ':') {
                            // Creazione del timer con inizio e fine
                            network.createTimer(deviceName, Time(startHour, startMinute), Time(stopHour, stopMinute));
                            logger.write("Impostato un timer per il dispositivo manuale '" + deviceName + "' dalle " + 
                                        Time(startHour, startMinute).printTime() + " alle " + 
                                        Time(stopHour, stopMinute).printTime() + ".", network);
                            network.checkPower();
                        } 
                        else {
                            logger.write("Errore: orario di fine mancante per il dispositivo manuale '" + deviceName + "'.", network);
                        }
                    }
                    else {
                        logger.write("Comando non riconosciuto.", network);
                    }
                } 
                else {
                    logger.write("Comando non riconosciuto.", network);
                }
            }

        } 
        else {
            logger.write("Comando non riconosciuto.", network);
        }
}
 
    else if (cmd == "rm") {
        iss >> deviceName;
        if(validateDeviceName(deviceName)){
           network.removeTimer(deviceName);
           logger.write("Rimosso il timer dal dispositivo '" + deviceName + "'", network);
        }
        else
               logger.write("Comando non riconosciuto.", network);
    } 
    else if (cmd == "show") {
        if(iss >> deviceName){
            if(validateDeviceName(deviceName))
               logger.write(network.showOneDevice(deviceName),network);
            else
               logger.write("Comando non riconosciuto.", network);  
        }
        else
            logger.write(network.showAllPower(),network);
    }
    else if (cmd == "reset") {
        std::string action;
        iss >> action;
        if (action == "time"){
            network.resetTime();
            logger.write("L'orario attuale e' " + network.getCurrentTime().printTime(), network);
        }
        else if (action == "timers") {
            network.removeAllTimer();
            logger.write("Rimossi tutti i timer", network);
        } 
        else if (action == "all") {
            network.resetAll();
            logger.write("Resettato il sistema", network);;
        }
        else{
            logger.write("Comando non riconosciuto.", network);
        }    
    } 
    else if (cmd == "exit") {
        std::cout << "Chiusura del sistema domotico.\n";
        exit(0);
    } 
    else {
        logger.write("Comando non riconosciuto.", network);
    }
}


int main(){
    Network net1 = Network(); //rappresenta la rete con 3.5 come massimo di potenza
    
    LoggerTime log1 = LoggerTime("log.txt"); //per scrivere i comandi in un file log.txt
    printHelp();
    std::cout<<"Inserire i comandi: \n";
    std::string command;
    while(true){
      std::getline(std::cin, command);
      Command(net1, log1, command);
    }
    return 0;
}