#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "vector"
#include "algorithm"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;
using namespace sf;

const float SECURDISTANCE = 400;//Distance de sécurité entre chaque rame
const float VMAX = 16.6;//vitesse max des rames
const int NBPASSMAX = 10;//Nombre de passager dans la rame et sur le quai
const int STOPDISTANCE = 100;//Distance à laquelle la déceleration s'enclenche
const float ACC = 1.4;//Accéleration des rames
const float TIMEPASS = 0.5;//Temps par passager lors de l'échange
const int RAMENOMBER = 3;

#ifdef _MSC_VER 
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define _PATH_IMG_ "C:/Program Files/SFML/img/"
#else
#define _PATH_IMG_ "../img/"
#endif


const std::string path_image(_PATH_IMG_);


