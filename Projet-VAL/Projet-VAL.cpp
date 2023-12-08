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
	vector<Station> NewStation(station);
	vector<Station>::iterator itStation = NewStation.begin();
	while (!stop_token.stop_requested()) {
		if ((itStation == NewStation.end() - 1 && rame.getDirection() == 1) || (itStation == NewStation.begin() && rame.getDirection() == -1)) {//condition pour la boucle avec les stations
			rame.Arreter(*itStation);
			NewStation.back().setDepart(NewStation.back().getDepart() != 1 ? 1 : 2);
			cout << "Dernière station: " << NewStation.back().getDepart() << endl;
			NewStation.front().setDepart(NewStation.front().getDepart() != 2 ? 2 : 1);
			cout << "Première station: " << NewStation.front().getDepart() << endl;
			rame.setDirection((-1) * rame.getDirection());
			itStation->setEtatMA(false);
			cout << "Direction de la rame "<<rame.getId() << ":" << rame.getDirection() << endl;
			this_thread::sleep_for(5s);
		}
		else {
			if (itStation->getEtatMA()) {
				if (rame.getDirection() == 1) {
					rame.Avancer(*(itStation + 1));
					cout << "++" << endl;
					itStation++;
				}
				else {
					rame.Avancer(*(itStation - 1));
					cout << "--" << endl;
					itStation--;
				}
			}
			else {
				cout << "Arret station dans boucle while" << endl;
				rame.Arreter(*itStation);
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
	Rame rame1(2, &rame0);
    Rame rame2(3, &rame1);
	//Ajout des rames au vecteur de rames :
	rames.push_back(ref(rame0)); 
	rames.push_back(ref(rame1));
	rames.push_back(rame2);

	//création des stations : 
	Station station0("Bois Rouge", 0, 500, 500, 1);
	Station station1("Bois Blanc", 500, 500, 400, 0);
	Station station2("Republique", 900, 400, 400, 0);
	Station station3("Jeremy", 1300, 400, 400, 2);
	stations.push_back(station0);
	stations.push_back(station1);
	stations.push_back(station2);
	stations.push_back(station3);
	Superviseur Super(rames); 
	//implémentation des passagers par station au départ 
	for (auto& i : stations) i.randPassager();
	stop_source s_source;
	
	jthread thr1(fonctionnement, s_source.get_token(), ref(rame0) ,stations);
	jthread thr2(fonctionnement, s_source.get_token(), ref(rame1), stations);
	jthread thr3(fonctionnement, s_source.get_token(), ref(rame2), stations);

	return 0;
}
