#include "Projet-VAL.h"
#include "vector"
#include "algorithm"
#include <thread>
#include <chrono>

using namespace std;

void avancer(std::stop_token stop_token, Rame rame) {
	while (!stop_token.stop_requested()) {
		std::this_thread::sleep_for(100ms);
		auto distance = rame.getDistanceDArame();
		distance += (0.083) * distance;
		rame.setDistanceDA(distance);
		std::cout << "distance rame :  " << distance << std::endl;
	}
}


int main()
{
	vector<Rame> rames;
	vector<Station> stations;
	Rame rame(1);
	//Rame rame1(2);
	//Rame rame2(3);
	//rames.push_back(rame); 
	//rames.push_back(rame1);
	//rames.push_back(rame2);
	//Station station("Depart", 0); 
	//Station station1("bois blanc", 500);
	//Station station2("bois blanc", 1200);
	//Station station3("arrivee", 1700);
	//while(rames[0].getDistanceDArame() != station3.getDistanceDAstation()){

	//}
	std::stop_source s_source;
	std::jthread thr1(avancer, s_source.get_token(), rame);

	return 0;
}
