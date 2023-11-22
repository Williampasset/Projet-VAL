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
		auto n = rand() % 10 + 1;
		station.at(i).setNbpassager(n);
		cout<<"Nombre de passager station "<<station.at(i).getNom()<<" : "<<station.at(i).getNbpassager()<<endl;
	}
}

void avancer(Rame rame, Station station) {
	auto distance=rame.getDistanceDArame(); 
	cout<<"Station:"<<station.getNom()<<endl;
	while(rame.getDistanceDArame() < station.getDistanceDAstation() - 1){
		if (distance < 200) {
			std::this_thread::sleep_for(100ms);
			distance += (0.083) * distance;
			rame.setDistanceDA(distance);
			std::cout << "distance rame demarage :  " << distance << std::endl;
			rame.setV((0.083) * distance);
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
}

void arret(Rame rame, Station station){
		if(station.getNbpassager()>0){
			auto n = 0;
			if(station.getDistanceDAstation() != 0){
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
				rame.setNbpassager(10);
			}
			else{
				auto j = station.getNbpassager();
				rame.setNbpassager(rame.getNbpassager() + station.getNbpassager());
				for(auto i = 0; i<j; i++){
					std::this_thread::sleep_for(0.5s);
				}
				station.setNbpassager(0);
			}
		}
		std::this_thread::sleep_for(3s);
		cout<<"Nombre de passager rame: "<<rame.getNbpassager()<<endl;
		cout<<"Nombre de passager station: "<<station.getNbpassager()<<endl;
}

void fonctionnement(std::stop_token stop_token, Rame rame, vector<Station> station){
	int i = 0;
	while(!stop_token.stop_requested()){
		cout<<i<<endl;
		cout<<station.at(i).getNom()<<endl;
		if(station.at(i).getEtatMA()){
			cout<<"Test6"<<endl;
			avancer(rame, station.at(i + 1));
			station.at(i+1).setEtatMA(false);
			i++;
		}
		else{
			cout<<"Test7"<<endl;
			arret(rame, station.at(i));
			station.at(i).setEtatMA(true);
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
	Station station("Depart", 0); 
	Station station1("bois blanc", 500);
	Station station2("bois blanc", 1200);
	//Station station3("arrivee", 1700);
	stations.push_back(station);
	stations.push_back(station1);
	stations.push_back(station2);
	//stations.push_back(station3);
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
