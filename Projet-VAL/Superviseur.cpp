#include "Superviseur.h"
#include "Rame.h"
using namespace std;

// Superviseur::Superviseur(){
//     rames.push_back(Rame());
// 	cout << "Nouveau Superviseur" << endl;
// }
Superviseur::~Superviseur() {
    cout << "Fin Superviseur" << endl;
}

// void addRame(const Rame& rame) {
// 	rames.push_back(rame);
// }

/*void removeRame(const Rame& rame) { Implémenter opérateur == dans la classe Rame
    rames.erase(std::remove(rames.begin(), rames.end(), rame), rames.end());
}*/

// int getNbRame() const {
// 	return rames.size();
// }

// void setNbRame(const int& nb) {
// 	rames.resize(nb);
// }

Rame Superviseur::getRame(const int& id) const {
    if (id >= 1 && id <= rames.size()) {
        return rames[id - 1];
    }
    else{
        return 0;
    }
}

void Superviseur::arretUrgence(const int& id) {
    if (id >= 0 && id < rames.size()) {
        rames[id].setUrgence(true);
    }
}