// Projet-VAL.cpp : définit le point d'entrée de l'application.
//

//#include "Projet-VAL.h"
#include "Rame.h"
#include "Station.h"
#include "Superviseur.h"
#include "vector"
#include "algorithm"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

void fonctionnement(stop_token stop_token, Rame& rame, vector<Station> station) {
	int i = 0;
	while (!stop_token.stop_requested()) {
		//if (i == station.size() - 1) {//condition pour la boucle avec les stations
		//	i = 0;
		//	station.at(i).setDistanceDA(0);
		//}
		//else {
		if (station.at(i).getEtatMA() && i < station.size() - 1) {
			rame.Avancer(station.at(i+1));
			i++;
		}
		else {
			cout << "Arret station dans boucle while" << endl;
			rame.Arreter(station.at(i));
		}
		/*}*/
	}
}

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	//Initialisation des rames et des stations :
	vector<Rame> rames;
	vector<Station> stations; 
	//création des rames : 
	Rame rame0(1);
	Rame rame1(2);
	//Rame rame2(3);
	//Ajout des rames au vecteur de rames :
	rames.push_back(rame0); 
	rames.push_back(rame1);
	//rames.push_back(rame2);

	//création des stations : 
	Station station0("Depart1", 0, 1); 
	Station station1("bois blanc", 500, 0);
	Station station2("Republique", 900, 0);
	Station station3("Terminus 1", 1300, 2);
	Station station4("Départ 2", 1550, 1);
	Station station5("Republique 2", 2050, 0);
	Station station6("bois blanc 2", 2450, 0);
	Station station7("Terminus 2", 2850, 2);
	stations.push_back(station0);
	stations.push_back(station1);
	stations.push_back(station2);
	stations.push_back(station3);
	stations.push_back(station4);
	stations.push_back(station5);
	stations.push_back(station6);
	stations.push_back(station7);
	Superviseur Super(rames); 
	//implémentation des passagers par station au départ 
	for (auto& i : stations) i.randPassager();
	stop_source s_source;
	
	cout << stations.at(1).getNbpassager() << endl;
	jthread thr1(fonctionnement, s_source.get_token(), ref(rames.at(0)) ,stations);
	//jthread thr2(fonctionnement, s_source.get_token(), ref(rames.at(1)), stations, Super);
	

	return 0;
}
