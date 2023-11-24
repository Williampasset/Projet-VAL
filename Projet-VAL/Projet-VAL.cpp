// Projet-VAL.cpp : définit le point d'entrée de l'application.
//

#include "Projet-VAL.h"
#include "vector"
#include "algorithm"
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

void passager(vector<Station>& station){
	for(auto i = 0; i<station.size(); i++){
		if(station.at(i).getDepart() != 2){
			auto n = rand() % 10 + 1;
			station.at(i).setNbpassager(n);
			cout<<"Nombre de passager station "<<station.at(i).getNom()<<" : "<<station.at(i).getNbpassager()<<endl;
		}
	}
}

void avancer(Rame& rame, Station& station) {
	auto distance=rame.getDistanceDArame(); 
	auto distanceArret = rame.getDistanceDArame();
	cout<<"Station:"<<station.getNom()<<endl;
	while((rame.getDistanceDArame() < station.getDistanceDAstation() - 1) || rame.getDistanceDArame()>station.getDistanceDAstation() + 1){
		if(rame.getDistanceDArame()>station.getDistanceDAstation()){
			station.setDistanceDA(distanceArret + 300);
		}
		if (distance < 200) {
			std::this_thread::sleep_for(100ms);
			distance += (0.083) * (distance);
			rame.setDistanceDA(distance);
			std::cout << "distance rame demarage :  " << distance << std::endl;
			rame.setV((0.083) * (distance));
		}
		else if (distance <  distanceArret + 200) {
			std::this_thread::sleep_for(100ms);
			distance += (0.083) * (distance - distanceArret+1);
			rame.setDistanceDA(distance);
			std::cout << "distance rame demarage :  " << distance << std::endl;
			rame.setV((0.083) * (distance - distanceArret));
		}
		else if(station.getDistanceDAstation() - distance <= 200){
			std::this_thread::sleep_for(100ms);
			distance += 16.6 + (0.083)*(station.getDistanceDAstation() - 200 - distance);
			rame.setDistanceDA(distance);
			std::cout << "distance rame deceleration :  " << distance << std::endl;
			rame.setV(16.6 + (0.083)*(station.getDistanceDAstation() - 200 - distance));
		}
		else{
			std::this_thread::sleep_for(1s);
			distance += 16.6;
			rame.setDistanceDA(distance);
			std::cout << "distance rame constant :  " << distance << std::endl;
			rame.setV(16.6);
		}		
	}
	cout<<"Arrêt station"<<endl;
	station.setEtatMA(false);
}

void arret(Rame& rame, Station& station){
		if(station.getNbpassager()>0){
			auto n = 0;
			if(station.getDepart() != 1){
				n = rand() % (rame.getNbpassager());
			}
			cout<<"Nombre de personne qui sortent: "<<n<<endl;
			for(auto i = 0; i<n; i++){
				std::this_thread::sleep_for(0.5s);
			}
			rame.setNbpassager(rame.getNbpassager() - n);
			if(station.getNbpassager()>= (10 - rame.getNbpassager())){
				auto k = 10 - rame.getNbpassager();
				station.setNbpassager(station.getNbpassager() - (10 - rame.getNbpassager()));
				for(auto i = 0; i<k; i++){
					std::this_thread::sleep_for(0.5s);
				}
				cout<<"If"<<endl;
				cout<<"Nombre de personne qui rentrent: "<<(10 - rame.getNbpassager())<<endl;
				rame.setNbpassager(10);
			}
			else{
				auto j = station.getNbpassager();
				rame.setNbpassager(rame.getNbpassager() + station.getNbpassager());
				for(auto i = 0; i<j; i++){
					std::this_thread::sleep_for(0.5s);
				}
				cout<<"Nombre de personne qui rentrent: "<<station.getNbpassager()<<endl;
				station.setNbpassager(0);				
			}
		}
		else{
			if(station.getDepart() == 2){
				cout<<"Nombre de personne qui sortent: "<<rame.getNbpassager()<<endl;
				for(auto i = 0; i<rame.getNbpassager(); i++){
					std::this_thread::sleep_for(0.5s);
				}
				rame.setNbpassager(0);
			}
		}
		std::this_thread::sleep_for(3s);
		station.setEtatMA(true);
		cout<<"Nombre de passager rame: "<<rame.getNbpassager()<<endl;
		cout<<"Nombre de passager station: "<<station.getNbpassager()<<endl;
}

void fonctionnement(std::stop_token stop_token, Rame rame, vector<Station> station){
	int i = 0;
	while(!stop_token.stop_requested()){
		cout<<station.at(i).getNom()<<endl;
		if(station.at(i).getEtatMA()){
			avancer(rame, station.at(i + 1));
			i++;
		}
		else{
			arret(rame, station.at(i));
		}
		if(i == station.size() - 1){//condition pour la boucle avec les stations
			i = 0;
			avancer(rame, station.at(i));
			rame.setDistanceDA(1);
			station.at(i).setDistanceDA(0);
		}
	}
}

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	vector<Rame> rames;
	vector<Station> stations; 
	//création des rames : 
	Rame rame(1);
	//Rame rame1(2);
	//Rame rame2(3);
	rames.push_back(rame); 
	//rames.push_back(rame1);
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
	//implémentation des passagers par station au départ 
	passager(stations);
	std::stop_source s_source;
	// if(station1.getEtatMA()){
	// 	
	// }
	std::jthread thr1(fonctionnement, s_source.get_token(), rame, stations);
	// std::jthread thr2(arret,rame, station1);
	

	return 0;
}
