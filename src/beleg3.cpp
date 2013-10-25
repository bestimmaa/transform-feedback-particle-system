// Computergrafik - WiSe 2011/2012
// Christoph Halang (#90463)
// Beleg Nr. 2

// Camera Controls:
// UP		Move Forward in XZ Plane (Free Cam)
// DOWN		Move Backward XZ Plane (Free Cam)
// Mouse	Drag And Hold -> Change Camera Rotation
// W / S	Particle Lifetime
// A / D	Particle Number
// Q / E	Particle Speed
// C		Comet Cam
// 1-9		Planet Cam
// 0		Free Cam

//////////////////////////////////////////////////////////////////////////
//Sources: http://openglbook.com/
////////// http://www.dhpoware.com/demos/glObjViewer.html
////////// http://www.arcsynthesis.org/gltut/
/////////////////////////////////////////////////////////////////////////
#define GLEW_STATIC

#define BUFFER_OFFSET(i) ((char *)NULL + (i)) // used for offset in VertexAttrPointer()

#define FREE 0
#define MERCURY 1
#define VENUS 2
#define EARTH 3
#define MARTIAN 4
#define JUPITER 5
#define SATURN 6
#define URANUS 7
#define NEPTUNE 8
#define PLUTO 9
#define COMET 10


#include <iostream>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif#include <gloostMath.h>
#include <gloostGlUtil.h>
#include <Shader.h>
#include "Texture.h"
#include "Particles.h"
#include <math.h>
#include <cstdio>
// include gloost::Mesh wich is a geometry container
#include <Mesh.h>
gloost::Mesh* mesh = 0;

// loader for the wavefront *.obj file format
#include <ObjLoader.h>




int CurrentWidth = 800, CurrentHeight = 600, WindowHandle = 0;

unsigned FrameCount = 0;

unsigned ProjectionMatrixUniformLocation = 0;
unsigned ModelViewMatrixUniformLocation  = 0;
unsigned NormalMatrixUniformLocation     = 0;
unsigned textureUniformLocation1         = 0;		//Uniform Texture
unsigned timeUniformLocation         = 0;
unsigned ModelMatrixUniformLocation = 0;


// Particle Uniform Locations

unsigned ParticleModelViewMatrixUniformLocation = 0;
unsigned ParticleProjectionMatrixUniformLocation = 0;

Texture* g_texture1 = 0;
Texture* g_texture2 = 0;
Texture* g_texture3 = 0;
Texture* g_texture4 = 0;
Texture* g_texture5 = 0;
Texture* g_texture6 = 0;
Texture* g_texture7 = 0;
Texture* g_texture8 = 0;
Texture* g_texture9 = 0;
Texture* g_texture10 = 0;
Texture* g_texture11 = 0;


unsigned BufferIds[8] = { 0u };
unsigned ShaderIds[6] = { 0u };

//the three different matrices for projection, viewing and model transforming
#include <Matrix.h>
gloost::Matrix ProjectionMatrix;

#include <MatrixStack.h>
gloost::MatrixStack ModelViewMatrixStack;


//Function callbacks
void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void IdleFunction(void);
void TimerFunction(int);
void Cleanup(void);
void LoadModel(void);
void SetupShader();
void Draw(void);
void RenderFunction(void);
void setCamera(int cameraPreset);

// Controls
void MouseFunction(int button, int state, int x, int y);
void MouseMovementFunction(int x, int y);
void KeyboardFunction(unsigned char key, int x, int y);
void KeyboardSpecialKeysFunction (int key, int x, int y);


// for storing camera transformation from inside the callback functions

gloost::Matrix cameraTransform;

float cameraTranslateX = 0;
float cameraTranslateY = 0;
float cameraTranslateZ = 5;
float cameraRotateX = 0;
float cameraRotateY = 0;

float mouseDistanceY = 0;
float mouseDistanceX = 0;
float lastMousePositionY = 0;
float lastMousePositionX = 0;

Particles myParticles; // simple particle system for comet trail
Particles myStars;

gloost::Vector3 cometPosition = gloost::Vector3(-5.0,0.0,0.0); 
gloost::Vector3 cometDirection = gloost::Vector3(0.05,0.001,0.0);
gloost::Vector3 cameraTranslation = gloost::Vector3(1.0,0.0,0.0);


int cameraMode = FREE;
float lifetimeOfParticles = 1.0;
float particleSpeed = 6.0;
int numberOfParticles = 10000;



