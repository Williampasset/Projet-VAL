
#pragma once
#include "vector"
#include "algorithm"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
class Rame {
private:
	const int id = 0;
	int nbpassager = 0;
	double v = 0;//vitesse actuelle
	double distanceDA = 1;
	double distanceTotal = 1;
	bool urgence = false;
	//pas modifiable 
	float distanceAcc = 200;
	float distanceDec = 200;
	float distanceFreinage = 50;
	float distanceSecurite = 400;
	double vmax = 16.6;//vitesse max
	int nbpassagermax = 10;
public:
	Rame();
	Rame(const int& id_);
	~Rame();
	void setV(const double& v_);
	void setDistanceDA(const double& distanceDA_);
	void setNbpassager(const int& nbpassager_);
	int getId() const;
	int getNbpassager() const;
	int getnbpassagermax() const;
	double getV() const;
	double getDistanceDArame() const;
	bool getUrgence() const;
	void setUrgence(const bool& urgence_);
	double distanceToNextRame(const Rame& otherrame);
	void setDistanceTotal(const double& distance);
	double getDistanceTotal() const;
};