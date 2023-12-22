
#pragma once
#include "Projet-VAL.h"
#include "Station.h"
class Rame {
private:
	const int id = 0;
	int nbpassager = 0;
	float v = 0;//vitesse actuelle
	float distanceOldStation = 1;
	float distanceTotal = 0;
	bool urgence = false;
	int direction = 1;
	float distanceLigne = 0;
	Rame* NextRame = nullptr;
	//pas modifiable 
public:
	Rame();
	Rame(const int& id_);
	Rame(const int& id_, Rame* NextRame_);
	~Rame();
	void setV(const double& v_);
	void setDistanceOldStation(const double& distanceDA_);
	void setNbpassager(const int& nbpassager_);
	int getId() const;
	int getNbpassager() const;
	int getnbpassagermax() const;
	double getV() const;
	double getDistanceOldStation() const;
	bool getUrgence() const;
	void setUrgence(const bool& urgence_);
	double distanceToNextRame();
	void setDistanceTotal(const double& distance);
	double getDistanceTotal() const;
    void Avancer(Station& NextStation);
    void Arreter(Station& StopStation);
	void setDirection(const int& direction_);
	int getDirection();
	void setDistanceLigne(const float& distanceLigne_);
	float getDistanceLigne();
	Rame* getNextRame();
};
