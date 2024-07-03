#include <iostream>
#include <string>
#include <cassert> 
#include <stb_image.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

#include "Shader.h"
#include "Sprite.h"

// Protótipos
GLuint carregarTextura(const string& caminhoTextura); 

// Dimensões da janela
const GLuint LARGURA = 800, ALTURA = 600; 

Sprite* jogador;
Sprite* cenario;

int main()
{
    // Inicializando a GLFW
    if (!glfwInit())
    {
        cerr << "Falha ao inicializar a GLFW" << endl;
        return -1;
    }

    // Criando a janela GLFW
    GLFWwindow* janela = glfwCreateWindow(LARGURA, ALTURA, "Sprites", nullptr, nullptr);
    if (!janela) 
    {
        cerr << "Falha ao criar a janela GLFW" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(janela);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cerr << "Falha ao inicializar a GLAD" << endl;
        return -1;
    }

    // Habilitando o modo de transparência (blend - mistura de cores)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Compilando e construindo o programa de shader padrão
    Shader shader("../shaders/tex_vert.glsl", "../shaders/tex_frag.glsl");

    // Criando as sprites
    jogador = new Sprite(carregarTextura("../player.png"), vec3(400.0f, 150.0f, 0.0f), vec3(128.0f, 128.0f, 1.0f), &shader, 180.0f);
    cenario = new Sprite(carregarTextura("../background.png"), vec3(400.0f, 300.0f, 0.0f), vec3(960.0f, 540.0f, 1.0f), &shader, 180.0f);

    // Ativando o buffer de textura 0 da opengl
    glActiveTexture(GL_TEXTURE0);

    shader.Use();

    // Matriz de projeção paralela ortográfica
    glm::mat4 projecao = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    shader.setMat4("projection", glm::value_ptr(projecao));

    // Matriz de transformação
    shader.setInt("texBuffer", 0);

    // Game loop
    while (!glfwWindowShouldClose(janela))
    {
        // Checa se houveram eventos de input (key pressed, mouse moved etc.)
        glfwPollEvents();

        // Dimensões da viewport com as mesmas dimensões da janela da aplicação
        int largura, altura;
        glfwGetFramebufferSize(janela, &largura, &altura);
        glViewport(0, 0, largura, altura);

        // Limpa o buffer de cor
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);

        // desenha na tela
        cenario->desenhar();
        jogador->desenhar();

        glfwSwapBuffers(janela);
    }

    // Finaliza a execução da GLFW
    glfwTerminate();
    return 0;
}

GLuint carregarTextura(const string& caminhoTextura)
{
    GLuint texID;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int largura, altura, canais;
    unsigned char* dados = stbi_load(caminhoTextura.c_str(), &largura, &altura, &canais, 0);

    if (dados)
    {
        if (canais == 3) // jpg, bmp
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, largura, altura, 0, GL_RGB, GL_UNSIGNED_BYTE, dados);
        }
        else // png
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, dados);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cerr << "Falha ao carregar a textura" << endl;
    }

    stbi_image_free(dados);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}
