#include "game.h"
using namespace std;

int mode, pid;
int win_width, win_height;
extern bool dual;
void init();
void intro();
void begin();
void end();
void reshape(int, int);
void keyhandle(unsigned char key, int x, int y);
void mousehandle(int button, int state, int x, int y);
void specialhandle(int key, int x, int y);
void mousepassive(int, int);
bool instruction = false;
GLuint texture;

int main(int argc, char** argv) {
	mode = INTRO;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1300, 750);
	glutInitWindowPosition(100, 20);


	pid = glutCreateWindow("WarAndPeace");
	init();
	glutDisplayFunc(intro);

	glutKeyboardFunc(keyhandle);
	glutSpecialFunc(specialhandle);
	glutMouseFunc(mousehandle);
	glutPassiveMotionFunc(mousepassive);

	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}


void init() {
	glClearColor(0.52, 0.80, 0.97, 1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	if (FILE * file = fopen("../res/texture/begin.jpg", "r")) {
		cout << "Texture Loaded from ../../res/texture/" << endl;
		fclose(file);
		texture = LoadTexture("../res/texture/begin.jpg");
	}
	else if (FILE * file = fopen("./res/texture/begin.jpg", "r")) {
		cout << "Texture Loaded from ./res/texture/" << endl;
		fclose(file);
		texture = LoadTexture("./res/texture/begin.jpg");
	}
	else {
		cout << "Couldn't Load Textures from ./res/texture/" << endl;
	}
}

void reshape(int w, int h) {
	win_width = w; win_height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0, (float)w / (float)h, 0.5f, 500);
}


void keyhandle(unsigned char key, int x, int y) {
	switch (mode) {
	case INTRO: if (key == 13) {
		mode = BEGIN;
		glutMouseFunc(mousehandle);
		glutPassiveMotionFunc(mousepassive);
		glutDisplayFunc(begin);
		glutPostRedisplay();
	}break;
	case BEGIN: if (key == 'w') {
		mode = WAR;
		war();
		glutDisplayFunc(war_display);
		reshape(win_width, win_height);
		glutReshapeFunc(reshape);
		glutPostRedisplay();
	}
				else if (key == 'p') {
		mode = PEACE;
		peace();
		glutDisplayFunc(peace_display);
		reshape(win_width, win_height);
		glutReshapeFunc(reshape);
		glutPostRedisplay();
	}
				break;
	case WAR: control(key, x, y); break;
	case PEACE: break;
	}
}
void specialhandle(int key, int x, int y) {
	switch (mode) {
	case INTRO: if (key == GLUT_KEY_RIGHT) {
		mode = BEGIN;
		glutDisplayFunc(begin);
		glutPostRedisplay();
	}
				break;
	case BEGIN: break;
	case WAR: special_control(key, x, y);break;
	case PEACE: break;
	}
}
void mousehandle(int button, int state, int x, int y) {
	float xval = (float)x / (float)win_width * 500.0;
	float yval = (float)y / (float)win_height * 500.0;
	switch (mode) {
	case INTRO: break;
	case BEGIN: if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		if (xval >= 378 && xval <= 403 && yval >= 405 && yval <= 428) {
			mode = WAR;
			war();
			glutDisplayFunc(war_display);
			reshape(win_width, win_height);
			glutReshapeFunc(reshape);
			glutPostRedisplay();
		}
		else if (xval >= 416 && xval <= 455 && yval >= 406 && yval <= 428) {
			mode = PEACE;
			peace();
			glutDisplayFunc(peace_display);
			reshape(win_width, win_height);
			glutReshapeFunc(reshape);
			glutPostRedisplay();
		}
		else if (xval >= 374 && xval <= 462 && yval >= 318 && yval <= 336) {
			if (dual)
				dual = false;
			else
				dual = true;
		}
		else if (xval >= 373 && xval <= 463 && yval >= 362 && yval <= 382) {
			instruction = instruction ? false : true;
		}
				break;
	case WAR: war_mousehandle(button, state, x, y);break;
	case PEACE: if (xval >= 343 && xval <= 397 && yval >= 468 && yval <= 490) {
		mode = BEGIN;
		init();
		glutDisplayFunc(begin);
		reshape(win_width, win_height);
		glutReshapeFunc(reshape);
		glutPostRedisplay();
	}break;
	}
}