/////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])
{

	Initialize(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}


/////////////////////////////////////////////////////////////////////////////////////////


//called every frame this functions draw
void Draw(void)
{
	int now = glutGet(GLUT_ELAPSED_TIME);

	// Rotation
	float rotation = now*0.001;

	//glUseProgram(ShaderIds[0]);

	glEnable(GL_TEXTURE_2D); // enable texture mapping
	glActiveTexture(GL_TEXTURE0); // set texture layer 0
	glUniform1i(textureUniformLocation1, 0); //  Specify the value of text uniform variable for the current program object
	glUniform1i(timeUniformLocation, rotation); //  Specify the value of text uniform variable for the current program object


	//myParticles.createParticles(10000);

	// Basic camera setup

	float cameraOffset = 1.5;

	float rotationSpeedMercury = 0.2;
	float offsetMercury = 1.0;
	gloost::Vector3 planetPositionMercury = gloost::Vector3(cos(-rotation*rotationSpeedMercury)*offsetMercury,0,sin(-rotation*rotationSpeedMercury)*offsetMercury);
	gloost::Vector3 cameraPositionMercury = gloost::Vector3(cos(-rotation*rotationSpeedMercury)*(offsetMercury+cameraOffset),0,sin(-rotation*rotationSpeedMercury)*(offsetMercury+cameraOffset));

	float rotationSpeedVenus = 1.0;
	float offsetVenus = 1.3;
	gloost::Vector3 planetPositionVenus = gloost::Vector3(cos(-rotation*rotationSpeedVenus)*offsetVenus,0,sin(-rotation*rotationSpeedVenus)*offsetVenus);
	gloost::Vector3 cameraPositionVenus = gloost::Vector3(cos(-rotation*rotationSpeedVenus)*(offsetVenus+cameraOffset),0,sin(-rotation*rotationSpeedVenus)*(offsetVenus+cameraOffset));

	float rotationSpeedEarth = 0.8;
	float offsetEarth = 1.7;
	gloost::Vector3 planetPositionEarth = gloost::Vector3(cos(-rotation*rotationSpeedEarth)*offsetEarth,0,sin(-rotation*rotationSpeedEarth)*offsetEarth);
	gloost::Vector3 cameraPositionEarth = gloost::Vector3(cos(-rotation*rotationSpeedEarth)*(offsetEarth+cameraOffset),0,sin(-rotation*rotationSpeedEarth)*(offsetEarth+cameraOffset));

	float rotationSpeedMartian = 0.2;
	float offsetMartian = 2.0;
	gloost::Vector3 planetPositionMartian = gloost::Vector3(cos(-rotation*rotationSpeedMartian)*offsetMartian,0,sin(-rotation*rotationSpeedMartian)*offsetMartian);
	gloost::Vector3 cameraPositionMartian = gloost::Vector3(cos(-rotation*rotationSpeedMartian)*(offsetMartian+cameraOffset),0,sin(-rotation*rotationSpeedMartian)*(offsetMartian+cameraOffset));

	float rotationSpeedJupiter = 0.3;
	float offsetJupiter = 5;
	gloost::Vector3 planetPositionJupiter = gloost::Vector3(cos(-rotation*rotationSpeedJupiter)*offsetJupiter,0,sin(-rotation*rotationSpeedJupiter)*offsetJupiter);
	gloost::Vector3 cameraPositionJupiter = gloost::Vector3(cos(-rotation*rotationSpeedJupiter)*(offsetJupiter+cameraOffset),0,sin(-rotation*rotationSpeedJupiter)*(offsetJupiter+cameraOffset));

	float rotationSpeedSaturn = 0.65;
	float offsetSaturn = 6.0;
	gloost::Vector3 planetPositionSaturn = gloost::Vector3(cos(-rotation*rotationSpeedSaturn)*offsetSaturn,0,sin(-rotation*rotationSpeedSaturn)*offsetSaturn);
	gloost::Vector3 cameraPositionSaturn = gloost::Vector3(cos(-rotation*rotationSpeedSaturn)*(offsetSaturn+cameraOffset),0,sin(-rotation*rotationSpeedSaturn)*(offsetSaturn+cameraOffset));

	float rotationSpeedUranus = 0.45;
	float offsetUranus = 7.0;
	gloost::Vector3 planetPositionUranus = gloost::Vector3(cos(-rotation*rotationSpeedUranus)*offsetUranus,0,sin(-rotation*rotationSpeedUranus)*offsetUranus);
	gloost::Vector3 cameraPositionUranus = gloost::Vector3(cos(-rotation*rotationSpeedUranus)*(offsetUranus+cameraOffset),0,sin(-rotation*rotationSpeedUranus)*(offsetUranus+cameraOffset));

	float rotationSpeedNeptun = 0.35;
	float offsetNeptun = 9.0;
	gloost::Vector3 planetPositionNeptun = gloost::Vector3(cos(-rotation*rotationSpeedNeptun)*offsetNeptun,0,sin(-rotation*rotationSpeedNeptun)*offsetNeptun);
	gloost::Vector3 cameraPositionNeptun = gloost::Vector3(cos(-rotation*rotationSpeedNeptun)*(offsetNeptun+cameraOffset),0,sin(-rotation*rotationSpeedNeptun)*(offsetNeptun+cameraOffset));

	float rotationSpeedPluto = 0.8;
	float offsetPluto = 22.0;
	gloost::Vector3 planetPositionPluto = gloost::Vector3(cos(-rotation*rotationSpeedPluto)*offsetPluto,0,sin(-rotation*rotationSpeedPluto)*offsetPluto);
	gloost::Vector3 cameraPositionPluto = gloost::Vector3(cos(-rotation*rotationSpeedPluto)*(offsetPluto+cameraOffset),0,sin(-rotation*rotationSpeedPluto)*(offsetPluto+cameraOffset));

	cameraTransform.setIdentity();



	switch (cameraMode){

	case FREE:
		cameraTransform.setRotate(cameraRotateX,cameraRotateY,0);
		cameraTransform.setTranslate(cameraTranslateX,cameraTranslateY,cameraTranslateZ);
		break;
	case COMET:
		cameraTransform.setRotate(cameraRotateX,cameraRotateY,0);
		cameraTransform.setTranslate(cometPosition[0],cometPosition[1],cometPosition[2]+3);
		break;

	case MERCURY:
		cameraTransform.setRotate(0,rotation*rotationSpeedMercury+gloost::PI*0.5,0);
		cameraTransform.setTranslate(cameraPositionMercury);
		break;
	case VENUS:
		cameraTransform.setRotate(0,rotation*rotationSpeedVenus+gloost::PI*0.5,0);
		cameraTransform.setTranslate(cameraPositionVenus);
		break;
	case EARTH:
		cameraTransform.setRotate(0,rotation*rotationSpeedEarth+gloost::PI*0.5,0);
		cameraTransform.setTranslate(cameraPositionEarth);
		break;
	case MARTIAN:
		cameraTransform.setRotate(0,rotation*rotationSpeedMartian+gloost::PI*0.5,0);
		cameraTransform.setTranslate(cameraPositionMartian);
		break;

	case JUPITER:
		cameraTransform.setRotate(0,rotation*rotationSpeedJupiter+gloost::PI*0.5,0);
		cameraTransform.setTranslate(cameraPositionJupiter);
		break;

	case SATURN:
		cameraTransform.setRotate(0,rotation*rotationSpeedSaturn+gloost::PI*0.5,0);
		cameraTransform.setTranslate(cameraPositionSaturn);
		break;

	case URANUS:
		cameraTransform.setRotate(0,rotation*rotationSpeedUranus+gloost::PI*0.5,0);
		cameraTransform.setTranslate(cameraPositionUranus);
		break;

	case NEPTUNE:
		cameraTransform.setRotate(0,rotation*rotationSpeedNeptun+gloost::PI*0.5,0);
		cameraTransform.setTranslate(cameraPositionNeptun);
		break;

	case PLUTO:
		cameraTransform.setRotate(0,rotation*rotationSpeedPluto+gloost::PI*0.5,0);
		cameraTransform.setTranslate(cameraPositionPluto);
		break;


	default:
		cameraTransform.setRotate(cameraRotateX,cameraRotateY,0);
		cameraTransform.setTranslate(cameraTranslateX,cameraTranslateY,cameraTranslateZ);
		break;
	}

	cameraTransform.invert();





	//reset the modelmatrix
	ModelViewMatrixStack.clear();
	ModelViewMatrixStack.loadMatrix(cameraTransform);

	gloost::Matrix normalMatrix;

	glUseProgram(ShaderIds[3]);

	ModelViewMatrixStack.push();
	{
		

		ModelViewMatrixStack.translate(0.0,0.0,0.0);

		//std::cout<<"Uniform location for MVM: "<<ParticleModelViewMatrixUniformLocation<<std::endl;

		glUniformMatrix4fv(ParticleModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		glBindVertexArray(BufferIds[5]);

		glPointSize(1);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glDrawArrays(GL_POINTS,0,myStars.getParticles().size());
		glDisable(GL_BLEND);
		glBindVertexArray(0);
	}

	ModelViewMatrixStack.pop();

	glUseProgram(0);



	glUseProgram(ShaderIds[0]);
	// SONNE 

	//save the current transformation onto the MatrixStack
	ModelViewMatrixStack.push(); 
	{

		ModelViewMatrixStack.translate(0,0,0);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);

		g_texture1->bind();

		
		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);




	}

	ModelViewMatrixStack.pop();


	// MERKUR

	//save the current transformation onto the MatrixStack
	ModelViewMatrixStack.push();
	{

		//new planet movement

		

		ModelViewMatrixStack.translate(planetPositionMercury);
		
		
		//old planet movement
		//ModelViewMatrixStack.rotate(0,rotation*0.2,0);
		//ModelViewMatrixStack.translate(1.0,0.0,0.0);
		
		
		ModelViewMatrixStack.scale(0.1);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);

		g_texture2->bind();
		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);




	}




	ModelViewMatrixStack.pop(); // Pop Merkur


	






	/*--------------------------------------------------------------------------------------*/

	// VENUS

	ModelViewMatrixStack.push();
	{

		
		ModelViewMatrixStack.translate(planetPositionVenus);
		ModelViewMatrixStack.scale(0.3);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);

		g_texture3->bind();


		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);




	}




	ModelViewMatrixStack.pop();
	/*--------------------------------------------------------------------------------------*/

	// ERDE


	ModelViewMatrixStack.push();
	{

		
		ModelViewMatrixStack.translate(planetPositionEarth);
		ModelViewMatrixStack.scale(0.3);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);

		g_texture4->bind();

		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);




	}


	// Erde - Mond

	ModelViewMatrixStack.push();
	{

		ModelViewMatrixStack.rotate(0,rotation*0.5,0);
		ModelViewMatrixStack.translate(1,-0.01,0);
		ModelViewMatrixStack.scale(0.3);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);


		g_texture5->bind();

		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);




	}

	ModelViewMatrixStack.pop(); // Mond
	ModelViewMatrixStack.pop(); // Erde

	/*--------------------------------------------------------------------------------------*/
	// Mars


	ModelViewMatrixStack.push();
	{

		
		ModelViewMatrixStack.translate(planetPositionMartian);
		ModelViewMatrixStack.scale(0.15);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);

		g_texture6->bind();
		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);




	}

	ModelViewMatrixStack.pop();

	/*--------------------------------------------------------------------------------------*/

	// JUPITER

	ModelViewMatrixStack.push();
	{

		
		ModelViewMatrixStack.translate(planetPositionJupiter);
		ModelViewMatrixStack.scale(0.15);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);

		g_texture7->bind();
		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);




	}

	ModelViewMatrixStack.pop();

	/*--------------------------------------------------------------------------------------*/

	// Saturn

	ModelViewMatrixStack.push();
	{

		
		ModelViewMatrixStack.translate(planetPositionSaturn);
		ModelViewMatrixStack.scale(0.99);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);

		g_texture8->bind();
		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);




	}

	ModelViewMatrixStack.pop();

	/*--------------------------------------------------------------------------------------*/

	// Uranus

	ModelViewMatrixStack.push();
	{

		
		ModelViewMatrixStack.translate(planetPositionUranus);
		ModelViewMatrixStack.scale(0.3);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);

		g_texture9->bind();

		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);




	}

	ModelViewMatrixStack.pop();

	/*--------------------------------------------------------------------------------------*/

	// Neptun

	ModelViewMatrixStack.push();
	{

		
		ModelViewMatrixStack.translate(planetPositionNeptun);
		ModelViewMatrixStack.scale(0.5);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);

		g_texture10->bind();
		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);




	}

	ModelViewMatrixStack.pop();

	/*--------------------------------------------------------------------------------------*/


	// Pluto
	ModelViewMatrixStack.push();
	{

		;
		ModelViewMatrixStack.translate(planetPositionPluto);
		ModelViewMatrixStack.scale(0.05);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);

		g_texture11->bind();

		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);




	}

	ModelViewMatrixStack.pop();









	// Comet

	//save the current transformation onto the MatrixStack
	ModelViewMatrixStack.push();
	{

		//ModelViewMatrixStack.rotate(0,rotation*0.2,0);




		cometPosition = cometPosition + cometDirection;
		if(cometPosition.length() > 14.0){
			cometPosition = gloost::Vector3(-5.0,0.0,2.0);
			cometDirection = gloost::Vector3(gloost::getRandomMinMax(0.04,0.05),gloost::getRandomMinMax(0.0,0.03),gloost::getRandomMinMax(0.0,0.03));
		}


		ModelViewMatrixStack.translate(cometPosition[0],cometPosition[1],cometPosition[2]);
		ModelViewMatrixStack.scale(0.1);
		// transfer ModelViewMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		//set the NormalMatrix for Geometry 1
		normalMatrix = ModelViewMatrixStack.top();
		normalMatrix.invert();
		normalMatrix.transpose();

		// transfer NormalMatrix for Geometry 1 to Shaders
		glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

		//bind the Geometry
		glBindVertexArray(BufferIds[0]);

		g_texture2->bind();



		// draw Geometry 1
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);

		


	}


	glUseProgram(ShaderIds[3]);

	ModelViewMatrixStack.push();
	{
		myParticles.updateParticles(cometDirection*-1.0);

		ModelViewMatrixStack.translate(0.3,0,0);

		//std::cout<<"Uniform location for MVM: "<<ParticleModelViewMatrixUniformLocation<<std::endl;

		glUniformMatrix4fv(ParticleModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

		glBindVertexArray(BufferIds[4]);

		glBindBuffer(GL_ARRAY_BUFFER, BufferIds[3]);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(float)*8*myParticles.getParticles().size(),
			&myParticles.getParticles().front(),
			GL_STATIC_DRAW);
		glPointSize(3);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glDrawArrays(GL_POINTS,0,myParticles.getParticles().size());
		glDisable(GL_BLEND);
		glBindVertexArray(0);
	}

	ModelViewMatrixStack.pop();


}


