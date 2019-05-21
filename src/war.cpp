#include "game.h"
#define MAX 30

using namespace std;
extern int win_width, win_height, mode;
std::list<struct blast_coord> fire;
struct blast_coord{
	struct coord top, bottom, left, right,
		  top_right, top_left, bottom_right, bottom_left;
	int count=3;
};

///////////////////////////////////////// GAME PARAMETERS ////////////////////////////////////////////////

int score1=0, health1=10, ammo1=10;
int score2=0, health2=10, ammo2=10;
//bool player1=true, player2=true;
bool dual = false;
bool day = true;
bool paused=false, play=false;
bool finish=false;
bool change_mode=true;
int gen_opponent=3000, gen_opponent_bullet=1000, opponent_speed=100, ammo_fill=1000, rad=0;
list<struct coord> opponent;
list<struct coord> bullet1, bullet2;
list<struct coord> opponent_bullet;
struct coord plane1_pos = {-0.5f, -1.2f, -8.0f};
struct coord plane2_pos = {0.5f, -1.2f, -8.0f};
float white[] = {1.0, 1.0, 1.0, 1.0};
float black[] = {0.0, 0.0, 0.0, 1.0};
float red[] = {1.0, 0.0, 0.0, 1.0};
float green[] = {0.0, 1.0, 0.0, 1.0};
float blue[] = {0.0, 0.0, 1.0, 1.0};
float brown[] = {0.929, 0.511, 0.217, 1.0};
float pink[] = {0.85, 0.37, 0.94, 1.0};
float cyan[] = {0.14, 0.85, 0.81, 1.0};
float sky_blue[] = {0.52, 0.80, 0.97, 1.0};
float *plane1_color, *plane2_color;
float *plane_color = red;

///////////////////////////////////////  TERRAIN PARAMETERS ///////////////////////////////////////////////
GLfloat mat_ambient[] = {0.8,0.8,0.8,1.0};
GLfloat mat_diffuse[] = {0.6,0.6,0.6,1.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = {50.0};

GLfloat light_position0[] = {-50, 50, 200, 1.0};
GLfloat light_position1[] = {50, 50, 200, 1.0};

float random_height[321][42];
int yoffset=0;
int terrain_l = 20, terrain_w = 80;
int light, wireframe;
int wire = GL_FILL;


GLuint mountain_tex, plane1_tex, plane2_tex, cloud_tex, terrain_tex, night_tex;
////////////////////////// GLUT FUNCTIONS /////////////////////////////////////////////////

void war(){
	plane1_color = cyan;
	plane2_color = pink;
	float i, j;

	score1=0; health1=10; ammo1=10;
	if(dual){
		score2=0; health2=10; ammo2=10;
	}else{
		score2=0; health2=0; ammo2=0;
	}
	day = true;
	paused=false; play=false;
	finish=false;
	change_mode=true;
	gen_opponent=3000; gen_opponent_bullet=1000; opponent_speed=100; ammo_fill=1000; rad=0;
	plane1_pos = {-0.5f, -1.2f, -8.0f};
	plane2_pos = {0.5f, -1.2f, -8.0f};

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position0);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, brown);
	glLightfv(GL_LIGHT1,GL_POSITION,light_position1);
	glLightfv(GL_LIGHT1,GL_DIFFUSE, brown);


	glutTimerFunc(10 , generate_opponent, WAR);
	glutTimerFunc(1000, opponent_shoot, WAR);
	glutTimerFunc(100, terrain_animate, WAR);
	glutTimerFunc(10, update, WAR);
	glutTimerFunc(10, ammo_relode, WAR);

	for(j=-terrain_l;j<=terrain_l+1;j++)
		for(i=-terrain_w;i<=terrain_w;i+=0.5){
			random_height[(int)i+terrain_w][(int)(j+terrain_l)] = (float)rand()/RAND_MAX - 1.0;
			random_height[(int)i+terrain_w][(int)(j+terrain_l)] = (float)rand()/RAND_MAX - 1.0;
		}
	create_menu();

	glEnable(GL_TEXTURE_2D);
	if (FILE *file = fopen("../res/texture/mountain.jpg", "r")) {
		fclose(file);
		mountain_tex= LoadTexture("../res/texture/mountain.jpg");
		cloud_tex  = LoadTexture("../res/texture/day.jpg");
		night_tex = LoadTexture("../res/texture/night.jpg");
		terrain_tex = LoadTexture("../res/texture/land.jpg");
	} else if(FILE *file = fopen("./res/texture/mountain.jpg", "r")){
		fclose(file);
		mountain_tex= LoadTexture("./res/texture/mountain.jpg");
		cloud_tex  = LoadTexture("./res/texture/day.jpg");
		night_tex = LoadTexture("./res/texture/night.jpg");
		terrain_tex = LoadTexture("./res/texture/land.jpg");
	} else{
		cout<< "Couldn't Load Textures";
	}
}

