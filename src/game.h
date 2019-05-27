/*
 * game.h
 *
 *  Created on: 01-May-2019
 *      Author: sharath
 */



#include <iostream>
#include <stdlib.h>
#include <list>
#include <iterator>
#include <string>
#include <math.h>
#include <GL/glut.h>
#include "stb/stb_image.h"
#pragma warning(disable:4996)

using namespace std;

#ifndef GAME_H_
#define GAME_H_

#define BEGIN 1
#define WAR  2
#define PEACE 3

struct coord{
	float x;
	float y;
	float z;
};

////////////////////////// GLUT FUNCTIONS /////////////////////////
void war();
void reshape(int , int );
void create_menu();
void war_display();
void control(unsigned char , int , int );
void special_control(int , int , int );
void war_mousehandle(int, int , int , int);
void menu(int );
////////////////////////// COLLISSION DETECTION //////////////////
bool collision(struct coord, struct coord);

//////////////////////////// TIMER FUNCTIONS /////////////////////
void generate_opponent(int);
void opponent_shoot(int );
void terrain_animate(int );
void update(int);
void blast(int);
void ammo_relode(int);

//////////////////////////// SHOOT OPPONENT/////////////////
void generate_bullet(std::list<struct coord>* , struct coord);

//////////////////////////// DRAW FUNCTIONS /////////////////////
void draw_bullet(coord);
void draw_circle(float , float , float , float );
void draw_rectangle(float, float, float, float);
void draw_mountain();
void draw_opponent(coord);
void draw_plane();
void draw_terrain();
void draw_text(float , float , float , string );
void draw_text2(float , float , float , string );
void strokeString(float , float , float , float, string );
GLuint LoadTexture(const char*);


//////////////////////////////////////////////////////////////////
/////////////////////////// PEACE ////////////////////////////////

void peace();
void peace_display();

void plane_landing1(int );
void plane_landing2(int );
void agreement(int );

void draw_man1();
void draw_man2();
void leg_move(int );



#endif /* GAME_H_ */
