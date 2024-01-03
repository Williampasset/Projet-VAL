#include "Projet-VAL.h"
#include "Rame.h"
#include "Station.h"


Font font; // Définition de la police
Font arial; 
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
					rame.Avancer(*(itStation + 1), *(itStationBase + 1));
					itStation++;
					itStationBase++;
				}
				else{
					rame.Avancer(*(itStation - 1), *(itStationBase - 1));
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
	stations.emplace_back("Rihour", DISTANCELINE, 600, 2);
	
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
	if (!font.loadFromFile(path_font + "aeroport.ttf") || !arial.loadFromFile(path_font + "arial.ttf")) {
		cerr << "Error while loading font" << endl;
		return -1;
	}
	
    // Fond d'écran
    Texture backgroundImage, objet, objet2;
    Sprite backgroundSprite;

    if (!backgroundImage.loadFromFile(path_image + "rail.png") || !objet.loadFromFile(path_image + "train.png") || !objet2.loadFromFile(path_image + "trainarrêt.png") ) {
        cerr << "Erreur pendant le chargement des images" << endl;
        return EXIT_FAILURE; // On ferme le programme
    }
    backgroundSprite.setTexture(backgroundImage);

	vector<Sprite> ObjetSprite;
	vector<Sprite> ObjetSprite2;

	for (auto& rame : rames) {
		Sprite objetSprite;
		Sprite objetSprite2;
		objetSprite.setTexture(objet);
		objetSprite.setScale(Vector2f(0.33f, 0.33f));
		ObjetSprite.push_back(objetSprite);
		objetSprite2.setTexture(objet2);
		objetSprite2.setScale(Vector2f(0.33f, 0.33f));
		ObjetSprite2.push_back(objetSprite2);
	}

	// Indice du point actuel sur le trajet
    size_t index = 0;
	// Pour le tableau 

	vector<Row> rows{
	   {"Rame 1", RectangleShape(Vector2f(50, 20))},
	   {"Rame 2", RectangleShape(Vector2f(50, 20))},
	   {"Rame 3", RectangleShape(Vector2f(50, 20))}
	  
	};
	
	Vector2f tablePosition(110.f, 750.f);

	Vector2f tableStation(1200.f, 750.f);
	
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
		window.draw(backgroundSprite);
		auto i = 0;//Compteur

		//Affichage des rames
		for(auto& rame : rames){
			if (rame.getRotate()) {
				ObjetSprite.at(i).setScale(rame.getDirection() == 1 ? -(0.33f) : 0.33f, 0.33f);
				ObjetSprite2.at(i).setScale(rame.getDirection() == 1 ? -(0.33f) : 0.33f, 0.33f);
			}
			ObjetSprite.at(i).setPosition(rame.getXpos(), rame.getYpos());
			ObjetSprite2.at(i).setPosition(rame.getXpos(), rame.getYpos());
			if(rame.getGo()){
				if (rame.getDistanceOldStation() > 1) {
					window.draw(ObjetSprite.at(i));
				}
				else {
					window.draw(ObjetSprite2.at(i));
				}
			}
			i++;
		}
		// Affichage des stations 
		for (auto& station : stations) {
			CircleShape point(15.f);
			point.setFillColor(Color::Red);
			Vector2f pointCible(200 + (station.getDistanceDAstation()) * (1520) / DISTANCELINE, 300.f);
			point.setPosition(pointCible);
			window.draw(point);

			Text stationName;
			stationName.setFont(arial); // Utilisation de la police de caractères par défaut de SFML
			stationName.setFillColor(Color::Black);
			stationName.setCharacterSize(20);
			stationName.setString(station.getNom());
			stationName.setPosition(pointCible.x-30.f, pointCible.y +50.f);		
			window.draw(stationName);
		}

		// Affichage des titres 
		Text TitreRame;
		TitreRame.setFont(font);
		TitreRame.setString("Informations Rames");
		TitreRame.setCharacterSize(40);
		TitreRame.setFillColor(Color::Yellow);
		TitreRame.setPosition(tablePosition.x+300.f, tablePosition.y -110.f);
		window.draw(TitreRame);
		Text Titre0;
		Titre0.setFont(font);
		Titre0.setString("Rames");
		Titre0.setCharacterSize(40);
		Titre0.setFillColor(Color::Yellow);
		Titre0.setPosition(tablePosition.x, tablePosition.y - 40.f);
		window.draw(Titre0);
		Text Titre1;
		Titre1.setFont(font);
		Titre1.setString("Vitesse");
		Titre1.setCharacterSize(40);
		Titre1.setFillColor(Color::Yellow);
		Titre1.setPosition(tablePosition.x + 150.f, tablePosition.y - 40.f);
		window.draw(Titre1);
		Text Titre2;
		Titre2.setFont(font);
		Titre2.setString("Distance");
		Titre2.setCharacterSize(40);
		Titre2.setFillColor(Color::Yellow);
		Titre2.setPosition(tablePosition.x + 300.f, tablePosition.y - 40.f);
		window.draw(Titre2);
		Text Titre3;
		Titre3.setFont(font);
		Titre3.setString("Passagers");
		Titre3.setCharacterSize(40);
		Titre3.setFillColor(Color::Yellow);
		Titre3.setPosition(tablePosition.x + 450.f, tablePosition.y - 40.f);
		window.draw(Titre3);
		Text Titre4;
		Titre4.setFont(font);
		Titre4.setString("Superviseur");
		Titre4.setCharacterSize(40);
		Titre4.setFillColor(Color::Yellow);
		Titre4.setPosition(tablePosition.x + 600.f, tablePosition.y - 40.f);
		window.draw(Titre4);

		//Pour les titres des stations 
		Text TitreStations;
		TitreStations.setFont(font);
		TitreStations.setString("Informations Stations");
		TitreStations.setCharacterSize(35);
		TitreStations.setFillColor(Color::Yellow);
		TitreStations.setPosition(tableStation.x+220.f, tableStation.y - 110.f);
		window.draw(TitreStations);

		Text TitreStations1;
		TitreStations1.setFont(font);
		TitreStations1.setString("Stations");
		TitreStations1.setCharacterSize(35);
		TitreStations1.setFillColor(Color::Yellow);
		TitreStations1.setPosition(tableStation.x, tableStation.y - 40.f);
		window.draw(TitreStations1);

		Text TitreStations2;
		TitreStations2.setFont(font);
		TitreStations2.setString("Passagers sens aller ->");
		TitreStations2.setCharacterSize(35);
		TitreStations2.setFillColor(Color::Yellow);
		TitreStations2.setPosition(tableStation.x+130.f, tableStation.y - 40.f);
		window.draw(TitreStations2);

		Text TitreStations3;
		TitreStations3.setFont(font);
		TitreStations3.setString("Passagers sens retour <-");
		TitreStations3.setCharacterSize(35);
		TitreStations3.setFillColor(Color::Yellow);
		TitreStations3.setPosition(tableStation.x+ 380.f, tableStation.y - 40.f);
		window.draw(TitreStations3);
	

		//Pour le tableau : 
		for (size_t i = 0; i < rows.size(); ++i) {
			//nom de la rame 
			Text text;
			text.setFont(font);
			text.setString(rows[i].name);
			text.setCharacterSize(40);
			text.setFillColor(Color::White);
			text.setPosition(tablePosition.x, tablePosition.y + i * rowSpacing);
			window.draw(text);
			//vitesse  de la rame 
			Text text2;
			text2.setFont(font);
			float vitesserame = rames.at(i).getV();
			text2.setString(to_string(vitesserame));
			text2.setCharacterSize(40);
			text2.setFillColor(Color::White);
			text2.setPosition(tablePosition.x + 150.f, tablePosition.y + i * rowSpacing);
			window.draw(text2);

			//distance de la rame 
			Text text3;
			text3.setFont(font);
			float distancerame = rames.at(i).getDistanceTotal();
			text3.setString(to_string(distancerame));
			text3.setCharacterSize(40);
			text3.setFillColor(Color::White);
			text3.setPosition(tablePosition.x + 300.f, tablePosition.y + i * rowSpacing);
			window.draw(text3);
			//nombre de passager 
			Text text4;
			text4.setFont(font);
			int nbrpassager = rames.at(i).getNbpassager();
			text4.setString(to_string(nbrpassager));
			text4.setCharacterSize(40);
			text4.setFillColor(Color::White);
			text4.setPosition(tablePosition.x + 450.f, tablePosition.y + i * rowSpacing);
			window.draw(text4);

			//bouton arrêt urgence 
			rows[i].button.setSize(Vector2f(buttonWidth, buttonHeight));
			rows[i].button.setPosition(tablePosition.x + 600.f, tablePosition.y + i * rowSpacing+15.f);
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
		// Pour les Stations 
		for (int e = 0; e < stations.size();e++) {
			Text nomstations;
			nomstations.setFont(font);
			auto nomstat = stations.at(e).getNom();
			nomstations.setString(nomstat);
			nomstations.setCharacterSize(35);
			nomstations.setPosition(tableStation.x , tableStation.y + e * rowSpacing);
			window.draw(nomstations);

			Text text5;
			text5.setFont(font);
			int nbrpassagerstation = stations.at(e).getNbpassagerDroite();
			text5.setString(to_string(nbrpassagerstation));
			text5.setCharacterSize(35);
			text5.setPosition(tableStation.x + 130.f, tableStation.y + e * rowSpacing);
			window.draw(text5);

			Text text6;
			text6.setFont(font);
			int nbrpassagerstation2 = stations.at(e).getNbpassagerGauche();
			text6.setString(to_string(nbrpassagerstation2));
			text6.setCharacterSize(35);
			text6.setFillColor(Color::White);
			text6.setPosition(tableStation.x + 380.f, tableStation.y + e * rowSpacing);
			window.draw(text6);

		}

		//Légendes 
		Text legende1;
		legende1.setFont(font);
		legende1.setString("Légendes");
		legende1.setCharacterSize(40);
		legende1.setFillColor(Color::Yellow);
		legende1.setPosition(tablePosition.x, tablePosition.y+150.f);
		window.draw(legende1);

		//carré blanc 
		RectangleShape rectblanc;
		rectblanc.setSize(Vector2f(buttonWidth, buttonHeight));
		rectblanc.setFillColor(Color::White);
		rectblanc.setPosition(tablePosition.x+20.f, tablePosition.y + 200.f);
		window.draw(rectblanc);
		//Texte carré blanc
		Text legende2;
		legende2.setFont(font);
		legende2.setString("Rame au départ");
		legende2.setCharacterSize(25);
		legende2.setFillColor(Color::White);
		legende2.setPosition(tablePosition.x, tablePosition.y+225.f);
		window.draw(legende2);
		//carré vert 
		RectangleShape rectvert;
		rectvert.setSize(Vector2f(buttonWidth, buttonHeight));
		rectvert.setFillColor(Color::Green);
		rectvert.setPosition(tablePosition.x+220.f, tablePosition.y + 200.f);
		window.draw(rectvert);
		//Texte carré vert
		Text legende3;
		legende3.setFont(font);
		legende3.setString("Rame en marche");
		legende3.setCharacterSize(25);
		legende3.setFillColor(Color::White);
		legende3.setPosition(tablePosition.x+200.f, tablePosition.y + 225.f);
		window.draw(legende3);
		//carré jaune 
		RectangleShape rectjaune;
		rectjaune.setSize(Vector2f(buttonWidth, buttonHeight));
		rectjaune.setFillColor(Color::Yellow);
		rectjaune.setPosition(tablePosition.x + 420.f, tablePosition.y + 200.f);
		window.draw(rectjaune);
		//Texte carré jaune
		Text legende4;
		legende4.setFont(font);
		legende4.setString("Rame en urgence automatique\n(danger detecté distance < 800m)");
		legende4.setCharacterSize(25);
		legende4.setFillColor(Color::White);
		legende4.setPosition(tablePosition.x + 350.f, tablePosition.y + 225.f);
		window.draw(legende4);
		//carré rouge 
		RectangleShape rectrouge;
		rectrouge.setSize(Vector2f(buttonWidth, buttonHeight));
		rectrouge.setFillColor(Color::Red);
		rectrouge.setPosition(tablePosition.x + 620.f, tablePosition.y + 200.f);
		window.draw(rectrouge);
		//Texte carré rouge
		Text legende5;
		legende5.setFont(font);
		legende5.setString("Rame en urgence\n(bouton appuyé)");
		legende5.setCharacterSize(25);
		legende5.setFillColor(Color::White);
		legende5.setPosition(tablePosition.x + 600.f, tablePosition.y + 225.f);
		window.draw(legende5);

        window.display();
    }

	return 0;
}