void create_menu(){
	light = glutCreateMenu(menu);
	glutAddMenuEntry("Day", 3);
	glutAddMenuEntry("Night", 4);


	wireframe = glutCreateMenu(menu);
	glutAddMenuEntry("sea", 5);
	glutAddMenuEntry("land", 6);

	glutCreateMenu(menu);
	glutAddMenuEntry("Play", 0);
	glutAddMenuEntry("Pause", 1);
	glutAddMenuEntry("Resume", 2);
	glutAddSubMenu("Theme", light);
	glutAddSubMenu("Terrain", wireframe);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}
void menu(int value){
	switch(value){
	case 0: play=true; break;
	case 1: paused=true; break;
	case 2: paused=false; break;
	case 3: day = true; break;
	case 4: day = false; break;
	case 5: wire = GL_LINE; break;
	case 6: wire = GL_FILL; break;
	}
	glutPostRedisplay();
}
void war_display(){
	glClearColor(sky_blue[0], sky_blue[1], sky_blue[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// background
	glPushMatrix();
		glColor3f(1,1,1);
		//glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
		//glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
		if(day)
			glBindTexture(GL_TEXTURE_2D, cloud_tex);
		else
			glBindTexture(GL_TEXTURE_2D, night_tex);
		glBegin(GL_QUADS);
		glTexCoord2f(0.2, 0.0);
		glVertex3f(-100, 20, -30);
		glTexCoord2f(0.2, 1.0);
		glVertex3f(-100, 60, -30);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(100, 60, -30);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(100, 20, -30);
		glEnd();
		//glLightfv(GL_LIGHT0,GL_DIFFUSE, brown);
		//glLightfv(GL_LIGHT1,GL_DIFFUSE, brown);
	glPopMatrix();

	// Terrain
	glPushMatrix();
		glColor3f(brown[0], brown[1], brown[2]);
		glPolygonMode(GL_FRONT_AND_BACK, wire);
		if(day){
			glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, blue);
		}else{
			glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, black);
		}
		glTranslatef(0, -1, -12);
		glRotatef(-30, 1, 0, 0);
		draw_terrain();
		glLightfv(GL_LIGHT0,GL_DIFFUSE, brown);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, brown);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();

	// Score, Health, Pause screen
	glPushMatrix();
		////////////// SCORE /////////////////
		glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
		glColor3f(1.0f,0.0f,0.0f);
		char buffer[10];
		snprintf(buffer, 10, "%d", score1);
		draw_text(-5.0f, 3.0f, -2.0, "SCORE1:");
		draw_text(-4.5f, 3.0f, -2.0, buffer);
		if(dual){
			snprintf(buffer, 10, "%d", score2);
			draw_text(-5.0f, 2.8f, -2.0, "SCORE2:");
			draw_text(-4.5f, 2.8f, -2.0, buffer);
		}

		//////////// HEALTH /////////////////
		glColor3f(1.0f,0.0f,0.0f);
		draw_text(3.2f, 3.0f, -2.0, "HEALTH1:");
		for(int i=0;i<health1;i++){
			glBegin(GL_QUADS);
			glVertex3f(i*0.05+4  ,3.06f, -2.0f);
			glVertex3f(i*0.05+4.02 ,3.06f, -2.0f);
			glVertex3f(i*0.05+4.02 ,2.98f , -2.0f);
			glVertex3f(i*0.05+4  ,2.98f , -2.0f);
			glEnd();
		}
		if(dual){
			draw_text(3.2f, 2.8f, -2.0, "HEALTH2:");
			for(int i=0;i<health2;i++){
				glBegin(GL_QUADS);
				glVertex3f(i*0.05+4  ,2.86f, -2.0f);
				glVertex3f(i*0.05+4.02 ,2.86f, -2.0f);
				glVertex3f(i*0.05+4.02 ,2.78f , -2.0f);
				glVertex3f(i*0.05+4  ,2.78f , -2.0f);
				glEnd();
			}
		}

		//////////// PAUSE ////////////////////////
		if(paused && (health1>0 || health2>0) && play && !finish){
			glColor3f(1.0f,0.0f,0.0f);
			glScalef(5.0f,5.0f,5.0f);
			draw_text(-0.2f, 3.1f, -2.0f, "PAUSED");
			draw_text(-0.5f, 3.0f, -2.0f, "PRESS P TO RESUME");
		}

		// Armistice
		glLightfv(GL_LIGHT0,GL_DIFFUSE, brown);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, brown);
		glColor3f(1.0, 1.0, 1.0);
		glPushMatrix();
		glTranslatef(-0.1,0,-3.001);
		draw_rectangle(7*win_width/1300, -5, 2, 0.5);
		draw_rectangle(5*win_width/1300, -5, 1, 0.5);
		glPopMatrix();
		glColor3f(0.0 ,1.0 ,0.0);
		draw_text2(7.1f*win_width/1350, -4.9f, -2.99999f, "ARMISTICE");
		draw_text2(5.1f*win_width/1350, -4.9f, -2.99999f, "QUIT");
		if(score1==MAX || score2==MAX){
			finish=true;
		}
		glLightfv(GL_LIGHT0,GL_DIFFUSE, brown);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, brown);
	glPopMatrix();

	// Player plane
	glPushMatrix();
		glColor3f(1,0,0);
		glLightfv(GL_LIGHT0,GL_DIFFUSE, plane2_color);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, plane2_color);
		if(health1>0 && !finish){
			glPushMatrix();
				glRotatef(-30, 1, 0, 0);
				glTranslatef(plane1_pos.x, plane1_pos.y, plane1_pos.z);
				glScalef(0.7,0.7, 0.7);
				plane_color = plane1_color;
				draw_plane();
			glPopMatrix();
		}
		if(dual && health2>0 && !finish){
			glRotatef(-30, 1, 0, 0);
			glTranslatef(plane2_pos.x, plane2_pos.y, plane2_pos.z);
			glScalef(0.7,0.7, 0.7);
			plane_color = plane1_color;
			draw_plane();
		}
		glLightfv(GL_LIGHT0,GL_DIFFUSE, brown);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, brown);
	glPopMatrix();

	// Opponent plane , opponent bullets, player bullets
	glPushMatrix();
		glColor3f(0, 0, 1);
		glLightfv(GL_LIGHT0,GL_DIFFUSE, plane2_color);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, plane2_color);
		glRotatef(-30, 1, 0, 0);
		if(!opponent.empty() && !finish)
			for(auto it=opponent.begin();it!=opponent.end();it++){
				plane_color = plane2_color;
				draw_opponent(*it);
			}
		if(!opponent_bullet.empty() && !finish)
			for(auto it=opponent_bullet.begin(); it!=opponent_bullet.end(); it++)
				draw_bullet(*it);
		glLightfv(GL_LIGHT0,GL_DIFFUSE, brown);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, brown);

		////////////////// PLAYERS BULLETS /////////////////
		glLightfv(GL_LIGHT0,GL_DIFFUSE, black);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, black);
		if(!bullet1.empty() && !finish)
			for(auto it=bullet1.begin();it!=bullet1.end();it++)
				draw_bullet(*it);
		if(!bullet2.empty() && !finish)
			for(auto it=bullet2.begin();it!=bullet2.end();it++)
				draw_bullet(*it);
		glLightfv(GL_LIGHT0,GL_DIFFUSE, brown);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, brown);

		// blast
		for(std::list<struct blast_coord>::iterator it=fire.begin(); it!=fire.end() && !finish; it++){
			glPushMatrix();
			glTranslatef(it->top.x, it->top.y, it->top.z);
			glutSolidSphere(0.2, 20, 50);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(it->bottom.x, it->bottom.y, it->bottom.z);
			glutSolidSphere(0.2, 20, 50);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(it->left.x, it->left.y, it->left.z);
			glutSolidSphere(0.2, 20, 50);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(it->right.x, it->right.y, it->right.z);
			glutSolidSphere(0.2, 20, 50);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(it->top_left.x, it->top_left.y, it->top_left.z);
			glutSolidSphere(0.2, 20, 50);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(it->top_right.x, it->top_right.y, it->top_right.z);
			glutSolidSphere(0.2, 20, 50);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(it->bottom_left.x, it->bottom_left.y, it->bottom_left.z);
			glutSolidSphere(0.2, 20, 50);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(it->bottom_right.x, it->bottom_right.y, it->bottom_right.z);
			glutSolidSphere(0.2, 20, 50);
			glPopMatrix();
		}
	glPopMatrix();



	// DRAW MOUNTAIN
	glPushMatrix();
		glColor3f(1,1,1);
		glLightfv(GL_LIGHT0,GL_DIFFUSE, brown);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, brown);
		glPushMatrix();
		glTranslatef(-10, 0, 0);
		draw_mountain();
		glTranslatef(-15, 0, 0);
		draw_mountain();
		glTranslatef(-10, 0, 0);
		draw_mountain();
		glTranslatef(-15, 0, 0);
		draw_mountain();
		glTranslatef(-10, 0, 0);
		draw_mountain();
		glPopMatrix();
		glTranslatef(5, 0, 0);
		draw_mountain();
		glTranslatef(5, 0, 0);
		draw_mountain();
		glTranslatef(17, 0, 0);
		draw_mountain();
		glTranslatef(15, 0, 0);
		draw_mountain();
		glTranslatef(17, 0, 0);
		draw_mountain();
		glLightfv(GL_LIGHT0,GL_DIFFUSE, brown);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, brown);
	glPopMatrix();

	// NO OF BULLETS LEFT
	glPushMatrix();
		glLightfv(GL_LIGHT0,GL_DIFFUSE, black);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, black);
		glTranslatef(2, -5, -1);
		glScalef(2.0, 2.0, 1);
		glColor3f(0.0f,0.0f,0.0f);
		for(int i=0;i<ammo1;i++){
			glBegin(GL_QUADS);
			glVertex3f(i*0.04+1.5  ,1.56f, -1.0f);
			glVertex3f(i*0.04+1.52 ,1.56f, -1.0f);
			glVertex3f(i*0.04+1.52 ,1.48f , -1.0f);
			glVertex3f(i*0.04+1.5  ,1.48f , -1.0f);
			glEnd();
		}
		if(dual)
			for(int i=0;i<ammo2;i++){
				glBegin(GL_QUADS);
				glVertex3f(i*0.04+1.5  ,1.36f, -1.0f);
				glVertex3f(i*0.04+1.52 ,1.36f, -1.0f);
				glVertex3f(i*0.04+1.52 ,1.28f , -1.0f);
				glVertex3f(i*0.04+1.5  ,1.28f , -1.0f);
				glEnd();
			}
		glLightfv(GL_LIGHT0,GL_DIFFUSE, brown);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, brown);
	glPopMatrix();

	//////////////////////// FINISHED /////////////////////////
	if(finish){
		if(score1==MAX){
			draw_text2(-1.2f, 1.1f, -2.0f, "CONGRATULATION");
			draw_text2(-1.3f, 0.8f, -2.0f, "YOU  WON THE WAR");
			draw_text2(-4.0f, 0.3f, -2.0f, "THE REAL AND LASTING VICTORIES ARE THOSE OF PEACE, AND NOT OF WAR.");
		}
		else if(score2==MAX){
			draw_text2(-1.2f, 1.1f, -2.0f, "CONGRATULATION");
			draw_text2(-1.3f, 0.8f, -2.0f, "YOU WON THE WAR");
			draw_text2(-4.0f, 0.3f, -2.0f, "THE REAL AND LASTING VICTORIES ARE THOSE OF PEACE, AND NOT OF WAR.");
		}
		else{
			mode = PEACE;
			peace();
			glutDisplayFunc(peace_display);
			glutReshapeFunc(reshape);
			glutPostRedisplay();
		}
	}

	glutSwapBuffers();
}

