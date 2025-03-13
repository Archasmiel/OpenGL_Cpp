#include <stdio.h>
#include <GL/glew.h>      // Підключення GLEW для управління розширеннями OpenGL
#include <GLFW/glfw3.h>   // Підключення GLFW для створення вікон та обробки вводу

const GLint WIDTH = 800;   // Ширина вікна
const GLint HEIGHT = 600;  // Висота вікна

int main()
{
    // Ініціалізація GLFW
    if (glfwInit() == GL_FALSE) { // Перевірка, чи вдалося ініціалізувати GLFW
        printf("GLFW init fail");
        glfwTerminate();
        return 1;
    }

    /// Налаштування GLFW
    // Визначення версії OpenGL (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // Велика версія (major)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // Маленька версія (minor)

    // Використання тільки сучасного (core) профілю OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Дозволяє використання майбутніх версій OpenGL (для macOS)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Створення вікна GLFW (посилання на нього зберігається у змінній window)
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
    if (!window) {  // Перевірка, чи вдалося створити вікно
        printf("Window creation fail");
        glfwTerminate();
        return 1;
    }

    // Отримання фактичного розміру буфера кадру (framebuffer) в пікселях
    int bufWidth, bufHeight;
    glfwGetFramebufferSize(window, &bufWidth, &bufHeight);

    // Встановлення контексту OpenGL для створеного вікна
    glfwMakeContextCurrent(window);

    // Дозвіл на використання експериментальних функцій GLEW
    glewExperimental = GL_TRUE;

    /// Ініціалізація GLEW
    if (glewInit() != GLEW_OK) {  // Перевірка, чи вдалося ініціалізувати GLEW
        printf("GLEW not init");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Встановлення області перегляду (viewport) на весь розмір буфера кадру
    glViewport(0, 0, bufWidth, bufHeight);

    //
    while (!glfwWindowShouldClose(window)) {  // Головний цикл програми
        // Оновлення подій (клавіатура, миша тощо)
        glfwPollEvents();

        // Очищення екрану кольором (GLclampf - значення у діапазоні [0,1])
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // Червоний, Зелений, Синій, Прозорість (RGBA)
        glClear(GL_COLOR_BUFFER_BIT);

        /*
            GL_COLOR_BUFFER_BIT - очищує буфер кольору (видаляє попередній кадр)
            GL_DEPTH_BUFFER_BIT - очищує буфер глибини (для коректного відображення 3D-об'єктів)
            GL_STENCIL_BUFFER_BIT - очищує буфер трафарету (використовується для маскування областей)
        */

        // Заміна переднього буфера (front) на задній (back), щоб відобразити новий кадр
        glfwSwapBuffers(window);
    }

    // Знищення вікна та завершення роботи GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
