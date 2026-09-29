Il progetto è impostato su diverse classi per rendere il codice più leggibile e semplice.

La classe Device, progettata da Chiara Vigna, rappresenta un dispositivo generico nel sistema domotico, con funzionalità per gestire il suo stato (on/off), verificare se è un dispositivo valido, manuale o a ciclo prefissato, calcolare il consumo di energia e fornire informazioni sul dispositivo.

La classe DeviceFixedCycle, progettata da Chiara Vigna, rappresenta un dispositivo domotico con ciclo prefissato, cioè un dispositivo che, una volta acceso, si spegne automaticamente dopo un tempo predefinito. Questa classe estende la classe Device e aggiunge funzionalità specifiche per gestire la durata del ciclo.

La classe DeviceManual, progettata da Chiara Vigna, rappresenta un dispositivo domotico manuale, ovvero un dispositivo che può essere gestito direttamente impostando un timer di accensione e spegnimento manuale. Questa classe estende la classe base Device e aggiunge funzionalità specifiche per i dispositivi manuali.

La classe LoggerTime, progettata da Elena Pavanello, ha il compito di scrivere su file esterno "log.txt" tutti i comandi che l'utente scrive da riga di comando e permette di stampare i log generati in un intervallo di tempo specifico.

La classe Network, progettata da Adele Bacchiega, rappresenta il cuore del sistema domotico. Si occupa di gestire dispositivi, timer e consumi energetici, monitorando e bilanciando la potenza disponibile.

La classe Time, progettata da Adele Bacchiega, fornisce un modo semplice per rappresentare, gestire e confrontare un orario in formato 24 ore.

La classe Timer, progettata da Chiara Vigna, rappresenta un timer associato a un dispositivo nella rete domotica. Gestisce gli orari di accensione e spegnimento per dispositivi, permettendo di controllare il loro comportamento temporale.

Ed infine il main, progettato da Elena Pavanello, che si occupa di creare e inizializzare una rete di dispositivi e controllare tutti i vari comandi, tramite le funzioni fornite dalle altre classi, che l'utente potrebbe inserire.

Nel corso della progettazione del lavoro, abbiamo riscontrato problemi nel compilare tramite cMake e nella realizzazione di alcune funzioni che siamo riuscite a risolvere dopo esserci confrontate e informate. Un problema che non siamo riuscite a risolvere pienamente, invece, è riguardo la gestione della potenza che in certi casi particolari potrebbe non risultare completamente corretta. 

Per la correzione, informiamo che nei dispositivi composti da più parole, abbiamo sostituito lo spazio con un trattino basso “_” in quanto abbiamo riscontrato problemi nella lettura di due parole da parte del compilatore.
