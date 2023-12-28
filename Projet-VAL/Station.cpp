#include "Station.h"
using namespace std;
Station::Station() {
    //cout<<"Nouvelle Station"<<endl;
}
Station::Station(const string& nom_, const float& DistanceDA_, const float& distanceBefStation_, const int& depart) : nom(nom_), DistanceDA(DistanceDA_), distanceBefStation(distanceBefStation_) {
    //cout<<"Station "<<nom<<endl;
    Depart = depart;
}
Station::Station(const Station& station) {
    Nbpersonne = station.getNbpassager();
    nom = station.getNom();
    DistanceDA = station.getDistanceDAstation();
    Depart = station.getDepart();
    distanceBefStation = station.getDistanceBefStation();
}
Station::~Station(){
    //cout<<"Fin Station"<<endl;
}
void Station::setNbpassager(const int& Nbpersonne_){
    Nbpersonne = Nbpersonne_;
}
void Station::setEtatMA(const bool& etatMA_){
    etatMA = etatMA_;
}
string Station::getNom() const{
    return nom;
}
int Station::getNbpassager() const{
    return Nbpersonne;
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
void Station::randPassager(){
    srand(static_cast<unsigned int>(time(nullptr)));
    if (getNbpassager() < 10 && getDepart() != 2 ) {
        int randToAdd = rand() % (10 - getNbpassager());
        Nbpersonne += randToAdd;
        cout << "Station: " << getNom() << " Nombre de personne dans la station: " << getNbpassager() << endl;
    }
}
void Station::setDistanceBefStation(const float& distanceBefStation_) {
    distanceBefStation = distanceBefStation_;
}
float Station::getDistanceBefStation() const{
    return distanceBefStation;
}

void Station::copyStation(Station& OtherStation) {
    Nbpersonne = OtherStation.getNbpassager();
    nom = OtherStation.getNom();
    DistanceDA = OtherStation.getDistanceDAstation();
    Depart = OtherStation.getDepart();
    distanceBefStation = OtherStation.getDistanceBefStation();
}