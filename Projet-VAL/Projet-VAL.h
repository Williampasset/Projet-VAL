#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

class Rame {
private:
	const int id = 0;
	int nbpassager = 0;
	double v = 0;//vitesse actuelle
	double distanceDA = 1;
	bool urgence = false;
	//pas modifiable 
	float distanceAcc = 200;
	float distanceDec = 200;
	float distanceFreinage = 50;
	float distanceSecurite = 400;
	double vmax = 16.6;//vitesse max
	int nbpassagermax = 10;
public:
	Rame(){
		std::cout<<"Nouvelle Rame"<<std::endl;
	}
	Rame(const int& id_) : id(id_){
		std::cout<<"Rame "<<id<<std::endl;
	}
	~Rame(){
		std::cout<<"Fin Rame"<<std::endl;
	}
	void setV(const double& v_){
		v = v_;
	}
	void setDistanceDA(const double& distanceDA_){
		distanceDA = distanceDA_;
	}
	void setNbpassager(const int& nbpassager_){
		nbpassager = nbpassager_;
	}
	int getId() const{
		return id;
	}
	int getNbpassager() const{
		return nbpassager;
	}
	int getnbpassagermax() const{
		return nbpassagermax;
	}
	double getV() const{
		return v;
	}
	double getDistanceDArame() const{
		return distanceDA;
	}
	bool getUrgence() const{
		return urgence;
	}
	void setUrgence(const bool& urgence_){
		urgence = urgence_;
	}
};

class Station {
	private : 
		int Nbpersonne = 0; 
		const std::string nom; 
		double DistanceDA=0; 
		bool etatMA = false;
		float Tempspassager = 0.5; //il faut l'ajouter à Tempsarretsec à chaque fois 
		float Tempsarretsec = 3; 
		int Depart = 0; //0 si non, 1 si oui, 2 si terminus de la ligne
	public:
		Station() {
			std::cout<<"Nouvelle Station"<<std::endl;
		}
		Station(const std::string& nom_, const float& DistanceDA_, const int& depart) : nom(nom_), DistanceDA(DistanceDA_){
			std::cout<<"Station "<<nom<<std::endl;
			Depart = depart;
		}
		~Station(){
			std::cout<<"Fin Station"<<std::endl;
		}
		void setNbpassager(const int& Nbpersonne_){
			Nbpersonne = Nbpersonne_;
		}
		void setEtatMA(const bool& etatMA_){
			etatMA = etatMA_;
		}
		std::string getNom() const{
			return nom;
		}
		int getNbpassager() const{
			return Nbpersonne;
		}
		double getDistanceDAstation() const{
			return DistanceDA;
		}
		bool getEtatMA() const{
			return etatMA;
		}
		float getTempspassager() const{
			return Tempspassager;
		}
		float getTempsarretsec() const{
			return Tempsarretsec;
		}
		int getDepart() const{
			return Depart;
		}
		void setDepart(const int& Depart_){
			Depart = Depart_;
		}
		void setDistanceDA(const double& DistanceDA_){
			DistanceDA = DistanceDA_;
		}
};

class Superviseur {
private:
	std::vector<Rame> rames;

public:
	Superviseur() {
		rames.push_back(Rame());
		std::cout << "Nouveau Superviseur" << std::endl;
	}
	Superviseur(const std::vector<Rame>& rames_) : rames(rames_) {
		std::cout << "Superviseur" << std::endl;
	}

	~Superviseur() {
		std::cout << "Fin Superviseur" << std::endl;
	}

	// void addRame(const Rame& rame) {
	// 	rames.push_back(rame);
	// }

	/*void removeRame(const Rame& rame) { Implémenter opérateur == dans la classe Rame
		rames.erase(std::remove(rames.begin(), rames.end(), rame), rames.end());
	}*/

	// int getNbRame() const {
	// 	return rames.size();
	// }

	// void setNbRame(const int& nb) {
	// 	rames.resize(nb);
	// }

	Rame getRame(const int& id) const {
		if (id >= 1 && id <= rames.size()) {
			return rames[id - 1];
		}
		else{
			return 0;
		}
	}

	void arretUrgence(const int& id) {
		if (id >= 0 && id < rames.size()) {
			rames[id].setUrgence(true);
		}
	}
};
