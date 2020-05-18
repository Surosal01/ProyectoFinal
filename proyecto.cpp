//BUENISIMO

#include <iostream>
#include <cmath>
#include <iostream>
#include <Windows.h>
// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models carga de modelos
#include "SOIL2/SOIL2.h"

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();
void casa();
void porton();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(15.0f, 10.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float spotAngle = 0.0f;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);// posicion de luz direccional
glm::vec3 PosIni(-95.0f, 1.0f, -45.0f);
glm::vec3 LightP1;
bool active;
bool bill = false;
bool recorrido1 = true;

//--------------CASA ARMABLE---------------------------------------
bool paredDer = false;
bool recorrido2 = true;

////////////////////////////
bool entradaGlesia = false;
bool recorrido3 = true;

/////////////////////
bool paredIzq = false;
bool recorrido4 = true;

/////////////////////
bool tras = false;
bool recorrido5 = true;

/////////////////////
bool picotras = false;
bool recorrido6 = true;

/////////////////////
bool picofre = false;
bool recorrido7 = true;

/////////////////////
bool techoDer = false;
bool recorrido8 = true;

/////////////////////
bool techoIzq = false;
bool recorrido9 = true;

/////////////////////
bool arriba = false;
bool recorrido10 = true;

/////////////////////
bool pico = false;
bool recorrido11 = true;

/////////////////////
bool puerta = false;
bool recorrido12 = true;

/////////////////////
bool bosque = false;

/////////////////////
bool puerta2 = false;
bool recorrido13 = true;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes declaro las partets del cuerpo
float  posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z,rotBill = 0;

//******************movimeinto de la casa armable****************-----------------------------------------------------------------------
float  rotparedDer = 0;
float  rotentrada = 0;
float  rotparIzq = 0;
float  rotTras = 0;

//------------
float  rotPicoTras = 0;
float  rotPicoFren = 0;
float  rotTDer = 0;
float  rotTIzq = 0;
float subeArriba = 310.0f;
float subePico = 310.0f;
float nivelBill = 0;
float rotPuerta = 0;

//********************Modo Dia/Modo Noche********************//
GLuint cubemapTexture;
float	luzX = 1.0f,
		luzY = 1.0f,
		luzZ = 1.0f,
		luz1X = 1.0f,
		luz1Y = 1.0f,
		luz1Z = 1.0f;
bool	dia = true;

