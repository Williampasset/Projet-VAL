#include "Projet-VAL.h"
#include "Rame.h"
#include "Station.h"
#include "Superviseur.h"


Font font; // Définition de la police

const string path_font = _PATH_FONT_; // Définition du chemin de la police



void fonctionnement(stop_token stop_token, Rame& rame, vector<Station>& station) {//Gére le fonctionnement entier d'une rame
	//Copie du vecteur station initiale pour chaque rame 
	vector<Station> NewStation(station);
	vector<Station>::iterator itStation = NewStation.begin();
	vector<Station>::iterator itStationBase = station.begin();

	while (!stop_token.stop_requested()) {
		for(auto i = 0; i<NewStation.size();i++){
			NewStation.at(i).setNbpassagerDroite(station.at(i).getNbpassagerDroite());
			NewStation.at(i).setNbpassagerGauche(station.at(i).getNbpassagerGauche());
		}
		if ((itStation == NewStation.end() - 1 && rame.getDirection() == 1) || (itStation == NewStation.begin() && rame.getDirection() == -1)) {//condition pour la boucle avec les stations
			//Arrêt à la station
			rame.Arreter(*itStation, *itStationBase);
			//Echange des propriètés de départ et d'arrivé car changement de sens
			NewStation.back().setDepart(NewStation.back().getDepart() != 1 ? 1 : 2);
			NewStation.front().setDepart(NewStation.front().getDepart() != 2 ? 2 : 1);
			//Changement de l'indice de direction de la rame
			rame.setDirection((-1) * rame.getDirection());
			//Inversion des distances pour chaque station
			for (int i = 0; i < (NewStation.size() / 2); i++) {
				// Sauvegarde temporaire de la valeur à la position i
				float temp = NewStation.at(i).getDistanceBefStation();
				float temp2 = NewStation.at(i).getDistanceDAstation();
				// Copie de la valeur de la position opposée à la position i
				NewStation.at(i).setDistanceBefStation(NewStation.at(NewStation.size() - 1 - i).getDistanceBefStation());
				NewStation.at(i).setDistanceDA(NewStation.at(NewStation.size() - 1 - i).getDistanceDAstation());
				// Copie de la valeur temporaire à la position opposée à la position i
				NewStation.at(NewStation.size() - 1 - i).setDistanceBefStation(temp);
				NewStation.at(NewStation.size() - 1 - i).setDistanceDA(temp2);
			}
			itStation->setEtatMA(false);
			this_thread::sleep_for(5s);
		}
		else {
			if (itStation->getEtatMA()) {
				if (rame.getDirection() == 1) {
					rame.Avancer(*(itStation + 1));
					itStation++;
					itStationBase++;
				}
				else{
					rame.Avancer(*(itStation - 1));
					itStation--;
					itStationBase--;
				}
			}
			else{
				rame.Arreter(*itStation, *itStationBase);
			}
		}
	}
}

