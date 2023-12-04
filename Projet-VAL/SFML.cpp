#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>

int main() {
    // Cr�ation de la fen�tre SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Objet suivant un trajet");

    // D�finition du trajet (coordonn�es pour une boucle)
    std::vector<sf::Vector2f> trajet = {
        {100.f, 100.f},
        {400.f, 100.f},
        {400.f, 400.f},
        {100.f, 400.f},
        {100.f, 100.f}
    };

    // Cr�ation de l'objet � d�placer
    sf::CircleShape objet(20.f);
    objet.setFillColor(sf::Color::Red);

    // Indice du point actuel sur le trajet
    size_t index = 0;

    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // D�placement de l'objet le long du trajet
        sf::Vector2f pointCible = trajet[index];
        sf::Vector2f direction = pointCible - objet.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // V�rification si l'objet est proche du point cible
        if (distance < 1.0f) {
            index = (index + 1) % trajet.size(); // Passage au prochain point du trajet
        }
        else {
            direction /= distance; // Normalisation du vecteur direction
            objet.move(direction * 2.0f); // Vitesse de d�placement de l'objet
        }

        window.clear();
        window.draw(objet);
        window.display();
    }

    return 0;
}