int main()
{
	// Init GLFW
	glfwInit();
	
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "P", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);/**/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);/**/

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	
	//-------------------------------------------------------------------------------------------------------------------------------------------------------
	Model Mymodel((char*)"Models/proy/acabado2.obj");
	Model bill((char*)"Models/proy/finbill.obj");
	Model paredDer((char*)"Models/proy/paredDer.obj");
	Model entrada((char*)"Models/proy/entradaGlesia.obj");
	Model paredIzq((char*)"Models/proy/paredIzq.obj");
	Model tras((char*)"Models/proy/salida.obj");
	Model picotras((char*)"Models/proy/trianguloTras.obj");
	Model picofre((char*)"Models/proy/trianguloFrente.obj");
	Model techoDer((char*)"Models/proy/Tderecho.obj");
	Model techoIzq((char*)"Models/proy/Tizq.obj");
	Model arriba((char*)"Models/proy/arriba.obj");
	Model pico((char*)"Models/proy/pico.obj");
	Model puerta((char*)"Models/proy/puerta.obj");
	Model bosque((char*)"Models/proy/bosque.obj");

	// Build and compile our shader program

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		//ordenamiento por vertices
		30.0f,  7.0f, -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,//tapadera
		30.0f,  7.0f, -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,
		15.0f,  25.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,
		15.0f,  25.0f, -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,
		15.0f,  25.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		30.0f,  7.0f, -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,

		0.0f,  7.0f, -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,//tapadera
		0.0f,  7.0f, -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,
		15.0f,  25.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,
		15.0f,  25.0f, -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,
		15.0f,  25.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.0f,  7.0f, -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,

		0.0f,  0.0f,  -10.0f,    0.0f,  0.0f,  1.0f,    6.0f,  0.0f,//lado izquierdo
		0.0f,  0.0f, -40.0f,    0.0f,  0.0f,  1.0f,    6.0f,  6.0f,
		0.0f, 7.0f, -40.0f,    0.0f,  0.0f,  1.0f,    0.0f,  6.0f,
		0.0f, 7.0f, -40.0f,    0.0f,  0.0f,  1.0f,    0.0f,  6.0f,
		0.0f, 7.0f,  -10.0f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,
		0.0f,  0.0f,  -10.0f,    0.0f,  0.0f,  1.0f,    6.0f,  0.0f,//29

		0.0f,	0.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//frente cubo izquierdo
		13.6f,  0.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 
		13.6f,  7.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,
		13.6f,  7.0f,  -10.0f,     0.0f,  0.0f,  1.0f,  	6.0f,  6.0f,//este
		0.0f,   7.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,//y este sale el derecho abajo
		0.0f,   0.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//11

		17.0f,	6.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//frente cubo centro peque
		13.6f,  6.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,//izq arriba 
		13.6f,  7.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		13.6f,  7.0f,  -10.0f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,//este
		17.0f,   7.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,//y este sale el derecho abajo
		17.0f,   6.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//11

		30.0f,	0.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//frente cubo derecho
		17.0f,  0.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 
		17.0f,  7.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,
		17.0f,  7.0f,  -10.0f,     0.0f,  0.0f,  1.0f,  	6.0f,  6.0f,//este
		30.0f,   7.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,//y este sale el derecho abajo
		30.0f,   0.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//11

		30.0f,  0.0f,  -10.0f,    0.0f,  0.0f,  1.0f,    6.0f,  0.0f,//lado derecho *** aquí reduce en 10 unidades
		30.0f,  0.0f, -40.0f,    0.0f,  0.0f,  1.0f,    6.0f,  6.0f,
		30.0f, 7.0f, -40.0f,    0.0f,  0.0f,  1.0f,    0.0f,  6.0f,
		30.0f, 7.0f, -40.0f,    0.0f,  0.0f,  1.0f,    0.0f,  6.0f,
		30.0f, 7.0f,  -10.0f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,//**reduce en 10 unidades
		30.0f,  0.0f,  -10.0f,    0.0f,  0.0f,  1.0f,    6.0f,  0.0f,//reduce en 10 unidades profundidad

		0.0f, 0.0f,  -40.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//frente cubo
		30.0f, 0.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 
		30.0f,  7.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,//este
		30.0f,  7.0f,  -40.0f,     0.0f,  0.0f,  1.0f,  	6.0f,  6.0f,//y este sale el derecho abajo
		0.0f,  7.0f,  -40.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,
		0.0f, 0.0f,  -40.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//5

		0.0f, 0.0f, -10.0f,    0.0f, 0.0f,  1.0f,     0.0f,  6.0f,//base
		30.0f, 0.0f, -10.0f,     0.0f, 0.0f,  1.0f,     6.0f,  6.0f,
		30.0f, 0.0f,  -40.0f,     0.0f, 0.0f,  1.0f,     6.0f,  0.0f,
		30.0f, 0.0f,  -40.0f,     0.0f, 0.0f, 1.0f,     6.0f,  0.0f,
		0.0f, 0.0f,  -40.0f,    0.0f, 0.0f,  1.0f,     0.0f,  0.0f,
		0.0f, 0.0f, -10.0f,    0.0f, 0.0f,  1.0f,     0.0f,  6.0f,//35

		//***piso
		0.0f, 7.0f, -10.0f,    0.0f, 0.0f,  1.0f,     0.0f,  8.0f,//base
		30.0f, 7.0f, -10.0f,     0.0f, 0.0f,  1.0f,     8.0f,  8.0f,
		30.0f, 7.0f,  -40.0f,     0.0f, 0.0f,  1.0f,     8.0f,  0.0f,
		30.0f, 7.0f,  -40.0f,     0.0f, 0.0f,  1.0f,     8.0f,  0.0f,
		0.0f, 7.0f,  -40.0f,    0.0f, 0.0f,  1.0f,     0.0f,  0.0f,
		0.0f, 7.0f, -10.0f,    0.0f, 0.0f,  1.0f,     0.0f,  8.0f,//35