//Pour le tableau : 
struct Row {
	string name;
	RectangleShape button;
};

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
	rames.at(0).setNextRame(&rames[2]);
	
	//création des stations : 
	stations.emplace_back("Bois Rouge", 0, 600, 1);
	stations.emplace_back("Bois Blanc", 600, 600, 0);
	stations.emplace_back("Republique", 1400, 800, 0);
	stations.emplace_back("Jeremy", DISTANCELINE, 600, 2);
	
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
	// Pour le tableau 

	vector<Row> rows{
	   {"Rame 1", RectangleShape(Vector2f(50, 20))},
	   {"Rame 2", RectangleShape(Vector2f(50, 20))},
	   {"Rame 3", RectangleShape(Vector2f(50, 20))}
	  
	};
	
	Vector2f tablePosition(100.f, 800.f);

	Vector2f tableStation(1200.f, 800.f);
	
	float rowSpacing = 30.f;
	
	// Position et taille des boutons
	float buttonWidth = 50.f;
	float buttonHeight = 20.f;
	

    // Boucle principale qui permet d'afficher le programme
    while (window.isOpen()) {
		//Initialisation de la fenêtre d'affichage
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
			if (event.type == Event::MouseButtonPressed) {
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
				for (size_t i = 0; i < rows.size(); ++i) {
					if (rows[i].button.getGlobalBounds().contains(mousePos) && rames.at(i).getGo() == true) {
						cout << "Bouton de la rame " << rows[i].name << " cliqué !" << endl;
						if (rames.at(i).getUrgence() == 1 && rames.at(i).getUrgenceAuto() == 0) {
							rames.at(i).setUrgence(0);
							cout << "urgence " << rames.at(i).getUrgence() << endl;
						}
						else {
							rames.at(i).setUrgence(1);
							rames.at(i).setUrgenceAuto(0);
							cout << "urgence " << rames.at(i).getUrgence() << endl;
						}					
						
					}
				}
			}
        }

		window.clear();

		auto i = 0;//Compteur

		//Affichage des rames
		for(auto& rame : rames){
			if (rame.getRotate()) {
				ObjetSprite.at(i).setScale(rame.getDirection() == 1 ? -(0.25f) : 0.25f, 0.25f);
			}
			ObjetSprite.at(i).setPosition(rame.getXpos(), rame.getYpos());
			if (rame.getGo()) {
				window.draw(ObjetSprite.at(i));
			}
			i++;
		}
		//affichage des stations 
		for (auto& station : stations) {
			CircleShape point(15.f);
			point.setFillColor(Color::Red);
			Vector2f pointCible(100 + (station.getDistanceDAstation()) * (1720) / DISTANCELINE, 100.f);
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

		//affichage des titres 
		Text TitreRame;
		TitreRame.setFont(font);
		TitreRame.setString("Informations Rames :");
		TitreRame.setCharacterSize(30);
		TitreRame.setFillColor(Color::White);
		TitreRame.setPosition(tablePosition.x, tablePosition.y -80.f);
		window.draw(TitreRame);
		Text Titre0;
		Titre0.setFont(font);
		Titre0.setString("Noms des Rames :");
		Titre0.setCharacterSize(20);
		Titre0.setFillColor(Color::White);
		Titre0.setPosition(tablePosition.x, tablePosition.y - 40.f);
		window.draw(Titre0);
		Text Titre1;
		Titre1.setFont(font);
		Titre1.setString("Vitesse :");
		Titre1.setCharacterSize(20);
		Titre1.setFillColor(Color::White);
		Titre1.setPosition(tablePosition.x + 200.f, tablePosition.y - 40.f);
		window.draw(Titre1);
		Text Titre2;
		Titre2.setFont(font);
		Titre2.setString("Distance :");
		Titre2.setCharacterSize(20);
		Titre2.setFillColor(Color::White);
		Titre2.setPosition(tablePosition.x + 300.f, tablePosition.y - 40.f);
		window.draw(Titre2);
		Text Titre3;
		Titre3.setFont(font);
		Titre3.setString("Nombre de passagers :");
		Titre3.setCharacterSize(20);
		Titre3.setFillColor(Color::White);
		Titre3.setPosition(tablePosition.x + 450.f, tablePosition.y - 40.f);
		window.draw(Titre3);
		Text Titre4;
		Titre4.setFont(font);
		Titre4.setString("Superviseur :");
		Titre4.setCharacterSize(20);
		Titre4.setFillColor(Color::White);
		Titre4.setPosition(tablePosition.x + 700.f, tablePosition.y - 40.f);
		window.draw(Titre4);

		//Pour les titres des stations 
		Text TitreStations;
		TitreStations.setFont(font);
		TitreStations.setString("Informations Stations :");
		TitreStations.setCharacterSize(30);
		TitreStations.setFillColor(Color::White);
		TitreStations.setPosition(tableStation.x, tableStation.y - 80.f);
		window.draw(TitreStations);

		Text TitreStations1;
		TitreStations1.setFont(font);
		TitreStations1.setString("Noms des stations :");
		TitreStations1.setCharacterSize(16);
		TitreStations1.setFillColor(Color::White);
		TitreStations1.setPosition(tableStation.x, tableStation.y - 40.f);
		window.draw(TitreStations1);

		Text TitreStations2;
		TitreStations2.setFont(font);
		TitreStations2.setString("Nombres passagers sens aller -> :");
		TitreStations2.setCharacterSize(16);
		TitreStations2.setFillColor(Color::White);
		TitreStations2.setPosition(tableStation.x+150.f, tableStation.y - 40.f);
		window.draw(TitreStations2);

		Text TitreStations3;
		TitreStations3.setFont(font);
		TitreStations3.setString("Nombres passagers sens retour <- :");
		TitreStations3.setCharacterSize(16);
		TitreStations3.setFillColor(Color::White);
		TitreStations3.setPosition(tableStation.x+ 400.f, tableStation.y - 40.f);
		window.draw(TitreStations3);
	

		//Pour le tableau : 
		for (size_t i = 0; i < rows.size(); ++i) {
			//nom de la rame 
			Text text;
			text.setFont(font);
			text.setString(rows[i].name);
			text.setCharacterSize(16);
			text.setFillColor(Color::White);
			text.setPosition(tablePosition.x, tablePosition.y + i * rowSpacing);
			window.draw(text);
			//vitesse  de la rame 
			Text text2;
			text2.setFont(font);
			float vitesserame = rames.at(i).getV();
			text2.setString(to_string(vitesserame));
			text2.setCharacterSize(16);
			text2.setFillColor(Color::White);
			text2.setPosition(tablePosition.x + 200.f, tablePosition.y + i * rowSpacing);
			window.draw(text2);

			//distance de la rame 
			Text text3;
			text3.setFont(font);
			float distancerame = rames.at(i).getDistanceTotal();
			text3.setString(to_string(distancerame));
			text3.setCharacterSize(16);
			text3.setFillColor(Color::White);
			text3.setPosition(tablePosition.x + 300.f, tablePosition.y + i * rowSpacing);
			window.draw(text3);
			//nombre de passager 
			Text text4;
			text4.setFont(font);
			int nbrpassager = rames.at(i).getNbpassager();
			text4.setString(to_string(nbrpassager));
			text4.setCharacterSize(16);
			text4.setFillColor(Color::White);
			text4.setPosition(tablePosition.x + 450.f, tablePosition.y + i * rowSpacing);
			window.draw(text4);

			//bouton arrêt urgence 
			rows[i].button.setSize(Vector2f(buttonWidth, buttonHeight));
			rows[i].button.setPosition(tablePosition.x + 700.f, tablePosition.y + i * rowSpacing);
			if (rames.at(i).getGo() == true) {
				if (rames.at(i).getUrgence() == true && rames.at(i).getUrgenceAuto() == false) {
					rows[i].button.setFillColor(Color::Red);
				}
				else if (rames.at(i).getUrgence() == true && rames.at(i).getUrgenceAuto() == true) {
					rows[i].button.setFillColor(Color::Yellow);
				}
				else {
					rows[i].button.setFillColor(Color::Green);
				}
			}
			else {
				rows[i].button.setFillColor(Color::White);
			}
			
			
			window.draw(rows[i].button);
		}
		//pour les Stations 
		for (int e = 0; e < stations.size();e++) {
			Text nomstations;
			nomstations.setFont(font);
			auto nomstat = stations.at(e).getNom();
			nomstations.setString(nomstat);
			nomstations.setCharacterSize(16);
			nomstations.setFillColor(Color::White);
			nomstations.setPosition(tableStation.x , tableStation.y + e * rowSpacing);
			window.draw(nomstations);
			Text text5;
			text5.setFont(font);
			int nbrpassagerstation = stations.at(e).getNbpassagerDroite();
			text5.setString(to_string(nbrpassagerstation));
			text5.setCharacterSize(16);
			text5.setFillColor(Color::White);
			text5.setPosition(tableStation.x + 150.f, tableStation.y + e * rowSpacing);
			window.draw(text5);

			Text text6;
			text6.setFont(font);
			int nbrpassagerstation2 = stations.at(e).getNbpassagerGauche();
			text6.setString(to_string(nbrpassagerstation2));
			text6.setCharacterSize(16);
			text6.setFillColor(Color::White);
			text6.setPosition(tableStation.x + 400.f, tableStation.y + e * rowSpacing);
			window.draw(text6);

		}

		
        window.display();
    }

	return 0;
}
