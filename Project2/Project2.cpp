#include <stdio.h>
#include <string.h>
#include <GL/glew.h>      
#include <GLFW/glfw3.h>

// Vertex shader: Transforms vertex positions to clip space
// Note: 'in' denotes input to the shader, not a variable inside a struct
static const char* vShader = "                              \n\
#version 330                                                \n\
                                                            \n\
layout (location = 0) in vec3 pos;                          \n\
void main() {                                               \n\
  gl_Position = vec4(1.0*pos.x, 1.0*pos.y, pos.z, 1.0);     \n\
}                                                           \n\
";

// Fragment shader: Sets the output color of each pixel
static const char* fShader = "                              \n\
#version 330                                                \n\
                                                            \n\
out vec4 colour;                                            \n\
void main() {                                               \n\
  colour = vec4(1.0, 0.0, 0.0, 1.0);                        \n\
}                                                           \n\
";

struct ShaderData {
    const char* vert;
    const char* frag;
    GLuint shader;
};

struct DrawData {
    GLuint VAO, VBO, shader;
};

const GLint WIDTH = 800, HEIGHT = 600;

GLfloat triangle1[] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  // Adjusted for visibility (was 0.1f)
     0.0f,  1.0f, 0.0f,
};

void CreateTriangle(DrawData& d, GLfloat vertices[], GLsizei vertexCount) {
    glGenVertexArrays(1, &d.VAO);
    glBindVertexArray(d.VAO);

    glGenBuffers(1, &d.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, d.VBO);
    // Pass the total size of the vertex data (number of floats * size of float)
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    // first 0: attribute location in shader (matches layout in vShader)
    // 3: number of components per vertex (x, y, z)
    // GL_FLOAT: data type of each component
    // GL_FALSE: no normalization of values
    // second 0: stride (0 means tightly packed data)
    // third 0: offset from start of buffer (no offset here)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint programId, const char* code, GLenum type) {
    GLuint shaderId = glCreateShader(type);

    const GLchar* shaderCode[1];
    shaderCode[0] = code;

    GLint codeLength[1];
    codeLength[0] = strlen(code);

    glShaderSource(shaderId, 1, shaderCode, codeLength);
    glCompileShader(shaderId);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    // Check compilation status of the shader (not the program)
    // shaderId is the local shader object, not the final program
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        printf("Shader '%d' compile error: '%s'\n", type, errorLog);
        glDeleteShader(shaderId);
        return;
    }

    glAttachShader(programId, shaderId);
    // Clean up shader object after attaching to program (no longer needed)
    glDeleteShader(shaderId);
}

void CompileShaders(ShaderData& d) {
    GLuint shader = glCreateProgram();

    if (!shader) {
        printf("No shader program created!\n");
        return;
    }

    AddShader(shader, d.vert, GL_VERTEX_SHADER);
    AddShader(shader, d.frag, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    // Link the vertex and fragment shaders into a single program
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        printf("Shader link error: '%s'\n", errorLog);
        glDeleteProgram(shader);
        return;
    }

    // Validate the program to ensure it can run in the current OpenGL state
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        printf("Shader validate error: '%s'\n", errorLog);
        glDeleteProgram(shader);
        return;
    }

    d.shader = shader;
}

int main() {
    if (!glfwInit()) {
        printf("GLFW init failed\n");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow) {
        printf("GLFW window creation failed\n");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("GLEW init failed\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    ShaderData sd1;
    sd1.vert = vShader;
    sd1.frag = fShader;
    CompileShaders(sd1);

    DrawData dd1;
    dd1.shader = sd1.shader;
    CreateTriangle(dd1, triangle1, 9);  // 9 floats = 3 vertices * 3 coords

    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(dd1.shader);
        glBindVertexArray(dd1.VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    // Clean up OpenGL resources to avoid memory leaks
    glDeleteVertexArrays(1, &dd1.VAO);
    glDeleteBuffers(1, &dd1.VBO);
    glDeleteProgram(dd1.shader);
    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 0;
}