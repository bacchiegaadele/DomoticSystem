//ADELE BACCHIEGA

//include guards
#ifndef TIME_H
#define TIME_H

#include <string>

class Time{
private:
    //variabili private
    int hours;
    int minutes;

    //funzione membro private per gestire un giorno in 24 ore
    //ed ogni ora in 60 minuti
    void normalize();

public:
    //costruttore
    Time(int h = 0, int m = 0);

    //funzioni get
    int getHours() const;
    int getMinutes() const;

    //funzione membro per aggiungere tempo
    void addTime(int h, int m);
    
    //funzione membro per cambiare l'orario
    void changeTime(int h, int m);

    //funzione membro per stampare l'orario
    std::string printTime() const;

    int differenceInMinutes(const Time& other) const;

    void resetTime();

    //operatore + per sommare due oggetti Time
    Time operator+(const Time& other) const;

    //confronto di due oggetti Time
    bool operator==(const Time& other) const;
    bool operator<(const Time& other) const;
    bool operator<=(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator>=(const Time& other) const;
};

#endif