void control(unsigned char key, int x, int y){
	switch(key){
	case 'w':
	case ' ': if(!paused && ammo1>0 && play){
				generate_bullet(&bullet1, plane1_pos);
				ammo1--;
			  }
			  break;
	case 'd': if(plane1_pos.x < 7.0f && !paused)
				plane1_pos.x += 0.5;
			  break;
	case 'a': if(plane1_pos.x > -7.0f && !paused)
				plane1_pos.x -= 0.5;
			  break;
	case 'p': if(paused)
					paused=false;
			  else
				  	paused=true;
			  break;
	}
	glutPostRedisplay();
}

void special_control(int key, int x, int y){
	switch(key){
	case GLUT_KEY_UP: if(!paused && ammo2>0 && play && dual){
							generate_bullet(&bullet2, plane2_pos);
							ammo2--;
				  	  	  }
				  	  	  break;
	case GLUT_KEY_RIGHT: if(plane2_pos.x < 10.0f && !paused && play && dual)
							plane2_pos.x += 0.5f;
						break;
	case GLUT_KEY_LEFT: if(plane2_pos.x > -10.0f && !paused && play && dual)
							plane2_pos.x -= 0.5f;
						break;
	}
}

void war_mousehandle(int button, int state, int x, int y){
	float xval = (float)x/(float)win_width*500.0;
	float yval = (float)y/(float)win_height*500.0;
	if(xval>=444 && xval<=499 && yval>=468 && yval<=489){
		mode = PEACE;
		peace();
		glutDisplayFunc(peace_display);
		reshape(win_width, win_height);
		glutReshapeFunc(reshape);
		glutPostRedisplay();
	}
	if(xval>=386 && xval<=413 && yval>=466 && yval<=488 ){
		extern int pid;
		glutDestroyWindow(pid);
	}
	if(dual && button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && !paused && play && ammo2>0){
		generate_bullet(&bullet2, plane2_pos);
			ammo2--;
	}
}

