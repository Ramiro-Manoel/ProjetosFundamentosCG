#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STB_IMAGE
#include <stb_image.h>

using namespace glm;


//Estrutura das sprites
struct Sprite
{
	GLuint VAO;
	GLuint texID;
	vec3 position;
	vec3 dimensions;
	float angle;
    float velocity;
	float size;

	// Para controle da animação
	int nAnimations, nFrames;
	int iAnimation, iFrame;
	vec2 d;
	float FPS;
	float lastTime;
	vec2 offsetTex;

	// Função de inicialização
	void setupSprite(int texID, vec3 position, vec3 dimensions, int nFrames, int nAnimations, float velocity, float size);
	vec2 getPMin();
	vec2 getPMax();
};


// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar *vertexShaderSource = "#version 400\n"
								   "layout (location = 0) in vec3 position;\n"
								   "layout (location = 1) in vec2 texc;\n"
								   "uniform mat4 projection;\n"
								   "uniform mat4 model;\n"
								   "out vec2 texCoord;\n"
								   "void main()\n"
								   "{\n"
								   //...pode ter mais linhas de código aqui!
								   "gl_Position = projection * model * vec4(position.x, position.y, position.z, 1.0);\n"
								   "texCoord = vec2(texc.s, 1.0 - texc.t);\n"
								   "}\0";

// Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar *fragmentShaderSource = "#version 400\n"
									 "in vec2 texCoord;\n"
									 "uniform sampler2D texBuffer;\n"
									 "uniform vec2 offsetTex;\n"
									 "out vec4 color;\n"
									 "void main()\n"
									 "{\n"
									 "color = texture(texBuffer, texCoord + offsetTex);\n"
									 "}\n\0";

const GLuint WIDTH  = 800;
const GLuint HEIGHT = 600;
const int HEIGHT_RANGE = 800;

bool keys[1024] = {false};

int setupShader();
int loadTexture(string filePath, int &imgWidth, int &imgHeight);
void drawSprite(Sprite spr, GLuint shaderID);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
bool checkCollision(Sprite &one, Sprite &two);

