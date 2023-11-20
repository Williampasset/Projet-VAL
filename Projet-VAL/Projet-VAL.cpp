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

//template <typename T>
void passager(Station& objet){
	auto n = rand() % 10;
	objet.setNbpassager(n);
	cout<<"Nombre de passager station: "<<objet.getNbpassager()<<endl;
}
void passager2(Rame& objet){
	auto n = rand() % 10 + 1;
	objet.setNbpassager(n);
	cout<<"Nombre de passager rame: "<<objet.getNbpassager()<<endl;
}
void avancer(std::stop_token stop_token, Rame rame, Station station) {
	while (!stop_token.stop_requested()) {
		auto distance=rame.getDistanceDArame(); 
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
			if(station.getDistanceDAstation() - distance < 1){
				station.setEtatMA(0);
			}
		}
		else{
			std::this_thread::sleep_for(1s);
			distance += 16.6;
			rame.setDistanceDA(distance);
			std::cout << "distance rame constant :  " << distance << std::endl;
			rame.setV(16.6);
		}		
	}
}

void arret(std::stop_token stop_token, Rame rame, Station station){
	//srand (time(NULL));
		if(station.getNbpassager()>0){
			cout<<"Test"<<endl;
			auto n = rand() % (rame.getNbpassager());
			cout<<"Nombre de personne qui sortent: "<<n<<endl;
			for(auto i = 0; i<n; i++){
				std::this_thread::sleep_for(0.5s);
			}
			rame.setNbpassager(rame.getNbpassager() - n);
			if(station.getNbpassager()>= (10 - rame.getNbpassager())){
				cout<<"Test2"<<endl;
				auto k = 10 - rame.getNbpassager();
				station.setNbpassager(station.getNbpassager() - (10 - rame.getNbpassager()));
				for(auto i = 0; i<k; i++){
					std::this_thread::sleep_for(0.5s);
				}
				rame.setNbpassager(10);
			}
			else{
				cout<<"Test3"<<endl;
				auto j = station.getNbpassager();
				rame.setNbpassager(rame.getNbpassager() + station.getNbpassager());
				for(auto i = 0; i<j; i++){
					std::this_thread::sleep_for(0.5s);
				}
				station.setNbpassager(0);
			}
		}
		cout<<"Test4"<<endl;
		std::this_thread::sleep_for(3s);
		station.setEtatMA(1);
		cout<<"Nombre de passager rame: "<<rame.getNbpassager()<<endl;
		cout<<"Nombre de passager station: "<<station.getNbpassager()<<endl;
}

void fonctionnement(std::stop_token stop_token, Rame rame, vector<Station> station){
	int i = 0;
	while(i!=2){
		if(station.at(i).getEtatMA()){
			avancer(stop_token, rame, station.at(i + 1));
		}
		else{
			arret(stop_token, rame, station.at(i + 1));
		}
		i++;
	}
	
}

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	//vector<Rame> rames;
	vector<Station> stations; 
	//création des rames : 
	Rame rame(1);
	//Rame rame1(2);
	//Rame rame2(3);
	//rames.push_back(rame); 
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
	passager(station1);
	passager2(rame);
	std::stop_source s_source;
	// if(station1.getEtatMA()){
	// 	
	// }
	std::jthread thr1(fonctionnement, s_source.get_token(), rame, stations);
	// std::jthread thr2(arret,rame, station1);
	

	return 0;
}