/////////////////////////////////////////////////////////////////////////////////////////
void TimerFunction(int value){
	if(0 != value)
	{
		int fps = FrameCount * 4;
		glutSetWindowTitle( (gloost::toString(fps) + " fps").c_str());

	}
	FrameCount = 0;
	glutTimerFunc(250, TimerFunction, 1);
}


void RenderFunction(void)
{
	++FrameCount;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Draw();

	glutSwapBuffers();
	glutPostRedisplay();
}



// MOUSE AND KEYBOARD FUNCTIONS

void MouseFunction(int button, int state, int x, int y){
	//std::printf("Mouse Button %d pressed at: x: %d, y: %d with state %d \n", button,x,y,state);

	if (state == 0){
		lastMousePositionY = y;
		lastMousePositionX = x;
	}

	if (state == 1){
		//std::printf("Mouse moved with Distance %f on Y-Axis and %f on X-Axis",mouseDistanceY, mouseDistanceX);
	}
}


void MouseMovementFunction(int x, int y){
	//std::printf("Mouse moved: x: %d, y: %d  \n", x,y);


	mouseDistanceY = lastMousePositionY-y;
	cameraRotateX = cameraRotateX + ((mouseDistanceY / (float)CurrentHeight)*0.1);

	gloost::clamp<float>(cameraRotateX,-0.5,0.5); 

	mouseDistanceX = lastMousePositionX-x;
	cameraRotateY = cameraRotateY +(( mouseDistanceX / (float)CurrentHeight)*0.1);

	gloost::clamp<float>(cameraRotateY,-0.3,0.3);

	cameraTranslation = gloost::Vector3(sin(cameraRotateY),sin(cameraRotateX),cos(cameraRotateY));
	//std::cout<< "X Translation "<<cameraTranslation[0]<<std::endl;
	//std::cout<< "Z Translation "<<cameraTranslation[2]<<std::endl;
}

