﻿// Projet-VAL.cpp : définit le point d'entrée de l'application.
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
#include <vector>


#ifdef _MSC_VER 
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define _PATH_IMG_ "C:/Program Files/SFML/img/"
#else
#define _PATH_IMG_ "../img/"
#endif


const std::string path_image(_PATH_IMG_);


using namespace std;
using namespace sf;

void fonctionnement(stop_token stop_token, Rame& rame, vector<Station>& station) {
	vector<Station> NewStation(station);
	vector<Station>::iterator itStation = NewStation.begin();
	while (!stop_token.stop_requested()) {
		if ((itStation == NewStation.end() - 1 && rame.getDirection() == 1) || (itStation == NewStation.begin() && rame.getDirection() == -1)) {//condition pour la boucle avec les stations
			rame.Arreter(*itStation);
			NewStation.back().setDepart(NewStation.back().getDepart() != 1 ? 1 : 2);
			NewStation.front().setDepart(NewStation.front().getDepart() != 2 ? 2 : 1);
			rame.setDirection((-1) * rame.getDirection());
			itStation->setEtatMA(false);
			this_thread::sleep_for(5s);
		}
		else {
			if (itStation->getEtatMA()) {
				if (rame.getDirection() == 1) {
					rame.Avancer(*(itStation + 1));
					itStation++;
				}
				else {
					rame.Avancer(*(itStation - 1));
					cout << "--" << endl;
					itStation--;
				}
			}
			else {
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
	rames.reserve(2);//A MODIFIER SELON LE NOMBRE DE RAME-->Sinon bug sur les adresses
	//création des rames : 
	rames.emplace_back(1);
	cout << &rames[0] << endl;

	// Création de la deuxième rame en utilisant l'adresse de la première rame :
	rames.emplace_back(2, &rames[0]);
	cout << rames.at(1).getNextRame()->getId() << endl;
	//Rame rame2(3, &rame1);

	//création des stations : 
	Station station0("Bois Rouge", 0, 500, 500, 1);
	Station station1("Bois Blanc", 500, 500, 400, 0);
	Station station2("Republique", 900, 400, 400, 0);
	Station station3("Jeremy", 1300, 400, 400, 2);
	stations.push_back(station0);
	stations.push_back(station1);
	stations.push_back(station2);
	stations.push_back(station3);
	//Superviseur Super(rames); 
	//implémentation des passagers par station au départ 
	stop_source s_source;
	
	jthread thr1(fonctionnement, s_source.get_token(), ref(rames.at(0)) ,ref(stations));
	jthread thr2(fonctionnement, s_source.get_token(), ref(rames.at(1)), ref(stations));
	//jthread thr3(fonctionnement, s_source.get_token(), ref(rame2), ref(stations));

	
	 // Création de la fenêtre SFML
    sf::VideoMode fullscreenMode = sf::VideoMode::getFullscreenModes()[0];
    sf::RenderWindow window(fullscreenMode, "Objet suivant un trajet", sf::Style::Fullscreen);
    std::cout << "Taille de l'écran en plein écran : " << fullscreenMode.width << "x" << fullscreenMode.height << std::endl;


    // Fond d'écran
    Texture backgroundImage, objet;
    Sprite backgroundSprite;

    if (!backgroundImage.loadFromFile(path_image + "rail.png") || !objet.loadFromFile(path_image + "train.png")) {
        cerr << "Erreur pendant le chargement des images" << endl;
        return EXIT_FAILURE; // On ferme le programme
    }
    backgroundSprite.setTexture(backgroundImage);

	std::vector<sf::Sprite> ObjetSprite; // Assure-toi que tu as déclaré ObjetSprite en tant que vecteur de sprites

	for (auto& rame : rames) {
		sf::Sprite objetSprite; // Déclare l'objetSprite à l'intérieur de la boucle pour éviter des problèmes de portée
		objetSprite.setTexture(objet);
		objetSprite.setScale(sf::Vector2f(0.25f, 0.25f)); // Ajoute 'f' aux valeurs pour indiquer des constantes flottantes
		ObjetSprite.push_back(objetSprite); // Ajoute l'objetSprite au vecteur
	}

	/*Sprite objetSprite;
	objetSprite.setTexture(objet);
	objetSprite.setScale(sf::Vector2f(0.25, 0.25));
	ObjetSprite.push_back(ref(objetSprite));*/

	
	// Indice du point actuel sur le trajet
    size_t index = 0;

    // Boucle principale qui permet d'afficher le programme
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		window.clear();
		auto i = 0;
		for(auto& rame : rames){
			if(rame.getDirection() == 1){
				auto distanceRame = rame.getDistanceLigne()*(1820-100) / 1300;
				ObjetSprite.at(i).setPosition(100 + distanceRame, 50.f);
			}
			else{
				auto distanceRame = rame.getDistanceLigne()*(1820-100) / 1300;
				ObjetSprite.at(i).setPosition(1820 - distanceRame, 150.f);
			}
			window.draw(ObjetSprite.at(i));
			i++;
		}

        window.draw(backgroundSprite);
		for (auto& station : stations) {
			/*sf::Text text;
			text.setString(station.getNom());
			sf::Vector2f textpos(100 + (station.getDistanceDAstation()) * (1820 - 100) / 1300, 200.f);
			text.setPosition(textpos);
			window.draw(text);*/

			sf::CircleShape point(20.f);
			point.setFillColor(sf::Color::Red);
			sf::Vector2f pointCible(100 + (station.getDistanceDAstation())*(1820-100)/1300,100.f);
			point.setPosition(pointCible);
			window.draw(point);
		}
		//window.draw(point);
		//window.draw(point1);
		
        window.display();
    }

	return 0;
}
