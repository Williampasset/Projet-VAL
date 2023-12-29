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
		int NbpersonneDroite = 0;
		int NbpersonneGauche = 0;
		string nom;
		float DistanceDA=0;
		float distanceBefStation = 0;
		bool etatMA = false;
		int Depart = 0; //0 si non, 1 si oui, 2 si terminus de la ligne
		//non modifiable
		float Tempspassager = 0.5; //il faut l'ajouter à Tempsarretsec à chaque fois 
		float Tempsarretsec = 3;
	public:
		Station();
		Station(const string& nom_, const float& DistanceDA_, const float& distanceBefStation_, const int& depart);
		Station(const Station& station);
		~Station();
		void setNbpassagerDroite(const int& Nbpersonne_);
		void setEtatMA(const bool& etatMA_);
		string getNom() const;
		int getNbpassagerDroite() const;
		float getDistanceDAstation() const;
		bool getEtatMA() const;
		float getTempspassager() const;
		float getTempsarretsec() const;
		int getDepart() const;
		void setDepart(const int& Depart_);
		void setDistanceDA(const float& DistanceDA_);
		void randPassager(const int& direction_);
		void setDistanceBefStation(const float& distanceBefStation_);
		float getDistanceBefStation() const;
		void copyStation(Station& OtherStation);
		void setNbpassagerGauche(const int& Nbpersonne_);
		int getNbpassagerGauche() const;
};