void KeyboardFunction(unsigned char key, int x, int y){
	//std::printf("Key %d with mouse at x: %d, y: %d  \n",key, x,y);


	switch (key){
	
	case 114: // R - Reset Camera
		cameraRotateX = 0.0;
		cameraRotateY = 0.0;
		cameraTranslateX = 0.0;
		cameraTranslateY = 0.0;
		cameraTranslateZ = 4.0;
		break;

	case 99: // C - Comet Cam
		 cameraMode = COMET;
		 break;
	case 119: // W - Lifetime Up
		lifetimeOfParticles += 0.2;
		if(lifetimeOfParticles > 10.0) lifetimeOfParticles = 10.0;
		myParticles.setLifetime(lifetimeOfParticles);
		std::cout<<"Lifetime: "<<lifetimeOfParticles<<std::endl;
		break;
	case 115: // S - Lifetime Down
		lifetimeOfParticles -= 0.2;
		if(lifetimeOfParticles < 0.2) lifetimeOfParticles = 0.2;
		myParticles.setLifetime(lifetimeOfParticles);
		std::cout<<"Lifetime: "<<lifetimeOfParticles<<std::endl;
		break;

	case 97: // A - More Particels
		numberOfParticles -=1000;
		if (numberOfParticles < 1) numberOfParticles = 1;
		myParticles.createParticles(numberOfParticles);
		std::cout<<"Number Particles: "<<numberOfParticles<<std::endl;
		break;
	case 100: // D - Less Particles
		numberOfParticles +=1000;
		if (numberOfParticles > 100000) numberOfParticles = 100000;
		myParticles.createParticles(numberOfParticles);
		std::cout<<"Number Particles: "<<numberOfParticles<<std::endl;
		break;
	case 113: // Q - Slower Particels
		particleSpeed -= 0.1;
		if(particleSpeed < 0.1)particleSpeed = 0.1;
		myParticles.setSpeed(particleSpeed);
		std::cout<<"Particle Speed: "<<particleSpeed<<std::endl;
		break;
	case 101: // E - Faster Particles
		particleSpeed += 2;
		if(particleSpeed > 100.0)particleSpeed = 100.0;
		myParticles.setSpeed(particleSpeed);
		std::cout<<"Particle Speed: "<<particleSpeed<<std::endl;
		break;
	default:
		cameraMode = key - 48;
		break;




	}


	

	
}

