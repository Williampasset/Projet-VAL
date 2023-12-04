#include "Station.h"
using namespace std;
Station::Station() {
    //cout<<"Nouvelle Station"<<endl;
}
Station::Station(const string& nom_, const float& DistanceDA_, const double& distanceBefStation_, const int& depart) : nom(nom_), DistanceDA(DistanceDA_), distanceBefStation(distanceBefStation_){
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
double Station::getDistanceDAstation() const{
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
    Depart = Depart_;
}
void Station::setDistanceDA(const double& DistanceDA_){
    DistanceDA = DistanceDA_;
}
void Station::randPassager(){
    if (getNbpassager() < 10) {
        int randToAdd = rand() % (10 - getNbpassager());
        Nbpersonne += randToAdd;
    }
}
void Station::setDistanceBefStation(const double& distanceBefStation_) {
    distanceBefStation = distanceBefStation_;
}
double Station::getDistanceBefStation() const{
    return distanceBefStation;
}