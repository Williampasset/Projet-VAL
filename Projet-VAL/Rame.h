
#pragma once
#include "Projet-VAL.h"
#include "Station.h"
class Rame {
private:
	const int id = 0;//ID de la rame pour identifier chaque rame
	int nbpassager = 0;//Nombre de passager de chaque rame
	float v = 0;//vitesse actuelle
	float distanceOldStation = 1;//Distance depuis l'ancienne station
	float distanceTotal = 0;//Distance totale depuis le lancement de la rame
	bool urgence = false;//Indication d'arrêt d'urgence
	int direction = 1;//1 si sens aller, -1 si sens retour
	float distanceLigne = 0;//Distance dans le sens aller ou le sens retour
	Rame* NextRame = nullptr;//Adresse de la rame précédente pour gérer les distances de sécurité
	bool Go = false;//Indique si la rame est parti ou non --> principalement pour l'affichage au départ
	float x = 0;//Position en pixels
	float y = 0;
	bool rotate = false;

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
	bool getGo();
	float getXpos() const;
	float getYpos() const;
	void setXpos(const float& newX);
	void setYpos(const float& newY);
	void setPos();
	void Rotate(const double& angle);
	bool getRotate() const;
	void setNextRame(Rame* NextRame_);
};