void KeyboardSpecialKeysFunction (int key, int x, int y){
	//std::printf("Key %d with mouse at x: %d, y: %d  \n",key, x,y);

	if (key == GLUT_KEY_UP){

		//std::printf("UP!\n");
		cameraTranslateZ -= cameraTranslation[2];
		cameraTranslateX -= cameraTranslation[0];

	}

	/*

	if (key == GLUT_KEY_LEFT){

	std::printf("LEFT!\n");
	cameraTranslateX += cameraTranslation[0];

	}
	*/
	if (key == GLUT_KEY_DOWN){

		//std::printf("DOWN!\n");
		cameraTranslateZ += cameraTranslation[2];
		cameraTranslateX += cameraTranslation[0];

	}


	/*
	if (key == GLUT_KEY_RIGHT){

	std::printf("RIGHT!\n");
	cameraTranslateX -= cameraTranslation[0];
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////////////////


void SetupShader()
{
	// LOAD AND LINK SHADER
	ShaderIds[0] = glCreateProgram();
	{
		//takes a (shader) filename and a shader-type and returns and id of the compiled shader
		ShaderIds[1] = Shader::loadShader("../../../data/shaders/myVertexShader.vs", GL_VERTEX_SHADER);
		ShaderIds[2] = Shader::loadShader("../../../data/shaders/myPlaygroundFragmentShader.fs", GL_FRAGMENT_SHADER);

		//attaches a shader to a program
		glAttachShader(ShaderIds[0], ShaderIds[1]);
		glAttachShader(ShaderIds[0], ShaderIds[2]);
	}
	glLinkProgram(ShaderIds[0]);

	//describes how the uniforms in the shaders are named and to which shader they belong

	//Reference:

	/*glGetUniformLocation returns an integer that represents the location of a specific uniform variable within
	a program object. name must be a null terminated string that contains no white space.
	name must be an active uniform variable name in program that is not a structure, an array of structures,
	or a subcomponent of a vector or a matrix.
	This function returns -1 if name does not correspond to an active uniform variable in program or
	if name starts with the reserved prefix "gl_".*/


	ModelViewMatrixUniformLocation  = glGetUniformLocation(ShaderIds[0], "ModelViewMatrix"); // glGetUniformLocation — Returns the location of a uniform variable
	ProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ProjectionMatrix");
	NormalMatrixUniformLocation     = glGetUniformLocation(ShaderIds[0], "NormalMatrix");
	ModelMatrixUniformLocation      = glGetUniformLocation(ShaderIds[0], "ModelMatrix");
	textureUniformLocation1         = glGetUniformLocation(ShaderIds[0], "colorMap");
	timeUniformLocation             = glGetUniformLocation(ShaderIds[0], "now"); 




	// LOAD AND LINK SHADER
	ShaderIds[3] = glCreateProgram();


	{
		//takes a (shader) filename and a shader-type and returns and id of the compiled shader
		ShaderIds[4] = Shader::loadShader("../../../data/shaders/myParticleVertexShader.vs", GL_VERTEX_SHADER);
		ShaderIds[5] = Shader::loadShader("../../../data/shaders/myParticleFragmentShader.fs", GL_FRAGMENT_SHADER);

		//attaches a shader to a program
		glAttachShader(ShaderIds[3], ShaderIds[4]);
		glAttachShader(ShaderIds[3], ShaderIds[5]);
	}
	glLinkProgram(ShaderIds[3]);

	ParticleModelViewMatrixUniformLocation  = glGetUniformLocation(ShaderIds[3], "ParticleModelViewMatrix"); // glGetUniformLocation — Returns the location of a uniform variable
	ParticleProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[3], "ParticleProjectionMatrix");

	glUseProgram(0);
}


