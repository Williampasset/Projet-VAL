// Projet-VAL.cpp : définit le point d'entrée de l'application.
//

#include "Projet-VAL.h"
#include "Rame.h"
#include "Station.h"
#include "Superviseur.h"

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
	rames.reserve(RAMENOMBER);//A MODIFIER SELON LE NOMBRE DE RAME-->Sinon bug sur les adresses
	//création des rames : 
	rames.emplace_back(1);
	rames.emplace_back(2, &rames[0]);
	//rames.emplace_back(3, &rames[1]);

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
    VideoMode fullscreenMode = VideoMode::getFullscreenModes()[0];
    RenderWindow window(fullscreenMode, "Objet suivant un trajet", Style::Fullscreen);
    cout << "Taille de l'écran en plein écran : " << fullscreenMode.width << "x" << fullscreenMode.height << endl;


    // Fond d'écran
    Texture backgroundImage, objet;
    Sprite backgroundSprite;

    if (!backgroundImage.loadFromFile(path_image + "rail.png") || !objet.loadFromFile(path_image + "train.png")) {
        cerr << "Erreur pendant le chargement des images" << endl;
        return EXIT_FAILURE; // On ferme le programme
    }
    backgroundSprite.setTexture(backgroundImage);

	vector<Sprite> ObjetSprite; // Assure-toi que tu as déclaré ObjetSprite en tant que vecteur de sprites

	for (auto& rame : rames) {
		Sprite objetSprite; // Déclare l'objetSprite à l'intérieur de la boucle pour éviter des problèmes de portée
		objetSprite.setTexture(objet);
		objetSprite.setScale(Vector2f(0.25f, 0.25f)); // Ajoute 'f' aux valeurs pour indiquer des constantes flottantes
		ObjetSprite.push_back(objetSprite); // Ajoute l'objetSprite au vecteur
	}

	// Indice du point actuel sur le trajet
    size_t index = 0;

    // Boucle principale qui permet d'afficher le programme
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
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
			CircleShape point(20.f);
			point.setFillColor(Color::Red);
			Vector2f pointCible(100 + (station.getDistanceDAstation())*(1820-100)/1300,100.f);
			point.setPosition(pointCible);
			window.draw(point);
		}
		
        window.display();
    }

	return 0;
}