/////////////////////////////////// COLLISSION DETECTION /////////////////////////////////
bool collision(coord a, coord b){
	if(abs(a.x-b.x)<0.8f && abs(a.y-b.y)<0.5f){
		struct coord temp = {a.x, a.y, a.z};
		struct blast_coord temp1 = {temp, temp, temp, temp, temp, temp, temp, temp};
		fire.push_back(temp1);
		return true;
	}
	return false;
}

/////////////////////////////////// TIMER FUNCTIONS //////////////////////////////////////
void generate_opponent(int value){
	struct coord temp;
	if(!paused && value==WAR && play){
		temp.x = ((float)rand())*10/RAND_MAX - 8.0;
		temp.y = 30;
		temp.z = plane1_pos.z+1.0;
		opponent.push_back(temp);
		glutPostRedisplay();
	}
	if(mode==WAR)
	glutTimerFunc(gen_opponent, generate_opponent, value);
}

void opponent_shoot(int value){
	if(!paused && value==WAR && play && !opponent.empty()){
		int r = rand() % (opponent.size());
		list<struct coord>::iterator it= opponent.begin();
		advance(it, r);
		//if(!opponent.empty())
		generate_bullet(&opponent_bullet, *it);
	}
	if(mode==WAR)
		glutTimerFunc(gen_opponent_bullet, opponent_shoot, value);
}
void terrain_animate(int value){
	if(!paused && value==WAR && play){
		yoffset += 1;
		if(yoffset>terrain_l)
			yoffset=0;
		glutPostRedisplay();
	}
	if(mode==WAR)
		glutTimerFunc(opponent_speed, terrain_animate, value);
}
void update(int value){
	list<struct coord>::iterator it, it1;
	if(!paused && value==WAR && play){
		// VIEW OPPONENT
		if(!opponent.empty())
		for(it=opponent.begin();it!=opponent.end();){
			(*it).y -= 0.4;
			if((*it).y < -10.0f){
				it = opponent.erase(it);
			}
			if (it != opponent.end())
				it++;
		}
		// VIEW BULLET1
		if(!bullet1.empty())
		for(it=bullet1.begin();it!=bullet1.end(); ){
			(*it).y += 0.7;
			if((*it).y > 30.0f){
				it = bullet1.erase(it);
			}
			if (!bullet1.empty())
				it++;
		}
		// VIEW BULLET2
		if(!bullet2.empty())
		for(it=bullet2.begin();it!=bullet2.end(); ){
			(*it).y += 0.7;
			if((*it).y > 30.0f){
				it = bullet2.erase(it);
			}
			if (!bullet2.empty())
				it++;
		}

		// VIEW OPPONENT BULLET
		if(!opponent_bullet.empty())
		for(it=opponent_bullet.begin(); it!=opponent_bullet.end(); ){
			it->y -= 0.7;
			if(it->y < -10.0f){
				it = opponent_bullet.erase(it);
			}
			if (it != opponent_bullet.end())
				it++;
			else
				break;
		}

		// COLLISION1
		for (it = opponent.begin();it != opponent.end();) {
			for (it1 = bullet1.begin();it1 != bullet1.end(); it1++) {
				if (collision(*it, *it1)) {
					score1++;
					if (gen_opponent > 2000)
						gen_opponent -= 100;     //MIN 2500  3000
					if (gen_opponent_bullet > 500)
						gen_opponent_bullet -= 10; //MIN 500  1000
					if (opponent_speed > 50)
						opponent_speed -= 5; //MIN 80   100
					if (ammo_fill > 700)
						ammo_fill -= 50;     //MIN 700  1000
					it = opponent.erase(it);
					it1 = bullet1.erase(it1);
					break;
				}
			}
			if (it!=opponent.end())
				it++;
		}
		//COLLISION2
		for (it = opponent.begin();it != opponent.end();) {
			for (it1 = bullet2.begin();it1 != bullet2.end(); it1++) {
				if (collision(*it, *it1)) {
					score2++;
					if (gen_opponent > 2000)
						gen_opponent -= 100;     //MIN 2500  3000
					if (gen_opponent_bullet > 500)
						gen_opponent_bullet -= 10; //MIN 500  1000
					if (opponent_speed > 50)
						opponent_speed -= 5; //MIN 80   100
					if (ammo_fill > 700)
						ammo_fill -= 50;     //MIN 700  1000
					it = opponent.erase(it);
					it1 = bullet2.erase(it1);
					break;
				}
			}
			if (it!=opponent.end())
				it++;
		}

		// HEALTH UPDATE
		if(!opponent_bullet.empty())
		for(it=opponent_bullet.begin();it!=opponent_bullet.end();){
			if(collision(*it, plane1_pos)){
				it = opponent_bullet.erase(it);
				health1--;
			}
			if(dual && collision(*it, plane2_pos)){
				it = opponent_bullet.erase(it);
				health2--;
			}
			if (it != opponent_bullet.end())
				it++;
		}

		// HEALTH UPDATE
		if(!opponent.empty())
			for (it = opponent.begin();it != opponent.end();) {
			if(collision(*it, plane1_pos)){
				it = opponent.erase(it);
				health1--;
			}
			if(dual && collision(*it, plane2_pos)){
				it = opponent.erase(it);
				health2--;
			}
			if (it != opponent.end())
				it++;
		}



		// SURVIVAL
		if(health1==0 && health2==0){
			paused = true;
			finish = true;
		}

		// WIN
		if(score1==MAX || score2==MAX){
			paused = true;
		}

		// blast coord
		if(!fire.empty())
		for(auto it=fire.begin();it!=fire.end();){
			it->top.y += 0.1;
			//it->top.z -= 0.1;
			it->bottom.y -= 0.1;
			//it->bottom.z -= 0.1;
			it->left.x -= 0.1;
			//it->bottom.z -= 0.1;
			it->right.x += 0.1;
			//it->right.z -= 0.1;

			it->top_right.x += 0.1;
			it->top_right.y += 0.1;
			//it->top_right.z -= 0.1;

			it->top_left.x -= 0.1;
			it->top_left.y += 0.1;
			//it->top_left.z -= 0.1;

			it->bottom_right.x += 0.1;
			it->bottom_right.y -= 0.1;
			//it->bottom_right.z -= 0.1;

			it->bottom_left.x -= 0.1;
			it->bottom_left.y -= 0.1;
			//it->bottom_left.z -= 0.1;
			it->count++;
			if(it->count>10)
				it = fire.erase(it);
			if (it != fire.end())
				it++;

		}
		glutPostRedisplay();
	}
	if(mode==WAR)
		glutTimerFunc(opponent_speed, update, value);
}