/////////////////////////////////////////////////////////////////////////////////////////


void LoadModel()
{

	//load a wavefront *.obj file
	gloost::ObjLoader loader("../../../data/objects/sphere_mod.obj");
	mesh = loader.getMesh();

	//IMPORTANT: use this to increase the reference counter
	//gloost::meshes have a garbage collector which throws
	//the mesh away otherwise
	mesh->takeReference();

	mesh->generateNormals();

	//normalizes the mesh
	mesh->scaleToSize(1.0);
	//puts the meshdata in one array
	mesh->interleave();

	mesh->printMeshInfo();

	//create VAO which holds the state of our Vertex Attributes and VertexBufferObjects - a control structure
	//note: for different objects more of these are needed
	glGenVertexArrays(1, &BufferIds[0]); // generate a single VertexArray and safe it to position 0 of BufferIDs
	//bind VertexArray - Scope begins
	glBindVertexArray(BufferIds[0]);


	//Create two VertexBufferObject and bind the first one to set its data
	glGenBuffers(2, &BufferIds[1]); 
	glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);

	//set the vertex data for the actual buffer; the second parameter is the size in bytes of all Vertices together
	//the third parameter is a pointer to the vertexdata
	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml

	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(float) * mesh->getInterleavedAttributes().size(),
		&mesh->getInterleavedAttributes().front(),
		GL_STATIC_DRAW);


	//enables a VertexAttributeArray
	//http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml

	glEnableVertexAttribArray(0);

	//specifies where in the GL_ARRAY_BUFFER our data(the vertex position) is exactly
	//
	glVertexAttribPointer(0,
		GLOOST_MESH_NUM_COMPONENTS_VERTEX,
		GL_FLOAT, GL_FALSE,
		mesh->getInterleavedInfo().interleavedPackageStride,//mesh->getInterleavedInfo().interleavedVertexStride,
		(GLvoid*)(mesh->getInterleavedInfo().interleavedVertexStride));


	//enables a VertexAttributeArray
	glEnableVertexAttribArray(1);

	//specifies where in the GL_ARRAY_BUFFER our data(the vertex position) is exactly
	glVertexAttribPointer(1,
		GLOOST_MESH_NUM_COMPONENTS_NORMAL,
		GL_FLOAT, GL_FALSE,
		mesh->getInterleavedInfo().interleavedPackageStride,
		(GLvoid*)(mesh->getInterleavedInfo().interleavedNormalStride));




	// the seceond VertexBufferObject ist bound
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
	// its data are the indices of the vertices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(gloost::TriangleFace) * mesh->getTriangles().size(),
		&mesh->getTriangles().front(),
		GL_STATIC_DRAW);



	// load texture data (myVertexShader: layout(location=2) in vec2 in_Texcoord)

	glEnableVertexAttribArray(2);

	//specifies where in the GL_ARRAY_BUFFER our data(the vertex position) is exactly
	glVertexAttribPointer(2,
		GLOOST_MESH_NUM_COMPONENTS_TEXCOORD,
		GL_FLOAT, GL_FALSE,
		mesh->getInterleavedInfo().interleavedPackageStride,
		(GLvoid*)(mesh->getInterleavedInfo().interleavedTexcoordStride));



	// the seceond VertexBufferObject ist bound
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
	// its data are the indices of the vertices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(gloost::TriangleFace) * mesh->getTriangles().size(),
		&mesh->getTriangles().front(),
		GL_STATIC_DRAW);

	glBindVertexArray(0);

	// Buffers for particles

	glGenVertexArrays(1, &BufferIds[4]); // generate a single VertexArray and safe it to position 4 of BufferIDs
	//bind VertexArray - Scope begins
	glBindVertexArray(BufferIds[4]);


	glGenBuffers(1,&BufferIds[3]);
	glBindBuffer(GL_ARRAY_BUFFER, BufferIds[3]);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(float)*8*myParticles.getParticles().size(),
		&myParticles.getParticles().front(),
		GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(Particle),BUFFER_OFFSET(0)); // position xyz and lifetime w


	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Particle),BUFFER_OFFSET(sizeof(float)*4)); // color xyzw -> rgba

	glBindBuffer(GL_ARRAY_BUFFER,0);


	glBindVertexArray(0);

	
	// Buffers for particles
	
	glGenVertexArrays(1, &BufferIds[5]); // generate a single VertexArray and safe it to position 4 of BufferIDs
	//bind VertexArray - Scope begins
	
	glBindVertexArray(BufferIds[5]);

	glGenBuffers(1,&BufferIds[6]);
	glBindBuffer(GL_ARRAY_BUFFER, BufferIds[6]);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(float)*8*myStars.getParticles().size(),
		&myStars.getParticles().front(),
		GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(Particle),BUFFER_OFFSET(0)); // position xyz and lifetime w


	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Particle),BUFFER_OFFSET(sizeof(float)*4)); // color xyzw -> rgba

	glBindBuffer(GL_ARRAY_BUFFER,0);


	glBindVertexArray(0);
	/**/
	


}


