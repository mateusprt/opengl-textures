#include "Sprite.h"

Sprite::Sprite(GLuint texturaID, vec3 posicao, vec3 escala, Shader* shader, float angulo, bool espelhado)
{
    this->texID = texturaID;
    this->pos = posicao;
    this->ang = angulo;
    this->scale = escala;
    this->isMirrored = espelhado;
    definirShader(shader);

    GLfloat vertices[] = {
        // Triângulo 0
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // v0
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // v1
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // v2
        // Triângulo 1
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // v1
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // v3
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f   // v2
    };

    GLuint VBO;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Atributo 0 - posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributo 1 - cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Atributo 2 - coordenadas de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Sprite::~Sprite()
{
    glDeleteVertexArrays(1, &VAO);
}

void Sprite::definirShader(Shader* shader) 
{
    this->shader = shader;
}

void Sprite::desenhar()
{
    atualizar();

    glBindTexture(GL_TEXTURE_2D, texID);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Sprite::atualizar() // Alterado o nome da função para português
{
    mat4 modelo = glm::mat4(1.0f);
    modelo = translate(modelo, pos);
    if (isMirrored)
        modelo = rotate(modelo, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
    modelo = rotate(modelo, radians(ang), vec3(0.0f, 0.0f, 1.0f));
    modelo = glm::scale(modelo, scale);
    shader->setMat4("model", glm::value_ptr(modelo));
}
