#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>
#include "train.hpp"
using namespace std;
using namespace sf;

#ifdef _MSC_VER 
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define _PATH_IMG_ "C:/Program Files/SFML/img/"
#else
#define _PATH_IMG_ "../img/"
#endif


const std::string path_image(_PATH_IMG_);


int main() {
    // Création de la fenêtre SFML
    sf::VideoMode fullscreenMode = sf::VideoMode::getFullscreenModes()[0];
    sf::RenderWindow window(fullscreenMode, "Objet suivant un trajet", sf::Style::Fullscreen);
    std::cout << "Taille de l'écran en plein écran : " << fullscreenMode.width << "x" << fullscreenMode.height << std::endl;
    // Définition du trajet (coordonnées pour une boucle)
    std::vector<sf::Vector2f> trajet = {
        {202.f, 250.f},
        {1860.f, 250.f},
        {1860.f, 970.f},
        {202.f, 970.f}
    };

    // Fond d'écran
    Texture backgroundImage, objet, runnerImage;
    Sprite backgroundSprite, objetSprite, runnerSprite;

    if (!backgroundImage.loadFromFile(path_image + "rail.png") || !objet.loadFromFile(path_image + "train.png")){
        cerr << "Erreur pendant le chargement des images" << endl;
        return EXIT_FAILURE; // On ferme le programme
    }
    backgroundSprite.setTexture(backgroundImage);
    objetSprite.setTexture(objet);
    objetSprite.setScale(sf::Vector2f(0.5, 0.5));

    // Création d'un point
    sf::CircleShape point(20.f);
    point.setFillColor(sf::Color::Red);
    sf::Vector2f pointCible = trajet[0];
    point.setPosition(pointCible);

    sf::CircleShape point1(20.f);
    point1.setFillColor(sf::Color::Red);
    pointCible = trajet[1];
    point1.setPosition(pointCible);

    sf::CircleShape point2(20.f);
    point2.setFillColor(sf::Color::Red);
    pointCible = trajet[2];
    point2.setPosition(pointCible);
   
    sf::CircleShape point3(20.f);
    point3.setFillColor(sf::Color::Red);
    pointCible = trajet[3];
    point3.setPosition(pointCible);
    

    // Indice du point actuel sur le trajet
    size_t index = 0;

    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Déplacement de l'objet le long du trajet
        sf::Vector2f pointCible = trajet[index];
        sf::Vector2f direction = pointCible - objetSprite.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // Vérification si l'objet est proche du point cible
        if (distance < 1.0f) {
            index = (index + 1) % trajet.size(); // Passage au prochain point du trajet
        }
        else {
            direction /= distance; // Normalisation du vecteur direction
            objetSprite.move(direction * 2.0f); // Vitesse de déplacement de l'objet
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(point);
        window.draw(point1);
        window.draw(point2);
        window.draw(objetSprite);
        
        window.display();
    }

    return 0;
}
