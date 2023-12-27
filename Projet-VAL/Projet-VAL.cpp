// Projet-VAL.cpp : définit le point d'entrée de l'application.
//

#include "Projet-VAL.h"
#include "Rame.h"
#include "Station.h"
#include "Superviseur.h"

Font font; // Définition de la police

const string path_font = _PATH_FONT_; // Définition du chemin de la police

void fonctionnement(stop_token stop_token, Rame& rame, vector<Station>& station) {//Gére le fonctionnement entier d'une rame
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
	rames.emplace_back(3, &rames[1]);

	//création des stations : 
	stations.emplace_back("Bois Rouge", 0, 400, 1);
	stations.emplace_back("Bois Blanc", 400, 400, 0);
	stations.emplace_back("Republique", 800, 400, 0);
	stations.emplace_back("Jeremy", 1200, 400, 2);
	//Superviseur Super(rames); 
	//implémentation des passagers par station au départ 
	stop_source s_source;
	
	jthread thr1(fonctionnement, s_source.get_token(), ref(rames.at(0)) ,ref(stations));
	jthread thr2(fonctionnement, s_source.get_token(), ref(rames.at(1)), ref(stations));
	jthread thr3(fonctionnement, s_source.get_token(), ref(rames.at(2)), ref(stations));

	
	 // Création de la fenêtre SFML
    VideoMode fullscreenMode = VideoMode::getFullscreenModes()[0];
    RenderWindow window(fullscreenMode, "Objet suivant un trajet", Style::Fullscreen);
    cout << "Taille de l'écran en plein écran : " << fullscreenMode.width << "x" << fullscreenMode.height << endl;

	//Pour la police d'écriture 
	// Charger la police ici
	if (!font.loadFromFile(path_font + "arial.ttf")) {
		cerr << "Error while loading font" << endl;
		return -1;
	}

    // Fond d'écran
    Texture backgroundImage, objet;
    Sprite backgroundSprite;

    if (/*!backgroundImage.loadFromFile(path_image + "rail.png") ||*/ !objet.loadFromFile(path_image + "train.png")) {
        cerr << "Erreur pendant le chargement des images" << endl;
        return EXIT_FAILURE; // On ferme le programme
    }
    //backgroundSprite.setTexture(backgroundImage);

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
		//Initialisation de la fenêtre d'affichage
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
		window.clear();

		auto i = 0;//Compteur
		//Affichage des rames
		for(auto& rame : rames){
			if (rame.getRotate()) {
				ObjetSprite.at(i).setScale(rame.getDirection() == 1 ? -(0.25) : 0.25, 0.25);
			}
			ObjetSprite.at(i).setPosition(rame.getXpos(), rame.getYpos());
			if (rame.getGo()) {
				window.draw(ObjetSprite.at(i));
			}
			i++;
		}

        //window.draw(backgroundSprite);
		for (auto& station : stations) {
			CircleShape point(15.f);
			point.setFillColor(Color::Red);
			Vector2f pointCible(100 + (station.getDistanceDAstation())*(1720)/1200,100.f);
			point.setPosition(pointCible);
			window.draw(point);

			Text stationName;
			stationName.setFont(font); // Utilisation de la police de caractères par défaut de SFML
			stationName.setCharacterSize(24);
			stationName.setFillColor(Color::White);
			stationName.setString(station.getNom());
			FloatRect textRect = stationName.getLocalBounds();
			stationName.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
			stationName.setPosition(pointCible.x, pointCible.y + point.getRadius() + 30);
			window.draw(stationName);
		}
		
        window.display();
    }

	return 0;
}