//*************parte arriba atras izq
		0.0f, 7.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 	
		15.0f,  25.0f, -40.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,//y este sale el derecho abajo
		15.0f, 7.0f, -40.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//17		
		0.0f, 25.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 
		0.0f, 7.0f,   -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,
		0.0f, 7.0f,   -40.0f,     0.0f,  0.0f,  1.0f,  	6.0f,  6.0f,//este
//***********fin parte arriba atras izq********************

//*************parte arriba atras der**********************
		30.0f, 7.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 		
		15.0f,  25.0f, -40.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,//y este sale el derecho abajo
		15.0f, 7.0f, -40.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//17
		30.0f, 7.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 
		30.0f, 25.0f,   -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,
		30.0f, 25.0f,   -40.0f,     0.0f,  0.0f,  1.0f,  	6.0f,  6.0f,//este
//***********fin parte arriba atras der********************

//parte arriba delante izq*********************************
			0.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 0.0f,//izq arriba 			
			15.0f, 25.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 6.0f,//y este sale el derecho abajo
			15.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//17
			0.0f, 25.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 0.0f,//izq arriba 
			0.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 6.0f,
			0.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 6.0f,//este
//***********fin parte arriba delante izq********************

//***********parte arriba delatne der********************
			30.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 0.0f,//izq arriba 			
			15.0f, 25.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 6.0f,//y este sale el derecho abajo
			15.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//17
			30.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 0.0f,//izq arriba 
			30.0f, 25.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 6.0f,
			30.0f, 25.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 6.0f,//este
//***********fin parte arriba delatne der********************	
	};

	//----------------------------------agregar skybox--------------------------------
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;//posiciones de memoria
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//**********************************skybox/**/*************************
	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	// Load textures
	if (dia)
	{
		vector<const GLchar*> faces;
		faces.push_back("SkyBox/rightN.tga");
		faces.push_back("SkyBox/leftN.tga");
		faces.push_back("SkyBox/topN.tga");
		faces.push_back("SkyBox/bottomN.tga");
		faces.push_back("SkyBox/backN.tga");
		faces.push_back("SkyBox/frontN.tga");
		cubemapTexture = TextureLoading::LoadCubemap(faces);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else
	{
		vector<const GLchar*> faces;
		faces.push_back("SkyBox/right.tga");
		faces.push_back("SkyBox/left.tga");
		faces.push_back("SkyBox/top.tga");
		faces.push_back("SkyBox/bottom.tga");
		faces.push_back("SkyBox/back.tga");
		faces.push_back("SkyBox/front.tga");
		cubemapTexture = TextureLoading::LoadCubemap(faces);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	// Load textures /////////////parte para crear texturas
	GLuint texture1, texture2;//declaro las texturas
	glGenTextures(1, &texture1);//las genero en memoria una difusa
	//glGenTextures(1, &texture2);//una especular ******* revisar shader

	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);//el flip
	unsigned char *image;//caract de repeticion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//parte de filtrado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	// Diffuse map************************************************************************
	image = stbi_load("images/casafrente.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)//la parte difusa, liberar memoria, etc etc etc
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	//enlazamos las texturas

	// Set texture units
	lightingShader.Use();//cargamos en el shader las texturas, se pueden hasta 20
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	//glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();
		casa();
		porton();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Load Model

		//***********************trabajando aqui***********************************************

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

		//*******************FIN DE MIS INTENTOS
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), luz1X, luz1Y, luz1Z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), luzX, luzY, luzZ);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);//brillo que se manda al shader

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader ****** lo pasamos al shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		glActiveTexture(GL_TEXTURE0);// textura con la que trabajo texture0
		glBindTexture(GL_TEXTURE_2D, texture1);

		//// Bind specular map
		//glActiveTexture(GL_TEXTURE1);//textura con la que travajo texture1
		//glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);

		glm::mat4 model(1);

		////*********************************************la casa***************************************
		/*for (GLuint i = 0; i < 10; i++)
		{*/
		model = glm::mat4(1);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 84);

		glBindVertexArray(0);

		//}

		//////Carga de modelo  pescado************************************************************************************
		view = camera.GetViewMatrix();
		//model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(15.0f, 6.0f, -29.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel.Draw(lightingShader);

