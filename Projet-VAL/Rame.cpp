#include "Rame.h"
# define M_PI  3.14159265358979323846

Rame::Rame(){
    cout<<"Rame created 0"<<endl;
}
Rame::Rame(const int& id_) : id(id_) {
	NextRame = nullptr;
	cout<<"Rame created 1"<<endl;
}
Rame::Rame(const int& id_, Rame* NextRame_) : id(id_) {
	NextRame = NextRame_;
	cout << "Rame created 2" << endl;
};
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
double Rame::distanceToNextRame(){
	if (NextRame == nullptr) {
        cout << "Pas de rame suivante." << endl;
        return 0.0; // ou une valeur appropriée
    }
	else{
		return NextRame->getDistanceTotal() - getDistanceTotal();
	}
}
void Rame::setNextRame(Rame* NextRame_){
	NextRame = NextRame_;
}
void Rame::setDistanceTotal(const double& distance){
	distanceTotal = distance;
}
double Rame::getDistanceTotal() const{
	return distanceTotal;
}
void Rame::Avancer(Station& nextStation) {
    auto distance = getDistanceOldStation(); //distance de la rame par rapport à la rame d'avant, variable incrémenter pour calculer la distance en permanence
    cout << "Station suivante: " << nextStation.getNom() << "\tRame: "<<getId()<< endl;//affichage de la station suivante, la station où on va 
	float time = 0;
	float distanceTotActuel = getDistanceTotal();
	float distanceLigneActuel = getDistanceLigne();
	float distanceacc = 0;
	float distanceconst = 0;
	float distancedec = 0;
    while (abs(distance - (nextStation.getDistanceBefStation())) >= 1 || (nextStation.getDepart() == 1 && abs(distance - (nextStation.getDistanceDAstation())) >= 1) || (urgence && getV() >= 1)  && distanceToNextRame() > SECURDISTANCE) {
		this_thread::sleep_for(100ms);
		time += 0.1;
		if (getV() < VMAX && (nextStation.getDistanceBefStation()) - distance > STOPDISTANCE && !urgence) {//condition d'acceleration
			if (distanceacc == 0) {
				time = 0.1;
			}
			distanceacc = (((ACC) * (time*time) * 0.5) + distancedec + distanceconst);
			distance = distanceacc;
			setV((1.4) * (time));
		}
		else if (((nextStation.getDistanceBefStation()) - distance < STOPDISTANCE) || (urgence && getV() >= 1) ) {//décélération normale
			if (distancedec == 0) {
				time = 0.1;
			}
			distancedec = ((VMAX * time - (ACC * (time * time) * 0.5)) + distanceconst);
			distance = distancedec;
			setV(VMAX - (ACC*time));
		}
		else {//Vitesse constante
			if (distanceconst == 0) {
				time = 0.1;
			}
			distanceconst = ((VMAX * time) + distanceacc + distancedec);
			distance = distanceconst;
			setV(VMAX);
		}
		setDistanceOldStation(distance);
		setDistanceTotal(distance + distanceTotActuel);
		setDistanceLigne(distance + distanceLigneActuel);
		setPos();
	}
	cout << "Arret station: " << nextStation.getNom() << endl;
	setDistanceOldStation(0);
	nextStation.setEtatMA(false);
}

void Rame::Arreter(Station& StopStation) {
	while (getId() != 1 && distanceToNextRame() < SECURDISTANCE) {
		this_thread::sleep_for(1s);
	}
	setPos();
	if (!Go) {
		Go = true;
	}
	StopStation.randPassager();
	if (StopStation.getNbpassager() > 0 && StopStation.getDepart() != 2) {
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
			cout << "Nombre de personne qui rentrent dans la rame " << getId() << " : " << n << endl;
			setNbpassager(10);
		}
		else {
			n = StopStation.getNbpassager();
			setNbpassager(getNbpassager() + StopStation.getNbpassager());
			for (auto i = 0; i < n; i++) {
				this_thread::sleep_for(0.5s);
			}
			cout << "Nombre de personne qui rentrent dans la rame " << getId() << " : " << n << endl;
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
	this_thread::sleep_for(3s);
	StopStation.setEtatMA(true);
	cout << "Nombre de passager rame " << getId() << ": " << getNbpassager() << endl;
	cout << "Nombre de passager station "<< StopStation.getNom() <<" : " << StopStation.getNbpassager() << endl;
	if (StopStation.getDepart() == 2) {
		double angle =  direction == 1 ? (-1)*(M_PI / 2) : (M_PI / 2);
		double endAngle = direction == 1 ? (M_PI / 2) : 3*M_PI/2 ;
		while (angle <= endAngle) {
			this_thread::sleep_for(10ms);
			Rotate(angle);
			angle += M_PI / 400;
		}
		rotate = true;
		cout << "Changement de voie de la rame " << getId() << endl;
		StopStation.setEtatMA(false);
		setDistanceLigne(0);
		this_thread::sleep_for(1s);
		rotate = false;
	}

}

void Rame::setDirection(const int& direction_) {
	direction = direction_;
}
int Rame::getDirection() {
	return direction;
}
void Rame::setDistanceLigne(const float& distanceLigne_) {
	distanceLigne = distanceLigne_;
}
float Rame::getDistanceLigne() {
	return distanceLigne;
}
Rame* Rame::getNextRame() {
	return NextRame;
}
bool Rame::getGo() {
	return Go;
}
float Rame::getXpos() const {
	return x;
};
float Rame::getYpos() const {
	return y;
};
void Rame::setXpos(const float& newX){
	x = newX;
}
void Rame::setYpos(const float& newY) {
	y = newY;
}
void Rame::setPos() {
	if (direction == 1) {
		setXpos((55 + (distanceLigne*1720/1200)));
		cout << x << endl;
		cout << distanceLigne << endl;
		setYpos(40.f);
	}
	else {
		setXpos(1860 - (distanceLigne * 1720 / 1200));
		setYpos(160.f);
	}
}
void Rame::Rotate(const double& angle) {
	//Position du point centrale
	double centerX = (direction == 1 ? 1860 : 55);
	double centerY = 100;

	// Effectue la rotation autour du centre (x, y)
	setXpos(centerX + (cos(angle) * 60));
	setYpos(centerY + (sin(angle) * 60));
}
bool Rame::getRotate() const{
	return rotate;
}