#pragma once

#include "vector"
#include "algorithm"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "Rame.h"
using namespace std;
class Superviseur {
private:
	vector<Rame> &rames;

public:
	// Superviseur();
	Superviseur(vector<Rame>& rames_) : rames(rames_){
        cout << "Superviseur" << endl;
    };
	~Superviseur();

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

	Rame getRame(const int& id) const;
	void arretUrgence(const int& id);
};