void ammo_relode(int value){
	if(!paused && value==WAR && play){
		if(ammo1<10)
			ammo1++;
		if(ammo2<10)
			ammo2++;
	}
	if(mode==WAR)
		glutTimerFunc(ammo_fill, ammo_relode, value);
}

//////////////////////////// SHOOT OPPONENT///////////////////////////////

void generate_bullet(list<struct coord> *list1, struct coord pos){
	struct coord temp;
	if(!paused && play){
		temp.x = pos.x;
		temp.y = pos.y;
		temp.z = pos.z+1.0;
		list1->push_back(temp);
		
	}
	glutPostRedisplay();
}

/////////////////////////////////// DRAW FUNCTIONS ////////////////////////////////


void draw_bullet(coord pos){
	float value[8][3] = {{pos.x-0.1f, pos.y-0.3f, pos.z-0.5f},
					     {pos.x+0.1f, pos.y-0.3f, pos.z-0.5f},
						 {pos.x+0.1f, pos.y+0.3f, pos.z-0.5f},
						 {pos.x-0.1f, pos.y+0.3f, pos.z-0.5f}};
	glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_QUADS);
		glVertex3fv(value[0]);
		glVertex3fv(value[1]);
		glVertex3fv(value[2]);
		glVertex3fv(value[3]);
	glEnd();

}

