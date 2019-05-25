#include "game.h"

using namespace std;

extern float win_width, win_height;
extern float white[];
extern float black[];
extern float red[];
extern float green[];
extern float blue[];
extern float brown[];
extern float pink[];
extern float cyan[];
extern float sky_blue[];
extern float *plane1_color, *plane2_color, *plane_color;
extern GLuint cloud_tex, night_tex;
bool turn1 = false, turn2 = false;
float color1[][4] = { {1.0, 0.0, 0.0 , 1.0},{0.0, 0.0, 0.0, 1.0},{0.90, 0.86, 0.23, 1.0}, {0.14, 0.85, 0.81, 1.0},
					{0.25, 0.28, 0.86, 1.0}, {0.85, 0.37, 0.94, 1.0}, {0.93, 0.38, 0.54, 1.0} };

float theta1 = -90, theta2 = 90;

struct coord plane_pos1 = { -10.0f, 15.0f, 10.0f };
struct coord plane_pos2 = { 10.0f, 15.0f, 10.0f };
list<struct coord> landed_plane1;
list<struct coord> landed_plane2;


/////////////////////////
float theta = 0, front1 = -5, front2 = 5;
bool clock1 = true;

int printing = 0;

extern bool day;
void peace() {
	war();
	turn1 = false; turn2 = false;
	theta1 = -90; theta2 = 90;
	plane_pos1 = { -10.0f, 15.0f, 10.0f };
	plane_pos2 = { 10.0f, 15.0f, 10.0f };
	landed_plane1.clear();
	landed_plane2.clear();
	theta = 0; front1 = -5;front2 = 5;
	clock1 = true;
	printing = 0;
	glutTimerFunc(10, plane_landing1, 0);
	glutTimerFunc(10, plane_landing2, 0);
	glutTimerFunc(1000, leg_move, 0);
	glutTimerFunc(3000, agreement, 0);

	glutCreateMenu(menu);
	glutAddMenuEntry("Day", 3);
	glutAddMenuEntry("Night", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void peace_display() {

	glClearColor(sky_blue[0], sky_blue[1], sky_blue[2], 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// background
	glPushMatrix();
	glColor3f(sky_blue[0], sky_blue[1], sky_blue[2]);
	if (day)
		glBindTexture(GL_TEXTURE_2D, cloud_tex);
	else
		glBindTexture(GL_TEXTURE_2D, night_tex);
	glBegin(GL_QUADS);
	glTexCoord2f(0.2, 0.0);
	glVertex3f(-120, 0, -30);
	glTexCoord2f(0.2, 1.0);
	glVertex3f(-120, 60, -30);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(120, 60, -30);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(120, 0, -30);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	if (day) {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, blue);
	}
	else {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, black);
	}
	glTranslatef(0, -1, -12);
	glRotatef(-90, 1, 0, 0);
	draw_terrain();
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glPopMatrix();

	// Left to right plane
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(plane_pos1.x, plane_pos1.y, plane_pos1.z);
	glScalef(0.8, 0.5, 0.5);
	glRotatef(theta1, 0, 0, 1);
	plane_color = plane1_color;
	draw_plane();
	glPopMatrix();

	// Right to left plane
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(plane_pos2.x, plane_pos2.y, plane_pos2.z);
	glScalef(0.8, 0.5, 0.5);
	glRotatef(theta2, 0, 0, 1);
	plane_color = plane2_color;
	draw_plane();
	glPopMatrix();


	// landed plane1
	for (auto it = landed_plane1.begin(); it != landed_plane1.end(); it++) {
		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glTranslatef((*it).x, (*it).y, (*it).z);
		glRotatef(90, 0, 0, 1);
		plane_color = plane1_color;
		draw_plane();
		glPopMatrix();
	}

	// landed plane2
	for (auto it = landed_plane2.begin(); it != landed_plane2.end(); it++) {
		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glTranslatef((*it).x, (*it).y, (*it).z);
		glRotatef(-90, 0, 0, 1);
		plane_color = plane2_color;
		draw_plane();
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(0, 0, 5);
	glColor3fv(white);
	draw_man1();
	glColor3fv(white);
	draw_man2();
	glPopMatrix();


	// DRAW MOUNTAIN
	glColor3fv(brown);
	glPushMatrix();
	glTranslatef(0, -15, 0);
	glPushMatrix();
	glColor3f(1, 1, 1);
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
	glPopMatrix();
	glPopMatrix();

	glColor3f(0.0, 0.0, 0.0);
	if (printing) {
		if (printing > 1)
			draw_text2(-2.5, -0.5, -0.9, "The undersigned the Commander-in-Chief, Arnor, on the one hand, and the Supreme Commander of Gondor is");
		if (printing > 2)
			draw_text2(-2.5, -0.6, -0.9, "interested in stopping the conflict with the following terms of armistice.");
		if (printing > 3)
			draw_text2(-2.5, -0.75, -0.9, "1. Military demarcation line shall be fixed and both sides shall withdraw 2Km from this line to establish a demilitary zone.");
		if (printing > 4)
			draw_text2(-2.5, -0.85, -0.9, "2. The waters of the Gihon shall be open to civil shipping of both sides.");
		if (printing > 5)
			draw_text2(-2.5, -0.95, -0.9, "3. Neither side shall execute any hostile act within, from otr against the demilitarized zone.");
		if (printing > 6)
			draw_text2(-2.5, -1.05, -0.9, "4. No person shall cross the military demarcation unless authorized by Military Armstice Commission.");

		if (printing > 7) {
			glColor3f(1.0, 1.0, 1.0);
			draw_text2(1.0f, -1.5f, -0.9f, "EXIT");
		}
	}

	glutSwapBuffers();

}

int timer1 = 10, timer2 = 10;
void plane_landing1(int value) {
	if (plane_pos1.x > 30.0)
		turn1 = true;
	if (!turn1)
		plane_pos1.x += 0.55;
	else
		plane_pos1.x -= 0.55;
	plane_pos1.y -= 0.1;
	plane_pos1.z -= 0.1;
	theta1 -= 2;
	timer1++;
	glutPostRedisplay();
	if (plane_pos1.z > -1)
		glutTimerFunc((int)(timer1 * 1.001), plane_landing1, 0);
	else {
		struct coord temp;
		temp.x = plane_pos1.x;
		temp.y = plane_pos1.y;
		temp.z = plane_pos1.z;
		cout << temp.x << " " << temp.y << " " << temp.z << endl;
		landed_plane1.push_back(temp);

		plane_pos1.x = -10.0;
		plane_pos1.y = 15.0 + 1 * landed_plane1.size();
		plane_pos1.z = 10.0;
		timer1 = 10;
		turn1 = false;
		theta1 = -90;
		glutTimerFunc((int)(timer1 * 1.001), plane_landing1, 0);
	}
}
void plane_landing2(int value) {
	if (plane_pos2.x < -30.0)
		turn2 = true;
	if (!turn2)
		plane_pos2.x -= 0.55;
	else
		plane_pos2.x += 0.55;
	plane_pos2.y -= 0.1;
	plane_pos2.z -= 0.1;
	theta2 += 2;
	timer2++;
	glutPostRedisplay();
	if (plane_pos2.z > -1)
		glutTimerFunc((int)(timer2 * 1.001), plane_landing2, 0);
	else {
		struct coord temp;
		temp.x = plane_pos2.x;
		temp.y = plane_pos2.y;
		temp.z = plane_pos2.z;
		cout << temp.x << " " << temp.y << " " << temp.z << endl;
		landed_plane2.push_back(temp);

		plane_pos2.x = 10.0;
		plane_pos2.y = 15.0 + 1 * landed_plane2.size();
		plane_pos2.z = 10.0;
		timer2 = 10;
		turn2 = false;
		theta2 = 90;
		glutTimerFunc((int)(timer2 * 1.001), plane_landing2, 0);
	}
}

void leg_move(int value) {
	if (clock1) {
		theta--;
		front1 += 0.01;
		front2 -= 0.01;
		if (theta < -20)
			clock1 = false;
	}
	else {
		theta++;
		front1 += 0.01;
		front2 -= 0.01;
		if (theta > 20)
			clock1 = true;
	}
	glutPostRedisplay();
	if (front1<-0.2 && front2>0.2)
		glutTimerFunc(50, leg_move, 0);
	else {
		theta = 0;
		glutPostRedisplay();
	}
}

void agreement(int value) {
	printing++;
	if (printing < 8)
		glutTimerFunc(3000, agreement, 0);
}
void draw_man1() {

	////////////////////// man 1 ////////////////////
	glPushMatrix();

	//body
	glPushMatrix();
	glTranslatef(front1, 0.15, -8);
	glScalef(1, 3, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	// face
	glPushMatrix();
	glTranslatef(front1, 0.4, -8);
	glScalef(1, 2, 1);
	glutSolidSphere(0.06, 20, 50);
	glPopMatrix();

	//back leg
	glPushMatrix();
	glTranslatef(front1, 0, -8);
	glRotatef(theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();

	//front leg
	glPushMatrix();
	glTranslatef(front1, 0, -8);
	glRotatef(-theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();
	////////////////////////////man 2///////////////////////////////
	glPushMatrix();
	glTranslatef(-0.5, 0, -0.5);
	//body
	glPushMatrix();
	glTranslatef(front1, 0.15, -8);
	glScalef(1, 3, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	// face
	glPushMatrix();
	glTranslatef(front1, 0.4, -8);
	glScalef(1, 2, 1);
	glutSolidSphere(0.06, 20, 50);
	glPopMatrix();

	//back leg
	glPushMatrix();
	glTranslatef(front1, 0, -8);
	glRotatef(theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();

	//front leg
	glPushMatrix();
	glTranslatef(front1, 0, -8);
	glRotatef(-theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();

	///////////////////////////////man 3/////////////////////////
	glPushMatrix();
	glTranslatef(-0.5, 0, 0.5);
	//body
	glPushMatrix();
	glTranslatef(front1, 0.15, -8);
	glScalef(1, 3, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	// face
	glPushMatrix();
	glTranslatef(front1, 0.4, -8);
	glScalef(1, 2, 1);
	glutSolidSphere(0.06, 20, 50);
	glPopMatrix();

	//back leg
	glPushMatrix();
	glTranslatef(front1, 0, -8);
	glRotatef(theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();

	//front leg
	glPushMatrix();
	glTranslatef(front1, 0, -8);
	glRotatef(-theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();

}
void draw_man2() {
	////////////////////// man 1 ////////////////////
	glPushMatrix();

	//body
	glPushMatrix();
	glTranslatef(front2, 0.15, -8);
	glScalef(1, 3, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	// face
	glPushMatrix();
	glTranslatef(front2, 0.4, -8);
	glScalef(1, 2, 1);
	glutSolidSphere(0.06, 20, 50);
	glPopMatrix();

	//back leg
	glPushMatrix();
	glTranslatef(front2, 0, -8);
	glRotatef(theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();

	//front leg
	glPushMatrix();
	glTranslatef(front2, 0, -8);
	glRotatef(-theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();
	////////////////////////////man 2///////////////////////////////
	glPushMatrix();
	glTranslatef(0.5, 0, -0.5);
	//body
	glPushMatrix();
	glTranslatef(front2, 0.15, -8);
	glScalef(1, 3, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	// face
	glPushMatrix();
	glTranslatef(front2, 0.4, -8);
	glScalef(1, 2, 1);
	glutSolidSphere(0.06, 20, 50);
	glPopMatrix();

	//back leg
	glPushMatrix();
	glTranslatef(front2, 0, -8);
	glRotatef(theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();

	//front leg
	glPushMatrix();
	glTranslatef(front2, 0, -8);
	glRotatef(-theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();

	///////////////////////////////man 3/////////////////////////
	glPushMatrix();
	glTranslatef(0.5, 0, 0.5);
	//body
	glPushMatrix();
	glTranslatef(front2, 0.15, -8);
	glScalef(1, 3, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	// face
	glPushMatrix();
	glTranslatef(front2, 0.4, -8);
	glScalef(1, 2, 1);
	glutSolidSphere(0.06, 20, 50);
	glPopMatrix();

	//back leg
	glPushMatrix();
	glTranslatef(front2, 0, -8);
	glRotatef(theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();

	//front leg
	glPushMatrix();
	glTranslatef(front2, 0, -8);
	glRotatef(-theta, 0, 0, 1);
	glScalef(1, 4, 0.2);
	glTranslatef(0, -0.05, 0);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();
}