int main()
{
    //inicia o glfw
    glfwInit();

    //Criação da janela
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Jogo GA - Ramiro", nullptr, nullptr);
	glfwMakeContextCurrent(window);

    // Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

    // GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

    // Obtendo as informações de versão
	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	/* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
    
    // Definindo viewport com as mesmas dimensões da janela
    int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

    //Iniciando shader
    GLuint shaderID = setupShader();

    //Criando sprite background
    Sprite background;
    int imgWidth, imgHeight;
	int texID = loadTexture("Textures/Background/background/orig.png", imgWidth, imgHeight);
	background.setupSprite(texID, vec3(400.0, 300.0, 0.0), vec3(imgWidth * 1.5, imgHeight * 1.5, 1.0), 1, 1, 0.0, 10);

	//Criando sprite bombScenario
    Sprite bombScenario1;
	texID = loadTexture("Textures/Background/background/4.png", imgWidth, imgHeight);
	bombScenario1.setupSprite(texID, vec3(400.0, 300.0, 0.0), vec3(imgWidth * 1.5, imgHeight * 1.5, 1.0), 1, 1, 2, 10);

	Sprite bombScenario2;
	texID = loadTexture("Textures/Background/background/4.png", imgWidth, imgHeight);
	bombScenario2.setupSprite(texID, vec3(1200.0, 300.0, 0.0), vec3(imgWidth * 1.5, imgHeight * 1.5, 1.0), 1, 1, 2, 10);
	
	//Criando sprite character
	Sprite character;
	texID = loadTexture("Textures/Character/Woodcutter/Swim.png", imgWidth, imgHeight);
	character.setupSprite(texID, vec3(50.0, 200.0, 0.0), vec3(imgWidth / 6.0 * 2.0, imgHeight * 2.0, 1.0), 6, 1, 1.5, 10);

	//Criando sprite enemySwordFish
	Sprite enemySwordFish;
	texID = loadTexture("Textures/Enemies/3/Walk.png", imgWidth, imgHeight);
	enemySwordFish.setupSprite(texID, vec3(-50.0, (rand() % HEIGHT_RANGE), 0.0), vec3(imgWidth / 4.0 * 2.0, imgHeight * 2.0, 1.0), 4, 1, 2, 15);
    glUseProgram(shaderID);

	//Criando sprite enemySquid
	Sprite enemySquid;
	texID = loadTexture("Textures/Enemies/6/Walk.png", imgWidth, imgHeight);
	enemySquid.setupSprite(texID, vec3(-50.0, (rand() % HEIGHT_RANGE), 0.0), vec3(imgWidth / 6.0 * 2.0, imgHeight * 2.0, 1.0), 6, 1, 2, 15);
    glUseProgram(shaderID);

    // Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers
	glUniform1i(glGetUniformLocation(shaderID, "texBuffer"), 0);

    //Matriz de projeção ortográfica
    mat4 projection = ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));
    
    // Ativando o primeiro buffer de textura da OpenGL
	glActiveTexture(GL_TEXTURE0);
    
    //Transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Habilitar o teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

    //game loop
    while (!glfwWindowShouldClose(window))
	{
        //verifica alterações no jogo - chama funçoes de callback
        glfwPollEvents();

        // Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniform2f(glGetUniformLocation(shaderID, "offsetTex"), background.offsetTex.s, background.offsetTex.t);
		
		//adiciona o background na tela
		drawSprite(background, shaderID);

		//controla movimentação do character
		if (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A])
			character.position.x -= character.velocity;
		if (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D])
			character.position.x += character.velocity;
		if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_W])
			character.position.y += character.velocity;
		if (keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S])
			character.position.y -= character.velocity;

		//Animação de natação do character
		float now = glfwGetTime();
		float dt = now - character.lastTime;
		if (dt >= 1.0 / character.FPS)
		{
			character.iFrame = (character.iFrame + 1) % character.nFrames; // incrementando ciclicamente o indice do Frame
			character.lastTime = now;
		}	
		character.offsetTex.s = character.iFrame * character.d.s;
		character.offsetTex.t = 0.0;
		glUniform2f(glGetUniformLocation(shaderID, "offsetTex"), character.offsetTex.s, character.offsetTex.t);
		
		//adiciona o character na tela
		drawSprite(character, shaderID);

		//adiciona inimigos
		//swordFish
		enemySwordFish.position.x += enemySwordFish.velocity;
		glUniform2f(glGetUniformLocation(shaderID, "offsetTex"), enemySwordFish.offsetTex.s, enemySwordFish.offsetTex.t);
		drawSprite(enemySwordFish, shaderID);
		if(enemySwordFish.position.x >= 850)
		{
			enemySwordFish.position.x = -50;
			enemySwordFish.position.y = (rand() % HEIGHT_RANGE);
			if(enemySwordFish.velocity < 8)
			enemySwordFish.velocity += 0.25;
		}

		//Squid	
		enemySquid.position.x += enemySquid.velocity;
		glUniform2f(glGetUniformLocation(shaderID, "offsetTex"), character.offsetTex.s, character.offsetTex.t);
		drawSprite(enemySquid, shaderID);
		if(enemySquid.position.x >= 850)
		{
			enemySquid.position.x = -50;
			enemySquid.position.y = (rand() % HEIGHT_RANGE);
			if(enemySquid.size < 50)
			{
			enemySquid.size *= 1.5;
			enemySquid.dimensions *= 1.25;
			}
		}

		//adiciona as bombas cenário na tela
		bombScenario1.position.x -= bombScenario1.velocity;
		glUniform2f(glGetUniformLocation(shaderID, "offsetTex"), bombScenario1.offsetTex.s, bombScenario1.offsetTex.t);
		drawSprite(bombScenario1, shaderID);
		
		bombScenario2.position.x -= bombScenario2.velocity;
		glUniform2f(glGetUniformLocation(shaderID, "offsetTex"), bombScenario2.offsetTex.s, bombScenario2.offsetTex.t);
		drawSprite(bombScenario2, shaderID);

		//ciclo entre as duas sprite de bombas, para sempre ter bomba na tela
		if(bombScenario1.position.x <= -400)
			bombScenario1.position.x = 1200;

		if(bombScenario2.position.x <= -400)
			bombScenario2.position.x = 1200;

		//verifica colisões
		if(checkCollision(character, enemySwordFish) || checkCollision(character, enemySquid))
		break;

        glfwSwapBuffers(window);
    }

    //encerra o glfw
    glfwTerminate();
	return 0;
}

void Sprite::setupSprite(int texID, vec3 position, vec3 dimensions, int nFrames, int nAnimations, float velocity, float size)
{
	this->texID = texID;
	this->dimensions = dimensions;
	this->position = position;
	this->nAnimations = nAnimations;
	this->nFrames = nFrames;
    this->velocity = velocity;
	this->size = size;
	iAnimation = 0;
	iFrame = 0;

	d.s = 1.0 / (float)nFrames;
	d.t = 1.0 / (float)nAnimations;
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {
		// x   y     z    s     		t
		// T0
		-0.5, -0.5, 0.0, 0.0, 0.0, // V0
		-0.5, 0.5, 0.0, 0.0, d.t,  // V1
		0.5, -0.5, 0.0, d.s, 0.0,  // V2
		0.5, 0.5, 0.0, d.s, d.t	   // V3

	};
	
	GLuint VBO, VAO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);
	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	//  Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	//  Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
	//  Tipo do dado
	//  Se está normalizado (entre zero e um)
	//  Tamanho em bytes
	//  Deslocamento a partir do byte zero

	// Atributo 0 - Posição - x, y, z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Atributo 1 - Coordenadas de textura - s, t
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);
	this->VAO = VAO;
	FPS = 6.0;
	lastTime = 0.0;
	this->offsetTex = vec2(0.0, 0.0);
}

vec2 Sprite::getPMin()
{
	return vec2 (position.x - size / 2, position.y - size / 2);
}

vec2 Sprite::getPMax()
{
	return vec2 (position.x + size / 2, position.y + size / 2);
}

int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int loadTexture(string filePath, int &imgWidth, int &imgHeight)
{
	GLuint texID;

	// Gera o identificador da textura na memória
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Configurando o wrapping da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Configurando o filtering de minificação e magnificação da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Carregamento da imagem da textura
	int nrChannels;
	unsigned char *data = stbi_load(filePath.c_str(), &imgWidth, &imgHeight, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) // jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << filePath << std::endl;
	}

	return texID;
}

void drawSprite(Sprite spr, GLuint shaderID)
{
	glBindVertexArray(spr.VAO); // Conectando ao buffer de geometria

	glBindTexture(GL_TEXTURE_2D, spr.texID); // conectando o buffer de textura

	// Matriz de modelo - Tranformações na geometria, nos objetos
	mat4 model = mat4(1); // matriz identidade
	// Translação
	model = translate(model, spr.position);
	// Rotação
	model = rotate(model, radians(spr.angle), vec3(0.0, 0.0, 1.0));
	// Escala
	model = scale(model, spr.dimensions);
	// Enviar para o shader
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

	// Chamada de desenho - drawcall
	// Poligono Preenchido - GL_TRIANGLES
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0); // Desconectando o buffer de geometria
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
	{
		keys[key] = true;
	}
	if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}

bool checkCollision(Sprite &one, Sprite &two)
{
// collision x-axis?
bool collisionX = one.getPMax().x >=
two.getPMin().x &&
two.getPMax().x >= one.getPMin().x;
// collision y-axis?
bool collisionY = one.getPMax().y >=
two.getPMin().y &&
two.getPMax().y >= one.getPMin().y;
// collision only if on both axes
return collisionX && collisionY;
}