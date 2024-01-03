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
#include <cmath>

using namespace std;
using namespace sf;

const float SECURDISTANCE = 800;
const float VMAX = 16.6f;
const int NBPASSMAX = 10;
const float ACC = 1.4f;
const float TIMEPASS = 0.5f;
const int RAMENOMBER = 3;
const float DISTANCELINE = 2000;

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

extern Font font; // Déclaration externe de la police

extern const string path_font; // Déclaration externe du chemin de la police

const string path_image(_PATH_IMG_);


