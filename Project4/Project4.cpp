#include <stdio.h>
#include <string>
#include <cmath>

#include <GL/glew.h>      
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

#include "GameClock.h"
#include "GameWindow.h"

GLint WIDTH = 800, HEIGHT = 600;
std::string title = "OpenGL Window";
float FPS = 60.0f;

GameWindow gameWindow(WIDTH, HEIGHT, title, FPS);
GameClock gameClk(FPS);

GLuint VAO, VBO, shaderId;
GLuint uniformXMove, uniformYMove;
bool directionX = true; 
bool directionY = true;
float triOffsetX = 0.0f;
float triOffsetY = 0.0f;
float triMaxOffsetX = 0.90f;
float triMaxOffsetY = 0.90f;
float speedX = 0.5f;
float speedY = 0.335f;

static const char* vShader = "                              \n\
#version 330                                                \n\
                                                            \n\
layout (location = 0) in vec3 pos;                          \n\
uniform float xMove;                                        \n\
uniform float yMove;                                        \n\
                                                            \n\
void main() {                                               \n\
  gl_Position = vec4(pos.x + xMove, pos.y + yMove, pos.z, 1.0);     \n\
}                                                           \n\
";

static const char* fShader = "                              \n\
#version 330                                                \n\
                                                            \n\
out vec4 colour;                                            \n\
void main() {                                               \n\
  colour = vec4(1.0, 0.0, 0.0, 1.0);                        \n\
}                                                           \n\
";

GLfloat triangle1[] = {
    -0.1f, -0.1f, 0.0f,
     0.1f, -0.1f, 0.0f,
     0.0f,  0.1f, 0.0f,
};

void CreateTriangle(GLfloat vertices[], GLsizei vertexCount) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint &shaderId, const char* code, GLenum type) {
    GLuint subShaderId = glCreateShader(type);

    const GLchar* shaderCode[1];
    shaderCode[0] = code;

    GLint codeLength[1];
    codeLength[0] = strlen(code);

    glShaderSource(subShaderId, 1, shaderCode, codeLength);
    glCompileShader(subShaderId);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    glGetShaderiv(subShaderId, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(subShaderId, sizeof(errorLog), NULL, errorLog);
        printf("Помилка компіляції шейдера '%d': '%s'\n", type, errorLog);
        glDeleteShader(subShaderId);
        return;
    }

    glAttachShader(shaderId, subShaderId);
    glDeleteShader(subShaderId);
}

void CompileShaders(GLuint &shaderId, const char* vert, const char* frag) {
    shaderId = glCreateProgram();

    if (!shaderId) {
        printf("Не вдалося створити шейдерну програму!\n");
        return;
    }

    AddShader(shaderId, vert, GL_VERTEX_SHADER);
    AddShader(shaderId, frag, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        printf("Помилка лінкування шейдерів: '%s'\n", errorLog);
        glDeleteProgram(shaderId);
        return;
    }

    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        printf("Помилка валідації шейдера: '%s'\n", errorLog);
        glDeleteProgram(shaderId);
        return;
    }

    uniformXMove = glGetUniformLocation(shaderId, "xMove");
    uniformYMove = glGetUniformLocation(shaderId, "yMove");
}

int main() {
    gameWindow.init(NULL, NULL);

    CompileShaders(shaderId, vShader, fShader);
    CreateTriangle(triangle1, 9);

    while (!gameWindow.shouldClose()) {
        if (gameClk.update()) continue;

        glfwPollEvents();

        // Proper movement with deltaTime
        if (directionX) triOffsetX += speedX * gameClk.getDeltaTime();
        else triOffsetX -= speedX * gameClk.getDeltaTime();

        if (directionY) triOffsetY += speedY * gameClk.getDeltaTime();
        else triOffsetY -= speedY * gameClk.getDeltaTime();

        if (abs(triOffsetX) >= triMaxOffsetX) 
            directionX = !directionX;
        if (abs(triOffsetY) >= triMaxOffsetY)
            directionY = !directionY;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderId);
        glBindVertexArray(VAO);
        
        glUniform1f(uniformXMove, triOffsetX);
        glUniform1f(uniformYMove, triOffsetY);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        gameWindow.swapBuffers();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderId);
    gameWindow.terminate();

    return 0;
}
