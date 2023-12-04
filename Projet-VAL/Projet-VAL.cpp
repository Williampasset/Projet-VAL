// Projet-VAL.cpp : définit le point d'entrée de l'application.
//

//#include "Projet-VAL.h"
#include <SFML/Graphics.hpp>
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
	vector<Station>::iterator it = station.begin();
	while (!stop_token.stop_requested()) {
		if ((it == station.end() && rame.getDirection() == 1) || (it == station.begin() && rame.getDirection() == -1)) {//condition pour la boucle avec les stations
			rame.Arreter(*it);
			rame.setDirection((-1) * rame.getDirection());
			this_thread::sleep_for(5s);
		}
		else {
			if (it->getEtatMA()) {
				if (rame.getDirection() == 1) {
					rame.Avancer(*(it + 1));
					cout << "++" << endl;
					it++;
				}
				else {
					rame.Avancer(*(it - 1));
					cout << "--" << endl;
					it--;
				}
			}
			else {
				cout << "Arret station dans boucle while" << endl;
				rame.Arreter(*it);
			}
		}
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
	Station station0("Bois Rouge", 0, 0, 1); 
	Station station1("Bois Blanc", 500, 500, 0);
	Station station2("Republique", 900, 400, 0);
	Station station3("Jeremy", 1300, 400, 2);
	/*Station station4("Départ 2", 1550, 1);
	Station station5("Republique 2", 2050, 0);
	Station station6("bois blanc 2", 2450, 0);
	Station station7("Terminus 2", 2850, 2);*/
	stations.push_back(station0);
	stations.push_back(station1);
	stations.push_back(station2);
	stations.push_back(station3);
	/*stations.push_back(station4);
	stations.push_back(station5);
	stations.push_back(station6);
	stations.push_back(station7);*/
	Superviseur Super(rames); 
	//implémentation des passagers par station au départ 
	for (auto& i : stations) i.randPassager();
	stop_source s_source;
	
	cout << stations.at(1).getNbpassager() << endl;
	jthread thr1(fonctionnement, s_source.get_token(), ref(rames.at(0)) ,stations);
	//jthread thr2(fonctionnement, s_source.get_token(), ref(rames.at(1)), stations);
	

	return 0;
}
