//CHIARA VIGNA

#include "../include/DeviceManual.h"
#include "../include/Time.h"

//costruttore
DeviceManual::DeviceManual(const std::string& DEVICENAME, int ID, double power) : Device(DEVICENAME, ID, power) {}

//funzione per calcolare il consumo energetico del dispoitivo manuale
double DeviceManual::calculateDeviceConsumption() const {
    if (switchOffTime > switchOnTime) {
        double hoursOn = switchOnTime.differenceInMinutes(switchOffTime) / 60.0;
        return power * hoursOn; // Restituisce il consumo
    } else {
        return 0.0; // Nessun consumo se il dispositivo non è stato spento correttamente
    }
}

//funzioni get e set
void DeviceManual::setTimer(Time start, Time stop) {
    if (stop <= start) {
        //verifica che l'orario di spegnimento non sia antecedente a quello di accensione
        throw std::invalid_argument("Errore: L'orario di spegnimento deve essere successivo a quello di accensione.");
    }
    switchOnTime = start;
    switchOffTime = stop;
}
std::string DeviceManual::getType() const {
    return "Manuale"; //restituisce il tipo di dispositivo
}