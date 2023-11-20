#include <iostream>

class Rame {
private:
	const int id = 0;
	int nbpassager = 0;
	float v = 0;//vitesse actuelle
	double distanceDA = 1;
	//pas modifiable 
	float distanceAcc = 200;
	float distanceDec = 200;
	float distanceFreinage = 50;
	float distanceSecurite = 1000;
	double vmax = 16.6;//vitesse max
	int nbpassagermax = 10;
public:
	Rame(){
		std::cout<<"Nouvelle Rame"<<std::endl;
	}
	Rame(const int& id_) : id(id_){
		std::cout<<"Rame "<<id<<std::endl;
	}
	~Rame(){
		std::cout<<"Fin Rame"<<std::endl;
	}
	void setV(const float& v_){
		v = v_;
	}
	void setDistanceDA(const double& distanceDA_){
		distanceDA = distanceDA_;
	}
	void setNbpassager(const int& nbpassager_){
		nbpassager = nbpassager_;
	}
	int getId() const{
		return id;
	}
	int getNbpassager() const{
		return nbpassager;
	}
	float getV() const{
		return v;
	}
	double getDistanceDArame() const{
		return distanceDA;
	}
};

};