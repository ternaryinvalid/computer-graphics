#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

// Структура луча, содержащая начальную точку (origin) и направление (direction)
struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

// Структура сферы с методом проверки пересечения
struct Sphere {
    glm::vec3 center;  // Центр сферы
    float radius;      // Радиус сферы
    glm::vec3 color;   // Цвет сферы

    // Метод проверки пересечения луча с поверхностью сферы
    bool intersect(const Ray& ray, float& t) const {
        glm::vec3 oc = ray.origin - center;  // Вектор от центра сферы до начальной точки луча
        float b = glm::dot(oc, ray.direction);  // Скалярное произведение
        float c = glm::dot(oc, oc) - radius * radius;  // Уравнение сферы
        float discriminant = b * b - c;  // Дискриминант для проверки пересечения
        if (discriminant > 0) {
            float sqrtDiscriminant = sqrt(discriminant);
            t = -b - sqrtDiscriminant;  // Ближайшая точка пересечения
            if (t > 0) return true;
            t = -b + sqrtDiscriminant;  // Альтернативная точка пересечения
            return t > 0;
        }
        return false;  // Пересечения нет
    }
};

// Структура плоскости с методом проверки пересечения
struct Plane {
    glm::vec3 point;   // Точка на плоскости
    glm::vec3 normal;  // Нормаль плоскости
    glm::vec3 color;   // Цвет плоскости

    // Метод проверки пересечения луча с плоскостью
    bool intersect(const Ray& ray, float& t) const {
        float denom = glm::dot(normal, ray.direction);  // Проверка на параллельность
        if (abs(denom) > 1e-6) {  // Если нормаль не перпендикулярна лучу
            glm::vec3 p0l0 = point - ray.origin;
            t = glm::dot(p0l0, normal) / denom;  // Вычисление расстояния до пересечения
            return (t >= 0);
        }
        return false;  // Пересечения нет
    }
};

// Структура источника света
struct Light {
    glm::vec3 position;  // Позиция света
    glm::vec3 color;     // Цвет света
};

// Функция Перлина для создания текстурного шума
float perlinNoise(const glm::vec3& point) {
    float n = sin(glm::dot(point, glm::vec3(12.9898f, 78.233f, 45.164f))) * 43758.5453f;
    return (sin(n) - 1.0f) / 2.0f;  // Возвращает значение шума в диапазоне [-0.5, 0.5]
}

// Функция трассировки лучей, обрабатывающая пересечения, освещение, отражения и т.д.
glm::vec3 trace(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const Light& light, const glm::vec3& viewPos, int depth = 0) {
    if (depth > 3) return glm::vec3(0.0f);  // Ограничение глубины рекурсии

    float closest_t = std::numeric_limits<float>::max();  // Ближайшее пересечение
    glm::vec3 closest_color = glm::vec3(0.0f);  // Цвет объекта пересечения
    glm::vec3 hitPoint;  // Точка пересечения
    glm::vec3 normal;    // Нормаль поверхности в точке пересечения
    bool hit = false;    // Флаг пересечения

    // Проверка пересечения со сферами
    for (const Sphere& sphere : spheres) {
        float t;
        if (sphere.intersect(ray, t) && t < closest_t) {
            closest_t = t;
            hit = true;
            hitPoint = ray.origin + t * ray.direction;  // Вычисление точки пересечения
            normal = glm::normalize(hitPoint - sphere.center);  // Нормаль к поверхности
            closest_color = sphere.color;

            // Применение текстурного шума Перлина
            float noise = perlinNoise(hitPoint);
            closest_color = closest_color * (0.5f + 0.5f * noise);
        }
    }

    // Проверка пересечения с плоскостями
    for (const Plane& plane : planes) {
        float t;
        if (plane.intersect(ray, t) && t < closest_t) {
            closest_t = t;
            hit = true;
            hitPoint = ray.origin + t * ray.direction;  // Вычисление точки пересечения
            normal = plane.normal;  // Нормаль к поверхности
            closest_color = plane.color;
        }
    }

    if (!hit) return glm::vec3(0.0f);  // Если пересечений нет, возвращаем черный цвет

    // Вычисление амбиентного и диффузного освещения
    glm::vec3 ambient = 0.1f * closest_color;  // Амбиентное освещение
    glm::vec3 lightDir = glm::normalize(light.position - hitPoint);  // Направление на источник света
    float diff = glm::max(glm::dot(normal, lightDir), 0.0f);  // Диффузная компонента
    glm::vec3 diffuse = diff * closest_color;

    // Логика отражения
    glm::vec3 reflectDir = glm::normalize(glm::reflect(ray.direction, normal));  // Направление отраженного луча
    Ray reflectRay = { hitPoint + normal * 0.001f, reflectDir };  // Смещение начальной точки отражения
    glm::vec3 reflectColor = trace(reflectRay, spheres, planes, light, viewPos, depth + 1);  // Рекурсивная трассировка

    return ambient + diffuse + 0.5f * reflectColor;  // Суммируем компоненты освещения
}

