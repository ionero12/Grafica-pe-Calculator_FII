#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "glut.h"
#include <map>

using namespace std;
unsigned char prevKey;


class GrilaCarteziana {
public:
	int nrLinii;
	int nrColoane;
	double eps;
	double cx; 
	double cy; 
	double dc; 
	double de; 
	double radius;

	GrilaCarteziana(int nrlinii, int nrcol) {
		this->nrLinii = nrlinii;
		this->nrColoane = nrcol;
		this->eps = 0.1;
		this->cx = -1 + this->eps;
		this->cy = -1 + this->eps;
		this->dc = (2 - 2 * this->eps) / (nrcol - 1);
		this->de = (2 - 2 * this->eps) / (nrlinii - 1);
		this->radius = this->dc / 3;
		this->drawColumns();
		this->drawLines();
	}

	void drawLines() {
		glLineWidth(1);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		double p1_x = this->cx;
		double p_y = this->cy;
		double p2_x = -this->cx;

		for (int i = 1; i <= this->nrLinii; i++) {
			glVertex2f(p1_x, p_y);
			glVertex2f(p2_x, p_y);
			p_y += this->de;
		}
		glEnd();
	}

	void drawColumns() {
		glLineWidth(1);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		double p_x = this->cx;
		double p1_y = this->cy;
		double p2_y = 1 - this->eps;

		for (int i = 1; i <= this->nrColoane; i++) {
			glVertex2f(p_x, p1_y);
			glVertex2f(p_x, p2_y);
			p_x += this->dc;
		}
		glEnd();
	}

	void drawCircle(double x, double y, double r, int numberOfSegments) {
		double pi = 4 * atan(1.0);
		glColor3ub(50, 50, 50);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y);

		for (int i = 0; i < numberOfSegments; i++) {
			float x_aux = x + (this->radius * cos(i * 2 * pi / numberOfSegments));
			float y_aux = y + (this->radius * sin(i * 2 * pi / numberOfSegments));
			glVertex2f(x_aux, y_aux);
		}
		glEnd();
	}

	void writePixel(int i, int j) {
		double x = this->cx + i * this->dc;
		double y = this->cy + j * this->de;
		drawCircle(x, y, radius, 10000);
	}

	void ScanConvertCircle3_Aux(double x, double y, map<double, double> m) {
		m[x] = y;
		m[-x] = -y;
		m[-x] = y;
		m[x] = -y;

		if (x != y) {
			m[y] = x;
			m[-y] = -x;
			m[-y] = x;
			m[y] = -x;
		}
	}

	void ScanConvertSegments3(int x0, int y0, int xn, int yn) {
		double dx, dy;
		dx = abs(xn - x0);
		dy = abs(yn - y0);
		int d = 2 * dy - dx;
		int dE = 2 * dy;
		int dNE = 2 * (dy - dx);
		int x = x0, y = y0;
		map<int, int> m;
		m[x] = y;

		if (y < this->nrLinii) {
			this->writePixel(x, y);
		}
		while (x < xn) {
			if (d <= 0) {
				d += dE;
				x++;
			}
			else {
				d += dNE;
				x++;
				if (x0 < xn && y0 < yn) {
					y++;
				}
				else {
					y--;
				}
			}
			if (y < this->nrLinii) {
				this->writePixel(x, y);
			}
			m[x] = y;
		}
	}

	void drawSegment(int x0, int y0, int xn, int yn) {
		glLineWidth(3);
		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINES);
		double x1, y1;
		x1 = this->cx + x0 * this->dc;
		y1 = this->cy + y0 * this->de;
		glVertex2f(x1, y1);
		x1 = this->cx + xn * this->dc;
		y1 = this->cy + yn * this->de;
		glVertex2f(x1, y1);
		glEnd();
	}
};

void Init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT, GL_LINE);
}

void display1() {
	int numarLinii = 16;
	int numarColoane = 16;
	GrilaCarteziana* grilaCarteziana = new GrilaCarteziana(numarLinii, numarColoane);

	grilaCarteziana->drawSegment(0, 15, 15, 10);
	grilaCarteziana->drawSegment(0, 0, 15, 7);

	grilaCarteziana->ScanConvertSegments3(0, 14, 15, 9);
	grilaCarteziana->ScanConvertSegments3(0, 15, 15, 10);
	grilaCarteziana->ScanConvertSegments3(0, 16, 15, 11);

	grilaCarteziana->ScanConvertSegments3(0, 0, 15, 7);
}

void Display(void) {
	printf("Call Display\n");
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {
	case '1':
		display1();
	default:
		break;
	}
	glRasterPos2d(-0.98, -0.98);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');

	glRasterPos2d(-0.98, 0.82);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'y');

	glRasterPos2d(0.81, -0.98);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'x');
	glFlush();
}

void Reshape(int w, int h) {
	printf("Call Reshape : width = %d, height = %d\n", w, h);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	printf("You have pressed <%c>. The mouse has the position %d, %d.\n",
		key, x, y);
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
	printf("Call MouseFunc : you have %s the %s button in the position %d %d\n",
		(state == GLUT_DOWN) ? "pressed" : "released",
		(button == GLUT_LEFT_BUTTON) ?
		"left" :
		((button == GLUT_RIGHT_BUTTON) ? "right" : "middle"),
		x, y);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow(argv[0]);
	Init();
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardFunc);
	glutMouseFunc(MouseFunc);
	glutDisplayFunc(Display);
	glutMainLoop();
	return 0;
}
