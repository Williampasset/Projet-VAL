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
		string nom;
		double DistanceDA=0;
		double distanceBefStation = 0;
		double distanceAftStation = 0;
		bool etatMA = false;
		int Depart = 0; //0 si non, 1 si oui, 2 si terminus de la ligne
		//non modifiable
		float Tempspassager = 0.5; //il faut l'ajouter à Tempsarretsec à chaque fois 
		float Tempsarretsec = 3;
	public:
		Station();
		Station(const string& nom_, const float& DistanceDA_, const double& distanceBefStation_, const double& distanceAftStation_, const int& depart);
		Station(const Station& station);
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
		void setDistanceBefStation(const double& distanceBefStation_);
		double getDistanceBefStation() const;
		void setDistanceAftStation(const double& distanceAftStation_);
		double getDistanceAftStation() const;
		void copyStation(Station& OtherStation);
};