/////////////////////////////////////////////////////////////////////////////////////////


void Cleanup()
{
	glDetachShader(ShaderIds[0], ShaderIds[1]);
	glDetachShader(ShaderIds[0], ShaderIds[2]);
	glDetachShader(ShaderIds[3], ShaderIds[4]);
	glDetachShader(ShaderIds[3], ShaderIds[5]);
	glDeleteShader(ShaderIds[1]);
	glDeleteShader(ShaderIds[2]);
	glDeleteShader(ShaderIds[4]);
	glDeleteShader(ShaderIds[5]);
	glDeleteProgram(ShaderIds[0]);
	glDeleteProgram(ShaderIds[3]);


	glDeleteBuffers(3, &BufferIds[1]);
	glDeleteVertexArrays(1, &BufferIds[0]);
	glDeleteVertexArrays(1, &BufferIds[4]);
}


/////////////////////////////////////////////////////////////////////////////////////////


void IdleFunction(void)
{
	glutPostRedisplay();
}


/////////////////////////////////////////////////////////////////////////////////////////


void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);

	gloost::gloostPerspective(ProjectionMatrix,
		60.0f,
		(float)CurrentWidth / CurrentHeight,
		1.0f,
		100.0f
		);

	glUseProgram(ShaderIds[0]);
	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix.data());
	glUseProgram(ShaderIds[3]);
	glUniformMatrix4fv(ParticleProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix.data());
	glUseProgram(0);
}


