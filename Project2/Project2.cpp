#include <stdio.h>
#include <string.h>

#include <GL/glew.h>      
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shaderId;

// Вершинний шейдер: перетворює позиції вершин у кліп-простір
// Примітка: 'in' означає вхідний параметр у шейдері, а не змінну всередині структури
static const char* vShader = "                              \n\
#version 330                                                \n\
                                                            \n\
layout (location = 0) in vec3 pos;                          \n\
void main() {                                               \n\
  gl_Position = vec4(1.0*pos.x, 1.0*pos.y, pos.z, 1.0);     \n\
}                                                           \n\
";

// Фрагментний шейдер: задає вихідний колір кожного пікселя
static const char* fShader = "                              \n\
#version 330                                                \n\
                                                            \n\
out vec4 colour;                                            \n\
void main() {                                               \n\
  colour = vec4(1.0, 0.0, 0.0, 1.0);                        \n\
}                                                           \n\
";

GLfloat triangle1[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};

void CreateTriangle(GLfloat vertices[], GLsizei vertexCount) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Передача повного розміру масиву вершин (кількість float * розмір float)
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    // перший 0: номер атрибута у шейдері (відповідає layout у vShader)
    // 3: кількість компонент на вершину (x, y, z)
    // GL_FLOAT: тип даних кожного компонента
    // GL_FALSE: не нормалізувати значення
    // другий 0: крок між вершинами (0 означає щільне зберігання)
    // третій 0: зміщення від початку буфера (немає зміщення)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint shaderId, const char* code, GLenum type) {
    GLuint subShaderId = glCreateShader(type);

    const GLchar* shaderCode[1];
    shaderCode[0] = code;

    GLint codeLength[1];
    codeLength[0] = strlen(code);

    glShaderSource(subShaderId, 1, shaderCode, codeLength);
    glCompileShader(subShaderId);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    // Перевірка статусу компіляції шейдера (не програми)
    // shaderId — це окремий шейдер, а не фінальна програма
    glGetShaderiv(subShaderId, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(subShaderId, sizeof(errorLog), NULL, errorLog);
        printf("Помилка компіляції шейдера '%d': '%s'\n", type, errorLog);
        glDeleteShader(subShaderId);
        return;
    }

    glAttachShader(shaderId, subShaderId);
    // Видаляємо шейдер після додавання до програми (він більше не потрібен)
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

    // Лінкуємо вершиний і фрагментний шейдери в єдину програму
    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        printf("Помилка лінкування шейдерів: '%s'\n", errorLog);
        glDeleteProgram(shaderId);
        return;
    }

    // Перевіряємо, чи може програма працювати в поточному стані OpenGL
    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        printf("Помилка валідації шейдера: '%s'\n", errorLog);
        glDeleteProgram(shaderId);
        return;
    }
}

int main() {
    if (!glfwInit()) {
        printf("Не вдалося ініціалізувати GLFW\n");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
    if (!mainWindow) {
        printf("Не вдалося створити вікно GLFW\n");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("Не вдалося ініціалізувати GLEW\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    CompileShaders(shaderId, vShader, fShader);
    CreateTriangle(triangle1, 9);  // 9 float = 3 вершини * 3 координати

    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderId);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    // Очищення ресурсів OpenGL, щоб уникнути витоку пам'яті
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderId);
    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 0;
}

