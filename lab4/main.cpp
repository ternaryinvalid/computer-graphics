#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Вершинный шейдер (Vertex Shader) для преобразования вершин
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;  // Позиция вершины
layout(location = 1) in vec3 aColor; // Цвет вершины
layout(location = 2) in vec3 aNormal; // Нормаль вершины
out vec3 ourColor;  // Цвет фрагмента
out vec3 FragPos;  // Позиция фрагмента
out vec3 Normal;   // Нормаль фрагмента
uniform mat4 model;    // Модельная матрица
uniform mat4 view;     // Видовая матрица
uniform mat4 projection; // Проекционная матрица
void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0f));  // Преобразование позиции вершины с учетом модели
    Normal = mat3(transpose(inverse(model))) * aNormal; // Преобразование нормали
    gl_Position = projection * view * vec4(FragPos, 1.0f); // Преобразование позиции с учетом проекции и вида
    ourColor = aColor;  // Передача цвета в фрагментный шейдер
})";

// Фрагментный шейдер (Fragment Shader) для расчета освещения
const char* fragmentShaderSource = R"(
#version 330 core
in vec3 ourColor;   // Цвет фрагмента
in vec3 FragPos;    // Позиция фрагмента
in vec3 Normal;     // Нормаль фрагмента
out vec4 FragColor; // Итоговый цвет фрагмента
uniform vec3 lightPos; // Позиция источника света
uniform vec3 viewPos;  // Позиция камеры
uniform float specularPower;  // Степень спекулярного освещения
uniform float specularIntensity; // Интенсивность спекулярного освещения
void main()
{
    // Амбиентное освещение: постоянный общий свет
    vec3 ambient = 0.1f * ourColor;

    // Диффузное освещение: свет, зависящий от угла между нормалью и направлением на источник света
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); // Направление на источник света
    float diff = max(dot(norm, lightDir), 0.0f); // Расчет диффузного освещения
    vec3 diffuse = diff * ourColor; // Диффузный свет, умноженный на цвет

    // Спекулярное освещение: зеркальное отражение света
    vec3 viewDir = normalize(viewPos - FragPos);  // Направление на камеру
    vec3 reflectDir = reflect(-lightDir, norm);   // Отраженный вектор
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), specularPower);  // Расчет спекулярного компонента
    vec3 specular = spec * specularIntensity * vec3(1.0f);  // Интенсивность спекулярного освещения

    // Итоговый цвет: сумма всех компонентов освещения
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);  // Запись итогового цвета
})";

// Вершины для простой пирамиды с цветами и нормалями
GLfloat vertices[] = {
    // Позиции             // Цвета            // Нормали
    0.0f,  0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // Верхняя передняя вершина
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  -0.5f, -0.5f, -0.5f,  // Нижняя задняя левая вершина
    0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.5f, -0.5f, -0.5f,  // Нижняя задняя правая вершина

    0.0f,  0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // Верхняя передняя вершина
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.5f, -0.5f, -0.5f,  // Нижняя задняя правая вершина
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.5f, -0.5f, 0.5f,  // Нижняя передняя правая вершина

    0.0f,  0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // Верхняя передняя вершина
    0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.5f, -0.5f, 0.5f,  // Нижняя передняя правая вершина
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  -0.5f, -0.5f, 0.5f,  // Нижняя передняя левая вершина

    0.0f,  0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // Верхняя передняя вершина
    -0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,  // Нижняя передняя левая вершина
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  -0.5f, -0.5f, -0.5f  // Нижняя задняя левая вершина
};

// Индексы для отрисовки пирамиды с использованием элементов
GLuint indices[] = {
    0, 1, 2,  3, 4, 5,  6, 7, 8,  9, 10, 11
};

GLuint VAO, VBO, EBO;  // Объект вершинного массива, буфер вершин и индексный буфер
GLuint vertexShader, fragmentShader, shaderProgram; // Шейдеры и программа

// Параметры для управления вращением куба и освещением
float angleX = 0.0f; // Вращение по оси X
float angleY = 0.0f; // Вращение по оси Y
glm::vec3 lightPos(2.0f, 2.0f, 2.0f); // Позиция источника света
glm::vec3 viewPos(0.0f, 0.0f, 3.0f);  // Позиция камеры
float specularPower = 32.0f; // Степень спекулярного освещения
float specularIntensity = 1.0f; // Интенсивность спекулярного освещения

// Компиляция шейдера
void compileShader(GLuint shader, const char* source) {
    glShaderSource(shader, 1, &source, NULL);  // Привязка исходного кода шейдера
    glCompileShader(shader);  // Компиляция шейдера
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);  // Проверка на успешную компиляцию
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);  // Получение информации об ошибке
        std::cout << "Shader Compilation Failed\n" << infoLog << std::endl;
    }
}

// Создание и компиляция шейдерной программы
void createShaderProgram() {
    // Создание шейдеров
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(vertexShader, vertexShaderSource);
    compileShader(fragmentShader, fragmentShaderSource);

    // Создание и линковка программы
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);  // Проверка на успешную линковку программы
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);  // Получение информации об ошибке
        std::cout << "Program Linking Failed\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);  // Использование программы шейдеров
}

// Обработка пользовательского ввода
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);  // Закрытие окна при нажатии на ESC

    // Управление вращением куба
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        angleX += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        angleX -= 0.1f;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        angleY -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        angleY += 0.1f;

    // Управление положением света
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        lightPos.y += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        lightPos.y -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        lightPos.x -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        lightPos.x += 0.1f;
}

// Коллбэк для изменения размера окна
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);  // Устанавливаем новый размер окна
}

int main() {
    glfwInit();  // Инициализация GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Lighting Example", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewInit();  // Инициализация GLEW

    // Создание шейдерной программы
    createShaderProgram();

    // Генерация VAO, VBO и EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); 

    // Основной цикл
    while (!glfwWindowShouldClose(window)) {
        processInput(window);  // Обработка ввода

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);  // Включение глубинного теста

        // Преобразования: модельная, видовая и проекционная матрица
        glm::mat4 model = glm::mat4(1.0f);  // Единичная матрица для модели
        model = glm::rotate(model, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));  // Вращение по оси X
        model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));  // Вращение по оси Y

        glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Камера
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);  // Перспективная проекция

        // Передача матриц и параметров в шейдеры
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
        glUniform1f(glGetUniformLocation(shaderProgram, "specularPower"), specularPower);
        glUniform1f(glGetUniformLocation(shaderProgram, "specularIntensity"), specularIntensity);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);  // Рисование куба
        glBindVertexArray(0);

        glfwSwapBuffers(window);  // Обмен буферов
        glfwPollEvents();  // Обработка событий
    }

    glfwTerminate();  // Завершение работы GLFW
    return 0;
}
