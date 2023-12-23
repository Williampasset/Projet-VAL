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
#include <cassert>

using namespace std;
using namespace sf;

const float SECURDISTANCE = 400;
const float VMAX = 16.6;
const int NBPASSMAX = 10;
const int STOPDISTANCE = 100;
const float ACC = 1.4;
const float TIMEPASS = 0.5;
const int RAMENOMBER = 3;

#ifdef _MSC_VER 
#define _PATH_IMG_ "C:/Program Files/SFML/img/"
#else
#define _PATH_IMG_ "../img/"
#endif

#ifdef _MSC_VER 
#define _PATH_FONT_ "C:/Program Files/SFML/font/"
#else
#define _PATH_FONT_ "../font/"
#endif

extern sf::Font font; // Déclaration externe de la police

extern const std::string path_font; // Déclaration externe du chemin de la police

const std::string path_image(_PATH_IMG_);