// Функция рендера сцены
void renderScene(const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const Light& light, const glm::vec3& viewPos, int width, int height) {
    std::vector<glm::vec3> framebuffer(width * height);  // Буфер кадра
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float u = (x + 0.5f) / float(width) * 2.0f - 1.0f;  // Нормализованные координаты
            float v = (y + 0.5f) / float(height) * 2.0f - 1.0f;

            Ray ray = { viewPos, glm::normalize(glm::vec3(u, v, -1.0f)) };  // Создание луча

            framebuffer[y * width + x] = trace(ray, spheres, planes, light, viewPos);  // Трассировка луча
        }
    }

    // Отображение буфера кадра
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (int i = 0; i < framebuffer.size(); ++i) {
        glColor3f(framebuffer[i].r, framebuffer[i].g, framebuffer[i].b);  // Цвет пикселя
        glVertex2f((i % width) / float(width) * 2.0f - 1.0f, (i / width) / float(height) * 2.0f - 1.0f);
    }
    glEnd();
    glFlush();
}

// Обработка ввода для перемещения источника света
void processInput(GLFWwindow* window, Light& light, float deltaTime) {
    const float movementSpeed = 5.0f;
    glm::vec3 targetPosition = light.position;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        targetPosition.y += movementSpeed * deltaTime;  // Движение вверх
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        targetPosition.y -= movementSpeed * deltaTime;  // Движение вниз
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        targetPosition.x -= movementSpeed * deltaTime;  // Влево
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        targetPosition.x += movementSpeed * deltaTime;  // Вправо
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        targetPosition.z += movementSpeed * deltaTime;  // Вперед
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        targetPosition.z -= movementSpeed * deltaTime;  // Назад
    }

    float smoothFactor = 0.3f;  // Фактор сглаживания
    light.position += (targetPosition - light.position) * smoothFactor;  // Плавное перемещение
}

// Основная функция
int main() {
    // Инициализация GLFW и OpenGL
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ray Tracing with New Colors", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    // Создание сфер
    std::vector<Sphere> spheres = {
        { glm::vec3(0.0f, 0.0f, -3.0f), 1.0f, glm::vec3(1.0f, 1.0f, 0.0f) },  // Желтая сфера
        { glm::vec3(2.0f, 0.0f, -3.0f), 1.0f, glm::vec3(0.0f, 0.8f, 0.8f) },  // Голубая сфера
        { glm::vec3(-2.0f, 0.0f, -3.0f), 1.0f, glm::vec3(0.9f, 0.0f, 0.9f) }  // Розовая сфера
    };

    // Создание плоскости
    std::vector<Plane> planes = {
        { glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f) }  // Серая плоскость
    };

    // Источник света
    Light light = { glm::vec3(3.0f, 2.0f, -2.0f), glm::vec3(1.0f, 1.0f, 0.0f) };  // Желтый свет
    glm::vec3 viewPos(0.0f, 0.0f, 3.0f);  // Позиция камеры

    while (!glfwWindowShouldClose(window)) {
        float deltaTime = glfwGetTime();  // Вычисление времени между кадрами
        glfwSetTime(0.0);

        processInput(window, light, deltaTime);  // Обработка ввода
        renderScene(spheres, planes, light, viewPos, 800, 600);  // Рендер сцены

        glfwSwapBuffers(window);  // Обновление окна
        glfwPollEvents();  // Обработка событий
    }

    glfwTerminate();  // Завершение работы GLFW
    return 0;
}


