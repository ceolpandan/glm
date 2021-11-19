#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h

#include "loadShaders.h"

#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

using namespace std;

GLuint
VaoId,
VboId,
ColorBufferId,
ProgramId,
myMatrixLocation,
matrScaleLocation,
matrTranslLocation,
matrRotlLocation,
codColLocation;

glm::mat4 myMatrix, resizeMatrix, matrTransl, matrTransl2, matrScale1, matrScale2, matrRot1, matrDepl, matrRot2, matBandaTransl, matMarcajTransl;

float  lenCar1 = 2.5, lenCar2 = 2.5, yMaxDepasire = 200;

int codCol;
float PI = 3.141592, angle = 0;
float tx = 0; float ty = 0;
float width = 2250.0f, height = 1500.0f;
float xMasina2 = 2250.0, xMasina1 = 3000.0, yMasina1 = 0.0, alpha = 0.0, vitezaMasina1 = 10, vitezaMasina2 = 6, latimeBanda = 200, marcaj = 1;

void incrementPositionsIJK(void)
{
	xMasina2 += vitezaMasina2 * alpha;
	xMasina1 += vitezaMasina1 * alpha;

	if (abs(xMasina1 - xMasina2) < 350 && xMasina1 > xMasina2) {
		if (yMasina1 > -yMaxDepasire) {
			yMasina1 += 7 * alpha;
		}
	}

	if (abs(xMasina2 - xMasina1) >= 150 && xMasina1 < xMasina2) {
		if (yMasina1 >= 0)
			yMasina1 = 0;
		else yMasina1 -= 5 * alpha;
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
				alpha = -0.05; glutIdleFunc(incrementPositionsIJK);
			break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
				glutIdleFunc(incrementPositionsIJK);
				xMasina2 = 2250.0;
				xMasina1 = 3000.0;
				yMasina1 = 0.0;
				alpha = 0.0;
				break;
		default:
			break;
	}
}

void CreateVBO(void)
{
	// varfurile 
	GLfloat Vertices[] = {

		// varfuri pentru axe
		 -(width), 0.0f, 0.0f, 1.0f,
		 width,  0.0f, 0.0f, 1.0f,
		 0.0f, 100.0f, 0.0f, 0.0f,
		 0.0f, -100.0f, 0.0f, 0.0f,
		 // varfuri pentru dreptunghi
		-50.0f,  -50.0f, 0.0f, 1.0f,
		50.0f, -50.0f, 0.0f, 1.0f,
		50.0f,  50.0f, 0.0f, 1.0f,
		-50.0f,  50.0f, 0.0f, 1.0f,

		width, 80.0f, 0.0f, 1.0f,
		width, -250.0f, 0.0f, 1.0f


	};



	//// culorile varfurilor din colturi
	//GLfloat Colors[] = {
	//  1.0f, 0.0f, 0.0f, 1.0f,
	//  0.0f, 1.0f, 0.0f, 1.0f,
	//  0.0f, 0.0f, 0.1f, 1.0f
	//};


	glGenBuffers(1, &VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//glGenBuffers(1, &ColorBufferId);
	//glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);


}

void DestroyVBO(void)
{


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);


}

void CreateShaders(void)
{
	ProgramId = LoadShaders("05_02_Shader.vert", "05_02_Shader.frag");
	glUseProgram(ProgramId);
}

void RenderFunction(void) {

	glClear(GL_COLOR_BUFFER_BIT);

	//TODO matricile pentru transformari
	resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / width, 1.f / height, 1.0)); //"patratul standard" [-1,1]x[-1,1]

	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(xMasina2, 0.0, 0.0)); // masina depasita
	matrTransl2 = glm::translate(glm::mat4(1.0f), glm::vec3(xMasina1, yMasina1, 0.0)); // masina care depaseste

	matBandaTransl = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -latimeBanda, 0.0));
	matMarcajTransl = glm::translate(glm::mat4(1.0f), glm::vec3(marcaj, 0.0, 0.0));

	matrScale1 = glm::scale(glm::mat4(1.0f), glm::vec3(lenCar1, 0.9, 0.0));

	matrRot1 = glm::rotate(glm::mat4(1.0f), PI / 8, glm::vec3(0.0, 0.0, 1.0));
	matrRot2 = glm::rotate(glm::mat4(1.0f), PI / 8, glm::vec3(0.0, 0.0, -1.0));

	codColLocation = glGetUniformLocation(ProgramId, "codCol");
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");

	//banda1
	myMatrix = resizeMatrix;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	glLineWidth(latimeBanda);
	glDrawArrays(GL_LINES, 0, 2);

	//banda2
	myMatrix = resizeMatrix * matBandaTransl;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	glLineWidth(latimeBanda);
	glDrawArrays(GL_LINES, 0, 2);

	//marcaje
	marcaj = 1;
	myMatrix = resizeMatrix * matMarcajTransl;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	codCol = 4;
	glUniform1i(codColLocation, codCol);
	glLineWidth(100);
	glDrawArrays(GL_LINES, 8, 2);

	



	//masina depasita
	myMatrix = resizeMatrix * matrTransl * matrScale1;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 4, 4);

	//masina care depaseste
	if (yMasina1 < 0 && yMasina1 > -yMaxDepasire && xMasina1 > xMasina2)
		myMatrix = resizeMatrix * matrTransl2 * matrRot1 * matrScale1;

	else if (yMasina1 < 0 && yMasina1 > -yMaxDepasire && xMasina2 > xMasina1)
		myMatrix = resizeMatrix * matrTransl2 * matrRot2 * matrScale1;

	else myMatrix = resizeMatrix * matrTransl2 * matrScale1;

	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	codCol = 3;
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 4, 4);

	glutSwapBuffers();
	glFlush();
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
	CreateVBO();
	CreateShaders();
}

void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Proiect depasire");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutMouseFunc(mouse);
	glutCloseFunc(Cleanup);

	glutMainLoop();


}