//ELENA PAVANELLO

#ifndef LOGGERTIME_H
#define LOGGERTIME_H

#include <string>
#include <fstream>
#include <vector>
#include "Time.h" // Include per la gestione del tempo
#include "Network.h"

// Forward declaration di Network
class Network;

class LoggerTime {
private:
    std::ofstream logFile; // File per scrivere i log
    bool showToConsole;    // Opzione per mostrare i log sulla console
    Time currentTime;      // Oggetto Time per il timestamp
    std::vector<std::string> dailyLogs; //contenitore che contiene i messaggi caricati in log.txt nella giornata
    Time lastLogTime;  // Tiene traccia dell'ultimo orario registrato

public:
    //costruttore
    LoggerTime(const std::string& filename, bool showToConsole = true);
    //distruttore
    ~LoggerTime();
    
    // Scrive un messaggio di log
    void write(const std::string& message, const Time& logTime); 
    // Scrive un messaggio di log
    void write(const std::string& message, const Network& net);
    
    // funzione per stampare tutti i log fino all'orario corrente
    void printLogsUpToCurrentTime(Network& network, const Time& newTime);

    // funzione per aggiornare l'ultimo orario stampato
    void updateLastPrintedTime(const Time& newTime);

    //funzione per stampare i comandi nel file di log
    void LoggerTime::logCommand(const std::string& command);
};

#endif