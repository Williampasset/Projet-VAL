#include "Station.h"
using namespace std;
Station::Station() {
    cout<<"Nouvelle Station"<<endl;
}
Station::Station(const string& nom_, const float& DistanceDA_, const float& distanceBefStation_, const int& depart) : nom(nom_), DistanceDA(DistanceDA_), distanceBefStation(distanceBefStation_) {
    cout<<"Station "<<nom<<endl;
    Depart = depart;
}
Station::Station(const Station& station) {
    NbpersonneDroite = station.getNbpassagerDroite();
    NbpersonneGauche = station.getNbpassagerGauche();
    nom = station.getNom();
    DistanceDA = station.getDistanceDAstation();
    Depart = station.getDepart();
    distanceBefStation = station.getDistanceBefStation();
}
Station::~Station(){
    cout<<"Fin Station"<<endl;
}
void Station::setEtatMA(const bool& etatMA_){
    etatMA = etatMA_;
}
string Station::getNom() const{
    return nom;
}
float Station::getDistanceDAstation() const{
    return DistanceDA;
}
bool Station::getEtatMA() const{
    return etatMA;
}
float Station::getTempspassager() const{
    return Tempspassager;
}
float Station::getTempsarretsec() const{
    return Tempsarretsec;
}
int Station::getDepart() const{
    return Depart;
}
void Station::setDepart(const int& Depart_){
    if (Depart_ >= 0 && Depart_ <= 2) {
        Depart = Depart_;
    }
    else {
        Depart = 0;
    }
}
void Station::setDistanceDA(const float& DistanceDA_){
    DistanceDA = DistanceDA_;
}
void Station::randPassager(const int& direction_){
    srand(static_cast<unsigned int>(time(nullptr)));
    if (getDepart() != 2) {
        if(direction_ == 1 && getNbpassagerDroite() < 10){
            int randToAdd = rand() % (10 - getNbpassagerDroite());
            NbpersonneDroite += randToAdd;
            cout << "Station: " << getNom() << " Nombre de personne dans la station a droite: " << getNbpassagerDroite() << endl;
        }
        else if(direction_ == -1 && getNbpassagerGauche() < 10){
            int randToAdd = rand() % (10 - getNbpassagerGauche());
            NbpersonneGauche += randToAdd;
            cout << "Station: " << getNom() << " Nombre de personne dans la station a gauche: " << getNbpassagerGauche() << endl;
        }
    }
}
void Station::setDistanceBefStation(const float& distanceBefStation_) {
    distanceBefStation = distanceBefStation_;
}
float Station::getDistanceBefStation() const{
    return distanceBefStation;
}

void Station::copyStation(Station& OtherStation) {
    NbpersonneDroite = OtherStation.getNbpassagerDroite();
    NbpersonneGauche = OtherStation.getNbpassagerGauche();
    nom = OtherStation.getNom();
    DistanceDA = OtherStation.getDistanceDAstation();
    Depart = OtherStation.getDepart();
    distanceBefStation = OtherStation.getDistanceBefStation();
}
void Station::setNbpassagerDroite(const int& Nbpersonne_) {
    NbpersonneDroite = Nbpersonne_;
}
void Station::setNbpassagerGauche(const int& Nbpersonne_) {
    NbpersonneGauche = Nbpersonne_;
}
int Station::getNbpassagerDroite() const {
    return NbpersonneDroite;
}
int Station::getNbpassagerGauche() const {
    return NbpersonneGauche;
}