void draw_circle(float x, float y, float z, float r){
	float theta;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x , y, z);
	for (theta=0.0f; theta<360.0f; theta+=0.2)
		glVertex3f(x+sin(theta)*r, y+cos(theta)*r, z);

	glEnd();

}
void draw_rectangle(float x, float y, float w, float h){
	glBegin(GL_QUADS);
	glVertex3f(x, y, 0.001);
	glVertex3f(x+w, y, 0.001);
	glVertex3f(x+w, y+h, 0.001);
	glVertex3f(x, y+h, 0.001);
	glEnd();
}

void draw_mountain(){

	glBindTexture(GL_TEXTURE_2D, mountain_tex);

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-15.0, 14, -20);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(0.0, 21, -20);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(15.0, 14, -20);
	glEnd();
}
void draw_opponent(coord pos){
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(0.7, 0.7, 0.7);
	glRotatef(180, 0.0, 0.0, 1.0);
	draw_plane();
	glPopMatrix();
}

void draw_plane(){
	//Plane exhaust
    glColor3f(1.0, 0.0, 0.0);
    draw_circle(0.5, -2, 0, 0.3);
    draw_circle(-0.5, -2, 0, 0.3);

    //Plane Body
    glColor3fv(plane_color);
    glPushMatrix();
    glScalef(0.8, 2.0, 0.5);
    glutSolidSphere(1, 20, 30);
    glPopMatrix();

    glBegin(GL_TRIANGLES);
    //Right Wing
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 0);
    glVertex3f(0.0, 1, 0);
    glColor3fv(plane_color);
    glVertex3f(5.0, -1.0, 0);

    //Left Wing
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 0);
    glVertex3f(0.0, 1, 0);
    glColor3fv(plane_color);
    glVertex3f(-5.0, -1.0, 0);

    //Right Tail Wing
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, -2.0, 0);
    glVertex3f(0.0, -1.0, 0);
    glColor3fv(plane_color);
    glVertex3f(2.0, -2.5, 0);

    //Left Tail Wing
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, -2.0, 0);
    glVertex3f(0.0, -1.0, 0);
    glColor3fv(plane_color);
	glVertex3f(-2.0, -2.5, 0);

	// rudder
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0, -2.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, -1.0, 2.0);

	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-0.2, 0.0, 1.0);
	glVertex3f(0.2, 0.0, 1.0);
	glVertex3f(0.0, 1.0, 1.0);

    glEnd();

    glFlush();
}

