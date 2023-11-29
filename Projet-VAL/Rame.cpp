#include "Rame.h"
#include "Station.h"
using namespace std;
Rame::Rame(){
    //cout<<"Rame created"<<endl;
}
Rame::Rame(const int& id_) : id(id_){
    //cout<<"Rame created"<<endl;
}
Rame::~Rame(){
    //cout<<"Rame destroyed"<<endl;
}
void Rame::setV(const double& v_){
    v = v_;
}
void Rame::setDistanceDA(const double& distanceDA_){
    distanceDA = distanceDA_;
}
void Rame::setNbpassager(const int& nbpassager_){
    nbpassager = nbpassager_;
}
int Rame::getId() const{
    return id;
}
int Rame::getNbpassager() const{
    return nbpassager;
}
int Rame::getnbpassagermax() const{
    return nbpassagermax;
}
double Rame::getV() const{
    return v;
}
double Rame::getDistanceDArame() const{
    return distanceDA;
}
bool Rame::getUrgence() const{
    return urgence;
}
void Rame::setUrgence(const bool& urgence_){
    urgence = urgence_;
}
double Rame::distanceToNextRame(const Rame& otherrame){
	return otherrame.distanceTotal - distanceTotal;
}
void Rame::setDistanceTotal(const double& distance){
	distanceTotal = distance;
}
double Rame::getDistanceTotal() const{
	return distanceTotal;
}