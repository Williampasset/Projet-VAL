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
//Initialisation des passagers au départ --> à revoir
void passager(vector<Station>& station){
	for(auto i = 0; i<station.size(); i++){
		if(station.at(i).getDepart() != 2){
			auto n = rand() % 10 + 1;
			station.at(i).setNbpassager(n);
			cout<<"Nombre de passager station "<<station.at(i).getNom()<<" : "<<station.at(i).getNbpassager()<<endl;
		}
	}
}

void avancer(Rame& rame, Station& station, Superviseur Super) {
	auto distance = rame.getDistanceDArame(); //distance de la rame par rapport au départ, variable incrémenter pour calculer la distance en permanence
	auto distanceArret = distance; //distance de la rame à la station d'arrêt, i.e. la station précédente
	cout<<"Station:"<<station.getNom()<<endl;//affichage de la station suivante, la station où on va 
	while((rame.getDistanceDArame() < station.getDistanceDAstation() - 1) || rame.getDistanceDArame()>station.getDistanceDAstation() + 1){//On avance jusqu'à la station suivante ou on démarre si c'est la station de départ
		//Si on boucle et qu'on veut revenir au départ :
		if(rame.getDistanceDArame()>station.getDistanceDAstation()){
			station.setDistanceDA(distanceArret + 300);//on met la distance de la station de départ à 300m de la station d'arrêt
		}
		if(rame.distanceToNextRame(Super.getRame(rame.getId()-1)) < 300 && station.getDepart()==1){//Si on est à moins de 400m de la rame précédente, on s'arrête
			this_thread::sleep_for(100ms); 
			distance += 16.6 + (0.083)*(station.getDistanceDAstation() - 200 - distance);
			cout << "distance rame deceleration :  " << distance << endl;
			rame.setV((0.083)*(station.getDistanceDAstation() - 200 - distance));
		}
		//Si on est en urgence :
		// if(rame.getUrgence()){
		// 	auto distanceUrgence = rame.getDistanceDArame();//Distance de la rame quand on appuie sur le bouton d'urgence
		// 	distance += 1.66 + (4*0.083)*((distanceUrgence + 50) - distance);//On décelerre pour s'arrêter à 50m de la station d'arrêt
		// 	cout << "distance rame deceleration Urgence:  " << distance << endl;
		// 	rame.setV(16.6 + (4*0.083)*((distanceUrgence + 50) - distance));
		// }
		if (distance < 200) {//démarrage quand on est au départ 
			this_thread::sleep_for(100ms);
			distance += (0.083) * (distance);
			cout << "distance rame demarage :  " << distance << endl;
			rame.setV((0.083) * (distance));//Mauvais calcul de la vitesse
		}
		else if (distance <  distanceArret + 200) {//démarage quand on n'est pas au départ 
			this_thread::sleep_for(100ms);
			distance += (0.083) * (distance - distanceArret+1);
			cout << "distance rame demarage :  " << distance << endl;
			rame.setV((0.083) * (distance - distanceArret));
		}
		else if(station.getDistanceDAstation() - distance <= 200){//décélération normale 
			this_thread::sleep_for(100ms); 
			distance += 16.6 + (0.083)*(station.getDistanceDAstation() - 200 - distance);
			cout << "distance rame deceleration :  " << distance << endl;
			rame.setV((0.083)*(station.getDistanceDAstation() - 200 - distance));
		}
		else{
			this_thread::sleep_for(1s);
			distance += 16.6;
			cout << "distance rame constant :  " << distance << endl;
			rame.setV(16.6);
		}
		rame.setDistanceDA(distance);//Test pour voir si on peut set ce paramètre à la fin de tous les cas
	}
	cout<<"Arret station"<<endl;
	station.setEtatMA(false);
}

void arret(Rame& rame, Station& station, Superviseur Super) {
	if (station.getNbpassager() > 0) {
		auto n = 0;
		if (station.getDepart() != 1) {
			n = rand() % (rame.getNbpassager());
		}
		cout << "Nombre de personne qui sortent: " << n << endl;
		for (auto i = 0; i < n; i++) {
			this_thread::sleep_for(0.5s);
		}
		rame.setNbpassager(rame.getNbpassager() - n);
		if (station.getNbpassager() >= (10 - rame.getNbpassager())) {
			n = 10 - rame.getNbpassager();
			station.setNbpassager(station.getNbpassager() - (10 - rame.getNbpassager()));
			for (auto i = 0; i < n; i++) {
				this_thread::sleep_for(0.5s);
			}
			cout << "If" << endl;
			cout << "Nombre de personne qui rentrent: " << (10 - rame.getNbpassager()) << endl;
			rame.setNbpassager(10);
		}
		else {
			n = station.getNbpassager();
			rame.setNbpassager(rame.getNbpassager() + station.getNbpassager());
			for (auto i = 0; i < n; i++) {
				this_thread::sleep_for(0.5s);
			}
			cout << "Nombre de personne qui rentrent: " << station.getNbpassager() << endl;
			station.setNbpassager(0);
		}
	}
	else {
		if (station.getDepart() == 2) {
			cout << "Nombre de personne qui sortent: " << rame.getNbpassager() << endl;
			for (auto i = 0; i < rame.getNbpassager(); i++) {
				this_thread::sleep_for(0.5s);
			}
			rame.setNbpassager(0);
		}
	}
	while (rame.getUrgence()) {
		this_thread::sleep_for(3s);
	}
	if (rame.getId() != 1){
		while(Super.getRame(rame.getId()-1).getDistanceDArame() - rame.getDistanceDArame() < 400){
			cout<<Super.getRame(rame.getId()-1).getDistanceDArame()<<endl;
			this_thread::sleep_for(3s); 
		}
		if(rame.distanceToNextRame(Super.getRame(rame.getId()-1)) > 300){
			station.setEtatMA(true);
		}
	}
	else{
		station.setEtatMA(true);
	}
	this_thread::sleep_for(3s);
	
	cout<<"Nombre de passager rame "<<rame.getId()<<": "<< rame.getNbpassager()<<endl;
	cout<<"Nombre de passager station: "<<station.getNbpassager()<<endl;
}

void fonctionnement(stop_token stop_token, Rame& rame, vector<Station> station, Superviseur Super){
	int i = 0;
	while(!stop_token.stop_requested()){
		//cout<<station.at(i).getNom()<<endl;
		if(i == station.size() - 1){//condition pour la boucle avec les stations
			arret(ref(rame), station.at(i), Super);
			i = 0;
			avancer(ref(rame), station.at(i), Super);
			rame.setDistanceDA(1);
			station.at(i).setDistanceDA(0);
		}
		else{
			if(station.at(i).getEtatMA() && i<station.size() - 1){
				avancer(ref(rame), station.at(i + 1), Super);
				i++;
			}
			else{
				cout<<"Arret station dans boucle while"<<endl;
				arret(ref(rame), station.at(i), Super);
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
	passager(stations);
	stop_source s_source;
	
	/*rames.at(0).setDistanceDA(13);
	
	cout << "distance de la rame : " << rame0.getDistanceDArame() << endl;
	auto distance = Super.getRame(rames.at()).getDistanceDArame();
	cout <<"distance de la rame : " << distance << endl;*/

	jthread thr1(fonctionnement, s_source.get_token(), ref(rames.at(0)) ,stations, Super);
	jthread thr2(fonctionnement, s_source.get_token(), ref(rames.at(1)), stations, Super);
	
	

	return 0;
}
