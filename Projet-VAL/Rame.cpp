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
    cout<<"Rame destroyed"<<endl;
}
void Rame::setV(const double& v_){
    v = v_;
}
void Rame::setDistanceOldStation(const float& distanceDA_){
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
float Rame::getV() const{
    return v;
}
float Rame::getDistanceOldStation() const{
    return distanceOldStation;
}
bool Rame::getUrgence() const{
    return urgence;
}
void Rame::setUrgence(const bool& urgence_){
    urgence = urgence_;
}
float Rame::distanceToNextRame(){
	if (NextRame == nullptr) {
        cout << "Pas de rame suivante." << endl;
        return 0.0; // ou une valeur appropriée
    }
	else if(id!=1){
		return abs(NextRame->getDistanceTotal() - getDistanceTotal());
	}
	else if ((compteLine + NextRame->compteLine)%2 == 1 && compteLine>0) {
		return NextRame->getDistanceTotal() + DISTANCELINE - distanceLigne;
	}
	else if ((compteLine + NextRame->compteLine) % 2 == 0 && compteLine > 0) {
		return NextRame->getDistanceLigne() - distanceLigne;
	}
	else {
		return SECURDISTANCE + 1;
	}
}
void Rame::setNextRame(Rame* NextRame_){
	NextRame = NextRame_;
}
void Rame::setDistanceTotal(const float& distance){
	distanceTotal = distance;
}
float Rame::getDistanceTotal() const{
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
	float distanceUrgence = 0;
	float vitesse = 0;
	float stopDistance = (VMAX*VMAX) / (2*ACC);
	bool Dec = false;
    while (abs(distance - (nextStation.getDistanceBefStation())) >= 2 || (nextStation.getDepart() == 1 && abs(distance - (nextStation.getDistanceDAstation())) >= 1)) {
		this_thread::sleep_for(100ms);
		time += 0.1;
		if(distanceToNextRame() < SECURDISTANCE && NextRame->getGo() && !urgence && Dec == false){
			urgence = true;
			urgenceAuto = true;
		}
		if(urgenceAuto && distanceToNextRame() > SECURDISTANCE){
			urgence = false;
			urgenceAuto = false;
		}
		if (getV() < VMAX && (nextStation.getDistanceBefStation()) - distance > stopDistance && !urgence) {//condition d'acceleration
			Dec = false;
			if (distanceacc == 0) {
					time = 0.1;
			}
			distanceacc = (((ACC) * (time*time) * 0.5) + distancedec + distanceconst + distanceUrgence);
			distance = distanceacc;
			setV((1.4) * (time));
			vitesse = getV();
			if(nextStation.getDistanceBefStation() - distance < 2*stopDistance && stopDistance == (VMAX*VMAX) / (2*ACC)){
				cout<<"Distance parcourue: "<<distance<<endl;
				stopDistance = abs((nextStation.getDistanceBefStation() - distance)/2);
				cout<<"Distance avant station: "<<nextStation.getDistanceBefStation() - distance<<endl;
				cout<<"STOP DISTANCE rame "<<getId()<<" : "<<stopDistance<<endl;
			}
		}
		else if (((nextStation.getDistanceBefStation()) - distance < stopDistance) || (urgence && getV()>1)) {//décélération normale
			Dec = true;
			if (distancedec == 0) {
				time = 0.1;
			}
			if(getV()<VMAX && distanceconst == 0){
				distancedec = ((vitesse * time - (ACC * (time * time) * 0.5)) + distanceacc);
				distance = abs(distancedec);
				setV(vitesse - (ACC*time));
			}
			else{
				distancedec = ((VMAX * time - (ACC * (time * time) * 0.5)) + distanceconst);
				distance = distancedec;
				setV(VMAX - (ACC*time));
			}
		}
		else if(urgence && (getV() <= 1)) {
			cout<<"URGENCE"<<endl;
			setV(0);
			distanceacc = 0;
			distanceconst = 0;
			distancedec = 0;
			distanceUrgence = distance;
		}
		else if(!urgence){//Vitesse constante
			Dec = false;
			if (distanceconst == 0) {
				time = 0.1;
			}
			distanceconst = ((VMAX * time) + distanceacc + distancedec);
			distance = distanceconst;
			setV(VMAX);
		}
		setDistanceOldStation(distance);
		setDistanceTotal(abs(distance + distanceTotActuel));
		setDistanceLigne(abs(distance + distanceLigneActuel));
		setPos();
	}
	setDistanceLigne(nextStation.getDistanceDAstation());
	setDistanceTotal((DISTANCELINE * compteLine) + nextStation.getDistanceDAstation());
	setPos();
	cout << "Arret station: " << nextStation.getNom() << endl;
	setDistanceOldStation(0);
	nextStation.setEtatMA(false);
	setV(0);
}

void Rame::Arreter(Station& StopStation, Station& BaseStation) {
	while (getId() != 1 && distanceToNextRame() < SECURDISTANCE) {
		this_thread::sleep_for(1s);
	}
	setPos();
	if (!Go) {
		Go = true;
	}
	StopStation.randPassager(direction);
	direction == 1 ? BaseStation.setNbpassagerDroite(StopStation.getNbpassagerDroite()) : BaseStation.setNbpassagerGauche(StopStation.getNbpassagerGauche());
	if ((direction == 1 ? StopStation.getNbpassagerDroite() > 0 : StopStation.getNbpassagerGauche() > 0)  && StopStation.getDepart() != 2) {
		auto n = 0;
		if (StopStation.getDepart() != 1) {
			n = rand() % (getNbpassager() + 1);
		}
		cout << "Nombre de personne qui sortent de la rame "<< getId() <<" : "<<  n << endl;
		for (auto i = 0; i < n; i++) {
			setNbpassager(getNbpassager() - 1);
			this_thread::sleep_for(0.5s);
		}
		if (direction == 1 ? StopStation.getNbpassagerDroite() >= (10 - getNbpassager()) : StopStation.getNbpassagerGauche() >= (10 - getNbpassager())) {
			n = 10 - getNbpassager();
			for (auto i = 0; i < n; i++) {
				setNbpassager(getNbpassager() + 1);
				direction == 1 ? BaseStation.setNbpassagerDroite(BaseStation.getNbpassagerDroite() - 1) : BaseStation.setNbpassagerGauche(BaseStation.getNbpassagerGauche() - 1);
				this_thread::sleep_for(0.5s);
			}
			direction == 1 ? StopStation.setNbpassagerDroite(StopStation.getNbpassagerDroite() - (10 - getNbpassager())) : StopStation.setNbpassagerGauche(StopStation.getNbpassagerGauche() - (10 - getNbpassager()));
			cout << "Nombre de personne qui rentrent dans la rame " << getId() << " : " << n << endl;
		}
		else {
			n = (direction == 1 ? StopStation.getNbpassagerDroite() : StopStation.getNbpassagerGauche());
			for (auto i = 0; i < n; i++) {
				setNbpassager(getNbpassager() + 1);
				direction == 1 ? BaseStation.setNbpassagerDroite(BaseStation.getNbpassagerDroite() - 1) : BaseStation.setNbpassagerGauche(BaseStation.getNbpassagerGauche() - 1);
				this_thread::sleep_for(0.5s);
			}
			cout << "Nombre de personne qui rentrent dans la rame " << getId() << " : " << n << endl;
			direction == 1 ? StopStation.setNbpassagerDroite(0) : StopStation.setNbpassagerGauche(0);
		}
	}
	else {
		if (StopStation.getDepart() == 2) {
			cout << "Nombre de personne qui sortent de la rame " << getId() << " : " << getNbpassager() << endl;
			for (auto i = 0; i < getNbpassager(); i++) {
				this_thread::sleep_for(0.25s);
				setNbpassager(getNbpassager() - 1);
				direction == 1 ? BaseStation.setNbpassagerDroite(BaseStation.getNbpassagerDroite() + 1) : BaseStation.setNbpassagerGauche(BaseStation.getNbpassagerGauche() + 1);
				this_thread::sleep_for(0.25s);
				direction == 1 ? BaseStation.setNbpassagerDroite(BaseStation.getNbpassagerDroite() - 1) : BaseStation.setNbpassagerGauche(BaseStation.getNbpassagerGauche() - 1);
			}
			setNbpassager(0);
			direction == 1 ? BaseStation.setNbpassagerDroite(0) : BaseStation.setNbpassagerGauche(0);
		}
	}
	this_thread::sleep_for(3s);
	StopStation.setEtatMA(true);
	cout << "Nombre de passager rame " << getId() << ": " << getNbpassager() << endl;
	cout << "Nombre de passager station "<< BaseStation.getNom() <<" : " << (direction == 1 ? BaseStation.getNbpassagerDroite() : BaseStation.getNbpassagerGauche()) << endl;
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
		compteLine++;
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
		setXpos((55 + (distanceLigne*1720/DISTANCELINE)));
		/*cout << x << endl;*/
		/*cout << v << endl;*/
		/*cout << distanceLigne << endl;*/
		setYpos(40.f);
	}
	else {
		setXpos(1860 - (distanceLigne * 1720 / DISTANCELINE));
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
bool Rame::getUrgenceAuto() const{
	return urgenceAuto;
}
void Rame::setUrgenceAuto(const bool& urgenceAuto_){
	urgenceAuto = urgenceAuto_;
}