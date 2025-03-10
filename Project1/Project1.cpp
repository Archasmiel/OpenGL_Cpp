#include <stdio.h>
#include <GL/glew.h>      
#include <GLFW/glfw3.h>

// Розміри вікна
const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
    // Ініціалізація GLFW
    if (!glfwInit()) {
        printf("GLFW init failed");
        glfwTerminate();
        return 1;
    }

    /// Настройки вікна
    // Головна частина MAJOR версії 3.X та "мала" частина MINOR Х.3, разом 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Немає ніяких старих залежностей (deprecated) та дозволити нові залежності нових версій
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Створення вікна (посилання на нього - символ *)
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow) {
        printf("GLFW window creation failed");
    }

    // Отримати розміри буфера кадрів з вікна
    // для правильного встановлення області перегляду (буде далі)
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
        // &bufferWidth - дозволяє передати змінну напряму і перезаписувати (reference)
        // якщо не додати '&', буде робити копію і міняти далі її

    // Зробити вікно головним
    glfwMakeContextCurrent(mainWindow);

    // Дозволити експериментальні(нові) властивості вікна
    glewExperimental = GL_TRUE;

    // Перевірка чи менеджер вікон GLEW запустився, не видає Bool
    if (glewInit() != GLEW_OK) {
        printf("GLEW init failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
    }

    // для правильного встановлення області перегляду
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Головний цикл вікна
    while (!glfwWindowShouldClose(mainWindow)) {
        // отримати всі введення користувача (мишка, клавіатура)
        glfwPollEvents();

        // Очистка (зафарбування всього буферу) одним кольором
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);   // R G B Alpha  [0f, 1f]  float
        glClear(GL_COLOR_BUFFER_BIT);  // спеціальне значення
            /*
               GL_COLOR_BUFFER_BIT - очищає буфер кольору, видаляючи попередній кадр
               GL_DEPTH_BUFFER_BIT - очищає буфер глибини, 
                запобігаючи некоректному порядку рендерингу
               GL_STENCIL_BUFFER_BIT - очищає буфер-трафарет (маска), який 
                використовується для маскування (ховання) деяких областей рендерингу
            */

        glfwSwapBuffers(mainWindow);  // замінити кадр у вікні на буфер в якому малювали
    }

    return 0;
}