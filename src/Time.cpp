//inclusione librerie necessarie
#include <iostream>
#include <string>
#include "../include/Time.h"

 //costruttore
Time::Time(int h, int m) : hours(h), minutes(m) {
    normalize();
}

int Time::getHours() const { return hours; }
int Time::getMinutes() const { return minutes; }

//funzione membro private per gestire un giorno in 24 ore
//ed ogni ora in 60 minuti
void Time::normalize() {
    //normalizza i minuti in ore (60 minuti)
    hours += minutes / 60;
    minutes %= 60;
    //normalizza le ore in un giorno (24 ore)
    hours %= 24;
}

//funzione membro per aggiungere tempo
void Time::addTime(int h, int m) {
    hours += h;
    minutes += m;
    normalize();
}

//funzione membro per cambiare l'orario
void Time::changeTime(int h, int m){
   if(h>=0 && h<24 && m>=0 && m<60){
      hours = h;
      minutes = m;
   }
}

//funzione membro per stampare l'orario
std::string Time::printTime() const {
    std::string hourStr = (hours < 10 ? "0" : "") + std::to_string(hours);
    std::string minuteStr = (minutes < 10 ? "0" : "") + std::to_string(minutes);
    return hourStr + ":" + minuteStr;
}

//funzione restituisce il numero di minuti trascorsi tra l’orario corrente (this) 
//e l’orario passato come parametro (other)
int Time::differenceInMinutes(const Time& other) const {
    int thisTotalMinutes = this->hours * 60 + this->minutes;
    int otherTotalMinutes = other.hours * 60 + other.minutes;
    int diff = otherTotalMinutes - thisTotalMinutes;

    // Se la differenza è negativa, calcola come se fosse il giorno successivo
    if (diff < 0) {
        diff += 24 * 60; // Aggiungi un giorno intero (1440 minuti)
    }
    return diff;
}

//funzione membro per resettare l'orario a 00:00
void Time::resetTime() {
    hours = 0;
    minutes = 0;
}

// Operatore + per sommare due oggetti Time
Time Time::operator+(const Time& other) const {
    int newHours = hours + other.hours;
    int newMinutes = minutes + other.minutes;

    // Creiamo un nuovo oggetto Time e normalizziamo il risultato
    Time result(newHours, newMinutes);
    result.normalize();
    return result;
}

//operatori di confronto per due oggetti Time
bool Time::operator==(const Time& other) const {
    return hours == other.hours && minutes == other.minutes;
}
bool Time::operator<(const Time& other) const {
    return (hours < other.hours) || (hours == other.hours && minutes < other.minutes);
}
bool Time::operator<=(const Time& other) const {
    return (*this < other) || (*this == other);
}
bool Time::operator>(const Time& other) const {
    return !(*this <= other);
}
bool Time::operator>=(const Time& other) const {
    return !(*this < other);
}