//glBindVertexArray(0);

		//**********************************--------------------ESTO SERÁ LA CASA ARMABLE---------------------------------******************************
		glm::mat4 tmp = glm::mat4(1.0f);
		//model = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		tmp = model = glm::translate(model, glm::vec3(-4000.0f, -900.0f, 3000.0f));
		//model = glm::translate(model, glm::vec3(-4000.0f, -900.0f, 3000.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rotparedDer), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		paredDer.Draw(lightingShader);//pared Derecha

		//**********************
		//model = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(400.0f, -400.0f, -800.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rotentrada), glm::vec3(0.0f, 0.0f, 1.0f));// moviemineto de la rodilla izq
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		entrada.Draw(lightingShader);//frente

		//**********************
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-30.0f, -600.0f, -950.f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-rotparIzq), glm::vec3(1.0f, 0.0f, 0.0f));// moviemineto de la rodilla izq
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		paredIzq.Draw(lightingShader);//izq

		//**********************
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(200.0f, 250.0f, -790.f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-rotTras), glm::vec3(0.0f, 0.0f, 1.0f));// moviemineto de la rodilla izq
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tras.Draw(lightingShader);//tras

		//**********************
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(200.0f, 250.0f, -790.f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-rotPicoTras), glm::vec3(0.0f, 0.0f, 1.0f));// moviemineto de la rodilla izq
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		picotras.Draw(lightingShader);//ppicotras

		//**********************
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-360.0f, 440.0f, -810.f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rotPicoFren), glm::vec3(0.0f, 0.0f, 1.0f));// moviemineto de la rodilla izq
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		picofre.Draw(lightingShader);//ppicofre

		//**********************
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-100.0f, 810.0f, -10.f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rotTDer), glm::vec3(1.0f, 0.0f, 0.0f));// moviemineto de la rodilla izq
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		techoDer.Draw(lightingShader);//techoDerecho

		//**********************
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-100.0f, 310.0f,-590.f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-rotTIzq), glm::vec3(1.0f, 0.0f, 0.0f));// moviemineto de la rodilla izq
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		techoIzq.Draw(lightingShader);//techoIzquierdo

		//**********************
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-100.0f, subeArriba, -750.f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::rotate(model, glm::radians(subeArriba), glm::vec3(1.0f, 0.0f, 0.0f));// moviemineto de la rodilla izq
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		arriba.Draw(lightingShader);//arriba

		//**********************
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-100.0f, subePico, -750.f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::rotate(model, glm::radians(subeArriba), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pico.Draw(lightingShader);//pico
		//glBindVertexArray(0);

		//**********************
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-100.0f, 250.0f, -750.f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::rotate(model, glm::radians(subeArriba), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bosque.Draw(lightingShader);//pico

		//*********************************-------------------------------------FIN CASA ARMABLE----------------------*****************************
		model = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(4000.0f, 1500.0f, -3100.f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rotBill), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bill.Draw(lightingShader);//bill

		model = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(4195.0f, 895.0f, -2012.f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puerta.Draw(lightingShader);//puerta

		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), 4195.0f, 895.0f, -2012.f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 4195.0f, 895.0f, -2012.f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 4195.0f, 895.0f, -2012.f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		//************************************************* BILL********************
		//glm::mat4 tmp = glm::mat4(1.0f);
		////Personaje
		////model = glm::mat4(1.0f);
		//view = camera.GetViewMatrix();
		////glm::mat4 model(1);
		//tmp = model = glm::translate(model, glm::vec3(-4000.0f, -900.0f, 3000.0f));
		//model = glm::translate(model, glm::vec3(15.0f, 6.0f, -29.0f));
		//model = glm::rotate(model, glm::radians(rotBill), glm::vec3(0.0f, 1.0f, 0.0));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//bill.Draw(lightingShader);

		//glBindVertexArray(0);

		//*******************************************fin bill*************

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		//para saber donde están mis luces
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

//***************************************************AGREGO skybox*****************************
		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void animacion()//BILL mueve la ciudad
{
	if (bill)
	{
		if (recorrido1)
		{
			rotBill -= 5.0f;
			if (rotBill > 90)
			{
				//recorrido1 = false;
				recorrido1 = true;
			}
		}
	}
}

//*****casa**********
void casa()
{
	if (paredDer)
	{
		if (recorrido2)
		{
			rotparedDer += 1.0f;
			if (rotparedDer >= 90)
			{
				recorrido2 = false;
				recorrido3 = true;
			}
		}

		if (recorrido3)
		{
			rotentrada += 1.0f;
			if (rotentrada >= 90)
			{
				recorrido3 = false;
				//recorrido3 = true;
			}
		}

		if (recorrido4)
		{
			rotparIzq += 1.0f;
			if (rotparIzq >= 90)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
		}

		//tras
		if (recorrido5)
		{
			rotTras += 1.0f;
			if (rotTras >= 90)
			{
				recorrido5 = false;
				//recorrido3 = true;
			}
		}

		//pico tras
		if (recorrido6)
		{
			rotPicoTras += 1.0f;
			if (rotPicoTras >= 90)
			{
				recorrido6 = false;
			}
		}

		//pico enfrente
		if (recorrido7)
		{
			rotPicoFren += 1.0f;
			if (rotPicoFren >= 90)
			{
				recorrido7 = false;
			}
		}

		//Techo Derecho
		if (recorrido8)
		{
			rotTDer += 1.0f;
			if (rotTDer >= 124)
			{
				recorrido8 = false;
			}
		}

		//Techo Izq
		if (recorrido9)
		{
			rotTIzq += 1.0f;
			if (rotTIzq >= 126)
			{
				recorrido9 = false;
			}
		}

		//palo
		if (recorrido10)
		{
			subeArriba += 25.0f;
			if (subeArriba >= 3500)
			{
				recorrido10 = false;
			}
		}

		//pico
		if (recorrido11)
		{
			subePico += 25.0f;
			if (subePico >= 3360)
			{
				recorrido11 = false;
			}
		}
	}
}

void porton() {
	if (puerta) {
		if (rotPuerta < 336)
			for (int i = 0; i <= 335; i++)
				rotPuerta += 5.0f;
	}
	else
		rotPuerta = 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	if (keys[GLFW_KEY_1])
	{
		paredDer = true;
		/*range += 0.1;
		printf("El rango es %f\n", range);*/
	}

	if (keys[GLFW_KEY_2])
	{
		bill = true;
		/*range -= 0.1;
		printf("El rango es %f\n", range);*/
	}

	if (keys[GLFW_KEY_3])
	{
		//puerta
		puerta = true;
	}

	if (keys[GLFW_KEY_4])
	{
		puerta = false;
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime*3);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime*3);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime*3);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime*3);
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(4195.0f, 895.0f, -2012.f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	//Modo noche
	if (keys[GLFW_KEY_8])
	{
		luzX = 0.0f;
		luzY = 0.0f;
		luzZ = 0.0f;
		luz1X = 0.5f;
		luz1Y = 0.5f;
		if (dia = false)
		{
			vector<const GLchar*> faces;
			faces.push_back("SkyBox/rightN.tga");
			faces.push_back("SkyBox/leftN.tga");
			faces.push_back("SkyBox/topN.tga");
			faces.push_back("SkyBox/bottomN.tga");
			faces.push_back("SkyBox/backN.tga");
			faces.push_back("SkyBox/frontN.tga");
			cubemapTexture = TextureLoading::LoadCubemap(faces);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	//Modo día
	if (keys[GLFW_KEY_9])
	{
		luzX = 1.0f;
		luzY = 1.0f;
		luzZ = 1.0f;
		luz1X = 1.0f;
		luz1Y = 1.0f;
		luz1Z = 1.0f;
		if (dia = true)
		{
			vector<const GLchar*> faces;
			faces.push_back("SkyBox/right.tga");
			faces.push_back("SkyBox/left.tga");
			faces.push_back("SkyBox/top.tga");
			faces.push_back("SkyBox/bottom.tga");
			faces.push_back("SkyBox/back.tga");
			faces.push_back("SkyBox/front.tga");
			cubemapTexture = TextureLoading::LoadCubemap(faces);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}