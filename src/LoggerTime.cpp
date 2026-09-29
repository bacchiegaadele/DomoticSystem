#include "../include/LoggerTime.h"
#include "../include/Network.h"
#include <iostream>

// Costruttore: apre il file di log
LoggerTime::LoggerTime(const std::string& filename, bool showToConsole)
    : showToConsole(showToConsole), currentTime(0, 0), lastLogTime(0,0) { // Imposta l'ora iniziale a 00:00
    logFile.open(filename, std::ios::out | std::ios::app);//gli ultimi due pezzi tra parantesi servono per
    //aprire il file in scrittura (ios::out)
    // e non sovrascrivere i comandi precedenti cosi da avere la cronologia completa (ios::app)
    if (!logFile) {
        throw std::runtime_error("Impossibile aprire il file di log: " + filename);
    }
}

// Distruttore: chiude il file di log
LoggerTime::~LoggerTime() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Funzione per scrivere un messaggio di log
void LoggerTime::write(const std::string& message, const Network& net) {
    // Ottieni l'ora corrente come stringa
    std::string timeStr = net.getCurrentTime().printTime();

    // Componi il messaggio completo
    std::string logMessage = "[" + timeStr + "] " + message;

    // Scrivi nel file di log
    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
    }

    // Mostra sulla console (non richiesto obbligatoriamente)
    if (showToConsole) {
        std::cout << logMessage << std::endl;
    }

    // Aggiungi ai log giornalieri
    dailyLogs.push_back(logMessage);

    // Aggiorna l'orario dell'ultimo log registrato
    lastLogTime = net.getCurrentTime();
}


// Funzione per scrivere un messaggio di log con parametro l'orario del log
void LoggerTime::write(const std::string& message, const Time& logTime) {
    // Componi il messaggio completo con l'orario specificato
    std::string timeStr = logTime.printTime();
    std::string logMessage = "[" + timeStr + "] " + message;

    // Scrivi nel file di log
    if (logFile.is_open())
        logFile << logMessage << std::endl;

    // Aggiungi ai log giornalieri
    dailyLogs.push_back(logMessage);
}


// Funzione per stampare tutti i log fino all'orario corrente
void LoggerTime::printLogsUpToCurrentTime(Network& network, const Time& newTime) {
    std::cout << "Log dall'orario " << lastLogTime.printTime() << " a " << newTime.printTime() << ":\n";

    // Aggiorna l'orario dell'ultimo log stampato
    Time previousLastLogTime = lastLogTime;  // Salva il valore precedente
    lastLogTime = newTime;                   // Aggiorna l'orario corrente

    // Controlla i timer e aggiorna lo stato dei dispositivi nell'intervallo
    network.checkTimers(*this, previousLastLogTime, newTime);

    // Stampa i log generati nell'intervallo
    for (const auto& log : dailyLogs) {
        std::string logTimeStr = log.substr(1, 5);  // Assume formato [HH:MM] all'inizio del log
        int logHour = std::stoi(logTimeStr.substr(0, 2));
        int logMinute = std::stoi(logTimeStr.substr(3, 2));
        Time logTime(logHour, logMinute);

        if (logTime > previousLastLogTime && logTime <= newTime) {
            std::cout << log << std::endl;
        }
    }
    std::cout << std::endl;
}

// Funzione per aggiornare l'ultimo orario stampato
void LoggerTime::updateLastPrintedTime(const Time& newTime) {
    lastLogTime = newTime;
}

// Funzione per scrivere un comando nel log
void LoggerTime::logCommand(const std::string& command) {

    // Componi il messaggio del comando con il timestamp
    std::string logMessage ="Comando: " + command;

    // Scrivi il comando nel file di log
    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
    }

}
