#pragma once

#include "vector"
#include "algorithm"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
using namespace std;
class Station {
	private : 
		int Nbpersonne = 0; 
		const string nom; 
		double DistanceDA=0; 
		bool etatMA = false;
		float Tempspassager = 0.5; //il faut l'ajouter à Tempsarretsec à chaque fois 
		float Tempsarretsec = 3; 
		int Depart = 0; //0 si non, 1 si oui, 2 si terminus de la ligne
	public:
		Station();
		Station(const std::string& nom_, const float& DistanceDA_, const int& depart);
		~Station();
		void setNbpassager(const int& Nbpersonne_);
		void setEtatMA(const bool& etatMA_);
		std::string getNom() const;
		int getNbpassager() const;
		double getDistanceDAstation() const;
		bool getEtatMA() const;
		float getTempspassager() const;
		float getTempsarretsec() const;
		int getDepart() const;
		void setDepart(const int& Depart_);
		void setDistanceDA(const double& DistanceDA_);
		void randPassager();
};