void draw_terrain(){
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    float i, j;
    glColor3f(0.97, 0.62, 0.25);

   glBindTexture(GL_TEXTURE_2D, terrain_tex);

	glBegin(GL_TRIANGLE_STRIP);
	for(j=-terrain_l;j<=terrain_l;j++){
		for(i=-terrain_w;i<=terrain_w;i+=0.5){
			glTexCoord2f(1, 1);
			glVertex3f(i, j, random_height[(int)i+terrain_w][(int)(j+terrain_l+yoffset)%(terrain_l+1)]);
			glTexCoord2f(0, 0);
			glVertex3f(i, j+1, random_height[(int)i+terrain_w][(int)(j+terrain_l+1+yoffset)%(terrain_l+1)]);
		}
	}
	glEnd();


}

void draw_text(float x, float y, float z, string s){
	const char *c = s.c_str();
	int i=0;
	glRasterPos3f(x, y, z);
	for(i=0; c[i]!='\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c[i]);
}

void draw_text2(float x, float y, float z, string s){
	const char *c = s.c_str();
	int i=0;
	glRasterPos3f(x, y, z);
	for(i=0; c[i]!='\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c[i]);
}

GLuint LoadTexture(const char *filename){
    GLuint texture = 0;
    int width, height, comp;
    unsigned char *data = NULL;

    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(filename, &width, &height, &comp, STBI_rgb_alpha);

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    //  Create texture mipmap for faster processsing
    //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_BGRA_EXT, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_TEXTURE_ENV_COLOR);
    // Parameters to adjust the image
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Parameters to wrap the texture image
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Deallocate the data
    free(data);

    return texture;
}
