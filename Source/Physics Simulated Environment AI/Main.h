#include <allegro.h>
#include <fstream>
#include <cstdio>
#include <vector>
#include <deque>
#include <queue>
#include <algorithm>
#include <cmath>

#include "CTimer.h"
#include "Vec2.h"
#include "FatalError.h"
#include "CColor.h"
#include "Mouse.h"
#include "AllegroEx.h"
#include "StarShip.h"
#include "AI.h"
#include "Fuzzy.h"
#include "Obstacle.h"
#include "Random.h"
#include "CCamera.h"
#include "GUI.h"

#define EPS 1e-9 
#define PI 3.14159265358979323846
#define DEG2RAD(deg) (PI * (deg) / 180.0)
#define RAD2DEG(rad) ((rad) * 180.0 / PI)
#define WHITE_COL	makecol(255,255,255)
#define KEYBOARDDELAY 0.5f
#define MOUSE_WHEEL_SPEED 0.01
#define VIRTUALSCREEN_WIDTH 1366
#define VIRTUALSCREEN_HEIGHT 768 

#define LEFTMBTN 1
#define RIGHTMBTN 2
#define MIDDLEMBTN 3

#pragma comment(lib,"alleg.lib")

