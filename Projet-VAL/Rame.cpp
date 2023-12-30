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
		return NextRame->getDistanceLigne() - distanceLigne + DISTANCELINE;
	}
	else if ((compteLine + NextRame->compteLine) % 2 == 0 && compteLine > 0) {
		return abs(NextRame->getDistanceLigne() - distanceLigne);
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
	cout << "Station suivante: " << nextStation.getNom() << "\tRame: " << getId() << endl;//affichage de la station suivante, la station où on va 
	float time = 0;//Calcul le temps en s
	float distanceTotActuel = getDistanceTotal();//Distance total parcouru par la rame
	float distanceLigneActuel = getDistanceLigne();//Distance parcouru par la rame sur une ligne dans un sens
	float distanceacc = 0;//Distance parcourue lors de l'acceleration de la rame
	float distanceconst = 0;//Distance parcourue lorsque la rame est à vitesse constante
	float distancedec = 0;//Distance parcourue lorsque la rame décelerre
	float distanceUrgence = 0;//Distance parcourue lorsque l'état d'urgence est enclenché et que la rame s'arrête
	float vitesse1 = 0;//Vitesse à stocker lorsqu'on passe de l'état d'acceleration à l'état de déceleration
	float vitesse2 = 0;//Vitesse à stocker lorsqu'on passe de l'état deleceration à l'état d'acceleration
	float stopDistance = (VMAX * VMAX) / (2 * ACC);//Distance à laquelle la rame doit décelerer et sur laquelle elle accelere
	bool Dec = false;//Indique si la rame est en train de décelerer
	while (abs(distance - (nextStation.getDistanceBefStation())) >= 1 || (nextStation.getDepart() == 1 && abs(distance - (nextStation.getDistanceDAstation())) >= 1)) {
		this_thread::sleep_for(100ms);//Pause de 100ms entre chaque itération
		time += 0.1;//On rajoute au temps 0.1s 
		if (distanceToNextRame() < SECURDISTANCE && NextRame->getGo() && !urgence && Dec == false) {//Si ces conditions sont respectés c'est que la rame est trop proche de la rame de devant et qu'elle doit décelerer pour reprendre une distance de sécurité suffisante
			urgence = true;
			urgenceAuto = true;
		}
		else if (urgenceAuto && distanceToNextRame() > SECURDISTANCE) {//Sinon si elle est en urgenceAuto et que la distance est suffisante on desactive l'état d'urgence
			urgence = false;
			urgenceAuto = false;
		}
		if (getV() < VMAX && (nextStation.getDistanceBefStation()) - distance > stopDistance && !urgence) {//condition d'acceleration
			if (Dec) {
				distanceacc = 0;
				distanceconst = 0;
			}
			Dec = false;//On indique que la rame n'est pas en déceleration
			if (distanceacc == 0) {//Si la distance d'acceleration est nulle
				time = 0.1;//On initialise le temps à 0.1
			}
			if (distancedec != 0) {
				setV(vitesse2 + (1.4) * (time));//On modifie la vitesse en fonction du temps
			}
			else {
				setV((1.4) * (time));//On modifie la vitesse en fonction du temps
			}
			distanceacc = (((ACC) * (time * time) * 0.5) + distancedec + distanceconst + distanceUrgence + vitesse2*time);//On calcule la distance d'acceleration en prenant en compte les possibles distances initiales
			distance = distanceacc;//On met la variable de distance à la distance calculer
			vitesse1 = getV();//On stock la vitesse en cas de changement d'état en deceleration
			if (nextStation.getDistanceBefStation() - distance < 2 * stopDistance && stopDistance == (VMAX * VMAX) / (2 * ACC)) {//On recalcule la distance d'arrêt si on est déjà trop proche de la prochaine station
				stopDistance = (nextStation.getDistanceBefStation() - distance) / 2;//distance pour accelerer et pour decelerer
				cout << "STOP DISTANCE rame " << getId() << " : " << stopDistance << endl;
			}
		}
		else if (((((nextStation.getDistanceBefStation()) - distance < stopDistance) || (urgence && getV() > 1))) && getV() > 0) {//décélération normale
			if (!Dec) {
				distancedec = 0;
			}
			if (distancedec == 0) {//De même que pour l'acceleration
				distanceUrgence = 0;//On repasse la distance d'urgence à 0
				time = 0.1;
			}
			if (getV() < VMAX && distanceconst == 0) {//Si la vitesse est inférieure à la vitesse maximale et que la distance en vitesse constante est nulle
				cout << time << endl;
				distancedec = ((vitesse1 * time - ((vitesse1 * ACC / VMAX) * time * time * 0.5)) + distanceacc);//on vient de passer de l'acc à la dec et donc on doit faire varier la vitesse en partant de la vitesse maximale atteinte en acc
				distance = distancedec;//On modifie la distance
				setV(vitesse1 - (ACC * time));//On change la vitesse
				cout << getId() << "    " << vitesse1 << "      "<<distance<<endl;
			}
			else {//Sinon ça veut dire qu'on pas de l'état vitesse const à la dec
				distancedec = ((VMAX * time - (ACC * (time * time) * 0.5)) + distanceconst);//Donc on décelere en partant de la vitesse max autorisé 
				distance = distancedec;//On modifie la distance
				setV(VMAX - (ACC * time));//Et la vitesse
			}
			vitesse2 = getV();
			Dec = true;//On passe dans l'état de deceleration si on entre en urgence ou que la distance entre la rame et la station atteint la distance d'arrêt
		}
		else if (urgence && (getV() <= 1)) {//Sinon si on est en urgence et que la vitesse est inférieure à 1
			setV(0);//On arrête la rame et on remet toutes les distances à 0 en stockant la distance à laquelle on s'est arrêté
			distanceacc = 0;
			distanceconst = 0;
			distancedec = 0;
			distanceUrgence = distance;//Stockage de la distance où on s'arrête
			if (nextStation.getDistanceBefStation() - distance < stopDistance) {//Si besoin on recalcule la distance d'arrêt dans la cas où on s'arrête proche de la station 
				stopDistance = (nextStation.getDistanceBefStation() - distance) / 2;
				cout << "STOP DISTANCE urgence rame " << getId() << " : " << stopDistance << endl;
			}
		}
		else if (distanceacc != 0 && !urgence) {//Vitesse constante
			Dec = false;
			if (distanceconst == 0) {
				distanceUrgence = 0;
				distancedec = 0;
				time = 0.1;
			}
			distanceconst = ((VMAX * time) + distanceacc + distancedec);
			distance = distanceconst;
			setV(VMAX);
		}
		//On set toutes les distances et la position de la rame au bon endroit
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