/////////////////////////////////////////////////////////////////////////////////////////


void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);

	glutInitWindowSize(CurrentWidth, CurrentHeight);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	WindowHandle = glutCreateWindow("");

	if(WindowHandle < 1)
	{
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);
		glutExit();
	}

	//Glut function callbacks
	glutTimerFunc(0, TimerFunction, 0);
	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(IdleFunction);
	glutMouseFunc(MouseFunction);
	glutMotionFunc(MouseMovementFunction);
	glutKeyboardFunc(KeyboardFunction);
	glutSpecialFunc(KeyboardSpecialKeysFunction);
}

/////////////////////////////////////////////////////////////////////////////////////////

void Initialize(int argc, char* argv[])
{
	GLenum GlewInitResult;

	InitWindow(argc, argv);

	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult)
	{
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
			);
		glutExit();
	}

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
		);

	glGetError();
	glClearColor(0.10f, 0.10f, 0.10f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	ModelViewMatrixStack.loadIdentity();
	ProjectionMatrix.setIdentity();

	myParticles = Particles();

	myParticles.createParticles(12000);

	myStars = Particles();

	myStars.createStars(1000);
	//myParticles.updateParticles();

	SetupShader();
	LoadModel();

	FreeImage_Initialise();

	//Load jpg as texture
	g_texture1 = new Texture("../textures/sun.jpg");
	g_texture2 = new Texture("../textures/merkur.jpg");
	g_texture3 = new Texture("../textures/venus.jpg");
	g_texture4 = new Texture("../textures/earth.jpg");
	g_texture5 = new Texture("../textures/moon.jpg");
	g_texture6 = new Texture("../textures/mars.jpg");
	g_texture7 = new Texture("../textures/jupiter.jpg");
	g_texture8 = new Texture("../textures/saturn.jpg");
	g_texture9 = new Texture("../textures/uranus.jpg");
	g_texture10 = new Texture("../textures/neptun.jpg");
	g_texture11 = new Texture("../textures/pluto.jpg");


}


void setCamera(int cameraPreset){


	switch(cameraPreset){

	case FREE:
		cameraTransform.setIdentity();
		cameraTransform.setRotate(0,gloost::PI,0);
		cameraTransform.setTranslate(0.0,0.0,4.0);
		cameraTransform.invert();

	case COMET:
		cameraTransform.setIdentity();
		cameraTransform.setRotate(cameraRotateX,gloost::PI,0);
		cameraTransform.setTranslate(cometPosition[0],cometPosition[1],cometPosition[2]-3);
		cameraTransform.invert();
		break;

	default:
		cameraTransform.setIdentity();
		cameraTransform.setRotate(cameraRotateX,cameraRotateY,0);
		cameraTransform.setTranslate(cameraTranslateX,cameraTranslateY,cameraTranslateZ);
		cameraTransform.invert();

	}




}