//CHIARA VIGNA

#include "../include/Device.h"
#include "../include/Time.h" 

//inizializzazione dei nomi validi per i dispositivi
const std::vector<std::string> Device::validDeviceNames = {
    "lavatrice", "lavastoviglie", "tapparelle_elettriche",
    "forno_a_microonde", "asciugatrice", "televisore",
    "impianto_fotovoltaico", "pompa_di_calore+termostato",
    "scaldabagno", "frigorifero"
};

//inizializzazione dei nomi per i dispositivi a ciclo prefissato
const std::vector<std::string> Device::fixedCycleDeviceNames = {
    "lavatrice", "lavastoviglie", "tapparelle_elettriche",
    "forno_a_microonde", "asciugatrice", "televisore"
};

//inizializzazione dei nomi per i dispositivi manuali
const std::vector<std::string> Device::manualDeviceNames = {
    "impianto_fotovoltaico", "pompa_di_calore+termostato",
    "scaldabagno", "frigorifero"
};

//costruttore
Device::Device(std::string DEVICENAME, int ID, double power) {
    this->DEVICENAME = DEVICENAME;
    this->ID = ID;
    this->power = power;
    this->state = false;
    this->switchOnTime = Time(0,0);
    this->switchOffTime = Time(0,0);

    if (!isValidName()) {
        std::cout<<"Nome del dispositivo non valido: " << DEVICENAME<<"\n";
    }
}

//verifica che il dispositivo sia tra quelli presenti nella lista di nomi validi
bool Device::isValidName() const{
    for (const auto& name : validDeviceNames) { 
        if (name == DEVICENAME) {
            return true; // Se c'e' una corrispondenza, il nome è valido
        }
    }
    return false; // Se nessuna corrispondenza viene trovata, il nome non è valido
}

//verifica che il dispositivo sia a ciclo prefissato
bool Device::isFixedCycle() const {
    for (const auto& name : fixedCycleDeviceNames) {
        if (name == DEVICENAME)
            return true; //il dispositivo e' a CP
    }
    return false;
}

//verifica se il dispositivo sia manuale
bool Device::isManual() const {
    for (const auto& name : manualDeviceNames) {
        if (name == DEVICENAME)
            return true; //il dispositivo e' M
    }
    return false;
}

//funzioni getter
std::string Device::getName() const { 
    return this->DEVICENAME; //restituisce il nome del dispositivo
}
int Device::getID() const {
    return this->ID; //restituisce l'ID
}
double Device::getPower() const {
    return this->power; //restituisce la potenza 
}

//verifica se il dispositivo è acceso restituendone lo stato 
bool Device::isOn() const {
    return state;
}

//accende il dispositivo
void Device::turnOn(const Time& currentTime) {
    if (state) { //verifa che il dispositivo non sia già acceso
        std::cout << "Il dispositivo '" << DEVICENAME << "' e' già acceso.\n";
        return;
    }
    state = true;
    this->switchOnTime = currentTime; //imposta orario di accensione corrente
}

//spegne il dispositivo
void Device::turnOff(const Time& currentTime) {
    if (!state) { //verifica che il dispositivo non sia già spento
        std::cout << "Il dispositivo '" << DEVICENAME << "' e' già spento.\n";
        return;
    }
    state = false;
    this->switchOffTime = currentTime; //imposta orario di accensione corrente

    // Calcola il consumo totale durante il periodo in cui è stato acceso
    double hoursOn = switchOnTime.differenceInMinutes(currentTime) / 60.0;
    double consumption = power * hoursOn;
} 

//mostra il consumo del singolo dispositivo
std::string Device::showOneDevice(const Time& currentTime) const {
    std::string str = "Il dispositivo '" + DEVICENAME + "' ";
    
    if (state) {
        // Se il dispositivo è acceso, calcola il consumo fino all'orario corrente
        double hoursOn = switchOnTime.differenceInMinutes(currentTime) / 60.0;
        str = str + "ha consumato " + std::to_string(this->power * hoursOn) + " kWh\n";
    } else {
        // Se il dispositivo è spento, calcola il consumo totale dall'ultimo accensione-spegnimento
        double hoursOn = switchOnTime.differenceInMinutes(switchOffTime) / 60.0;
        str = str + "ha consumato " + std::to_string(this->power * hoursOn) + " kWh\n";
    }
    return str;
}