void mousepassive(int x, int y) {
	float xval = (float)x / (float)win_width * 500.0;
	float yval = (float)y / (float)win_height * 500.0;
	switch (mode) {
	case INTRO:
	case BEGIN: if ((xval >= 378 && xval <= 403 && yval >= 405 && yval <= 428) ||
		(xval >= 416 && xval <= 455 && yval >= 406 && yval <= 428) ||
		(xval >= 374 && xval <= 462 && yval >= 318 && yval <= 336) ||
		(xval >= 373 && xval <= 463 && yval >= 362 && yval <= 382)) {
		glutSetCursor(GLUT_CURSOR_DESTROY);
	}
				else {
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}break;
	case WAR: if (xval >= 444 && xval <= 499 && yval >= 468 && yval <= 489 || xval >= 386 && xval <= 413 && yval >= 466 && yval <= 488)
		glutSetCursor(GLUT_CURSOR_DESTROY);
			  else
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		break;
	case PEACE: if (xval >= 343 && xval <= 397 && yval >= 468 && yval <= 490)
		glutSetCursor(GLUT_CURSOR_DESTROY);
				else
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		break;
	}
}
void intro() {
	glClearColor(0.52, 0.80, 0.97, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	draw_text2(-1, 1, -1, "BANGALORE INSTITUTE OF TECHNOLOGY");
	glColor3f(1, 0, 0);
	draw_text2(-0.9, 0.8, -1, "DEPARTMENT OF COMPUTER SCIENCE");
	draw_text2(-0.4, 0.6, -1, "AND ENGINEERING");
	glColor3f(1.0, 1.0, 1.0);
	draw_text2(-0.7, 0.3, -1, "AN OPENGL MINI PROJECT ON");
	draw_text2(-0.3, 0.1, -1, "WAR AND PEACE");
	glColor3f(0, 0.5, 0);
	draw_text(-0.2, -0.3, -1, "CLICK ENTER TO START");

	glColor3f(0.0, 0.0, 0.0);
	draw_text2(-2.5, -0.9, -1, "BY:");
	draw_text2(-2.5, -1.1, -1, "SHARATH B P");
	draw_text2(-2.5, -1.3, -1, "1BI16CS139");

	glColor3f(0.0, 0.0, 0.0);
	draw_text2(1.8, -0.9, -1, "Under the guidance of");
	draw_text2(1.8, -1.05, -1, "Prof K.J.Banushree");
	draw_text2(1.8, -1.2, -1, "Prof N Thanuja");
	draw_text2(1.8, -1.35, -1, "Prof Shruthi B R");

	glPopMatrix();

	glutSwapBuffers();
}

void begin() {
	glColor3f(1.0, 1.0, 1.0);
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(0, 0, -0.65);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	glTexCoord2f(0.1, 0.0);
	glVertex3f(-2.5, -1.35, 0.0);
	glTexCoord2f(0.1, 1.0);
	glVertex3f(-2.5, 1.35, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(2.5, 1.35, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(2.5, -1.35, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glColor3f(1.0, 0.0, 0.0);
	draw_text2(-1.6, 0.7, 0.1, "WAR & PEACE");


	glPushMatrix();
	glColor3f(1, 0, 0);
	draw_rectangle(1.0 * win_width / 1350, -0.4, 0.7, 0.1);
	glColor3f(1.0, 1.0, 1.0);
	if (!dual)
		draw_text2(1.0 * win_width / 1550, -0.3, 0.1, "DUAL:OFF");
	else
		draw_text2(1.0 * win_width / 1550, -0.3, 0.1, "DUAL:ON");
	glColor3f(1, 0, 0);
	draw_rectangle(1.0 * win_width / 1350, -0.6, 0.7, 0.1);
	glColor3f(1.0, 1.0, 1.0);
	if (!instruction)
		draw_text2(1.0 * win_width / 1550, -0.5, 0.1, "INSTRUCTIONS");
	else
		draw_text2(1.0 * win_width / 1550, -0.5, 0.1, "CLOSE");
	glPopMatrix();


	glPushMatrix();
	glColor3f(1, 0, 0);
	draw_rectangle(1.0 * win_width / 1350, -0.8, 0.2, 0.1);
	glColor3f(1.0, 1.0, 1.0);
	draw_text2(1.0 * win_width / 1550, -0.64, 0.1, "WAR");
	glTranslatef(0.3, 0, 0);
	glColor3f(1, 0, 0);
	draw_rectangle(1.0 * win_width / 1350, -0.8, 0.3, 0.1);
	glColor3f(1.0, 1.0, 1.0);
	draw_text2(1.0 * win_width / 1550, -0.64, 0.1, "PEACE");
	glPopMatrix();


	if (instruction) {
		draw_rectangle(-0.5, -0.8, 1.3, 0.5);
		glColor3f(1, 0, 0);
		float x1 = -0.45, y1 = -0.35;
		glPushMatrix();
		draw_text(x1, y1, 0.002, "FUNCTION");
		draw_text(x1 + 0.3, y1, 0.002, "PLAYER 1");
		draw_text(x1 + 0.6, y1, 0.002, "PLAYER 2");
		glTranslatef(0, -0.07, 0);
		draw_text(x1, y1, 0.002, "MOVE LEFT");
		draw_text(x1 + 0.3, y1, 0.002, "a");
		draw_text(x1 + 0.6, y1, 0.002, "LEFT ARROW");
		glTranslatef(0, -0.07, 0);
		draw_text(x1, y1, 0.002, "MOVE RIGHT");
		draw_text(x1 + 0.3, y1, 0.002, "d");
		draw_text(x1 + 0.6, y1, 0.002, "RIGHT ARROW");
		glTranslatef(0, -0.07, 0);
		draw_text(x1, y1, 0.002, "SHOOT");
		draw_text(x1 + 0.3, y1, 0.002, "SPACE / w");
		draw_text(x1 + 0.6, y1, 0.002, "UP ARROW / LEFT MOUSE CLICK");

		glPopMatrix();
	}

	glutSwapBuffers();
}
