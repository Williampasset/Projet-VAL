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
void Rame::setDistanceOldStation(const double& distanceDA_){
    distanceOldStation = distanceDA_;
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
double Rame::getDistanceOldStation() const{
    return distanceOldStation;
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
	distanceTotal += distance;
}
double Rame::getDistanceTotal() const{
	return distanceTotal;
}
void Rame::Avancer(Station& nextStation) {
    auto distance = getDistanceOldStation(); //distance de la rame par rapport � la rame d'avant, variable incr�menter pour calculer la distance en permanence
    cout << "Station suivante: " << nextStation.getNom() << "\tRame: "<<getId()<< endl;//affichage de la station suivante, la station o� on va 
	double time = 0;
	double distanceacc = 0;
	double distanceconst = 0;
	double distancedec = 0;
    while (abs(distance - (nextStation.getDistanceBefStation())) >= 1 || (nextStation.getDepart() == 1 && abs(distance - (nextStation.getDistanceDAstation())) >= 1) ) {
		this_thread::sleep_for(100ms);
		time += 0.1;
		if (getV() < 16.6 && (nextStation.getDistanceBefStation()) - distance > 100) {//d�marrage quand on est au d�part 
			if (distanceacc == 0) {
				time = 0.1;
			}
			distanceacc = (((1.4) * (time*time) * 0.5) + distancedec + distanceconst);
			cout << "distance rame demarrage :  " << distanceacc << endl;
			distance = distanceacc;
			setV((1.4) * (time));
		}
		else if ((nextStation.getDistanceBefStation()) - distance < 100) {//d�c�l�ration normale
			if (distancedec == 0) {
				time = 0.1;
			}
			distancedec = ((16.6 * time - (1.4 * (time * time) * 0.5)) + distanceconst);
			cout << "distance rame deceleration :  " << distancedec << endl;
			distance = distancedec;
			setV(16.6 - (1.4*time));
		}
		else {
			if (distanceconst == 0) {
				time = 0.1;
			}
			distanceconst = ((16.6 * time) + distanceacc + distancedec);
			cout << "distance rame constant :  " << distanceconst << endl;
			distance = distanceconst;
			setV(16.6);
		}
		setDistanceOldStation(distance);
	}
	cout << "Arret station" << nextStation.getNom()<< endl;
	setDistanceTotal(distance);
	setDistanceOldStation(0);
	nextStation.setEtatMA(false);
}
void Rame::Arreter(Station& StopStation) {
	if (StopStation.getNbpassager() > 0) {
		auto n = 0;
		if (StopStation.getDepart() != 1) {
			n = rand() % (getNbpassager() + 1);
		}
		cout << "Nombre de personne qui sortent de la rame "<<getId()<<" : "<<  n << endl;
		for (auto i = 0; i < n; i++) {
			this_thread::sleep_for(0.5s);
		}
		setNbpassager(getNbpassager() - n);
		if (StopStation.getNbpassager() >= (10 - getNbpassager())) {
			n = 10 - getNbpassager();
			StopStation.setNbpassager(StopStation.getNbpassager() - (10 - getNbpassager()));
			for (auto i = 0; i < n; i++) {
				this_thread::sleep_for(0.5s);
			}
			cout << "Nombre de personne qui rentrent de la rame " << getId() << " : " << n << endl;
			setNbpassager(10);
		}
		else {
			n = StopStation.getNbpassager();
			setNbpassager(getNbpassager() + StopStation.getNbpassager());
			for (auto i = 0; i < n; i++) {
				this_thread::sleep_for(0.5s);
			}
			cout << "Nombre de personne qui rentrent de la rame " << getId() << " : " << n << endl;
			StopStation.setNbpassager(0);
		}
	}
	else {
		if (StopStation.getDepart() == 2) {
			cout << "Nombre de personne qui sortent de la rame " << getId() << " : " << getNbpassager() << endl;
			for (auto i = 0; i < getNbpassager(); i++) {
				this_thread::sleep_for(0.5s);
			}
			setNbpassager(0);
		}
	}
	/*while (rame.getUrgence()) {
		this_thread::sleep_for(3s);
	}*/
	//if (rame.getId() != 1) {
	//	/*while (Super.getRame(rame.getId() - 1).getDistanceDArame() - rame.getDistanceDArame() < 400) {
	//		cout << Super.getRame(rame.getId() - 1).getDistanceDArame() << endl;
	//		this_thread::sleep_for(3s);
	//	}*/
	//	/*if (distanceToNextRame(Super.getRame(rame.getId() - 1)) > 300) {
	//		station.setEtatMA(true);
	//	}*/
	//}
	this_thread::sleep_for(3s);
	StopStation.setEtatMA(true);
	StopStation.randPassager();
	cout << "Nombre de passager rame " << getId() << ": " << getNbpassager() << endl;
	cout << "Nombre de passager station"<< StopStation.getNom() <<" : " << StopStation.getNbpassager() << endl;
}

void Rame::setDirection(const int& direction_) {
	direction = direction_;
}
int Rame::getDirection() {
	return direction;
}