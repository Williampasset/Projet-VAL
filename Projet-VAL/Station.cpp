#include "Station.h"
using namespace std;
Station::Station() {
    cout<<"Nouvelle Station"<<endl;
}
Station::Station(const string& nom_, const float& DistanceDA_, const int& depart) : nom(nom_), DistanceDA(DistanceDA_){
    cout<<"Station "<<nom<<endl;
    Depart = depart;
}
Station::~Station(){
    cout<<"Fin Station"<<endl;
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
    Nbpersonne = rand() % 10-Nbpersonne + 1;
}