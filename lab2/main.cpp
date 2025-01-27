#define GL_SILENCE_DEPRECATION

#include <SFML/Graphics.hpp>
#include <GLUT/glut.h>

// Определение скоростей перемещения камеры и источников света
#define CAMERA_SPEED 0.01f
#define LIGHT_SPEED 0.05f

// Параметры камеры и перспективы
float cameraX = 0, cameraY = 0, cameraZ = 5; // Начальные координаты камеры
float fieldOfView = 45.0f; // Угол обзора камеры

// Параметры источников света (всего 3 источника)
float light1X = -2.0f, light1Y = 2.0f, light1Z = 0.0f;
float light2X = 2.0f, light2Y = 2.0f, light2Z = 2.0f;
float light3X = 0.0f, light3Y = -2.0f, light3Z = -2.0f;

// Обработчик ввода для камерыю
void handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) cameraZ -= CAMERA_SPEED; // Вперед
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) cameraZ += CAMERA_SPEED; // Назад
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) cameraX -= CAMERA_SPEED; // Влево
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) cameraX += CAMERA_SPEED; // Вправо
}

// Обработчик ввода для источников света
void handleLightMovement() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) light1X -= LIGHT_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) light1X += LIGHT_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) light1Y += LIGHT_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) light1Y -= LIGHT_SPEED;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) light2X -= LIGHT_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) light2X += LIGHT_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) light2Y += LIGHT_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) light2Y -= LIGHT_SPEED;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) light3X -= LIGHT_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) light3X += LIGHT_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) light3Y += LIGHT_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) light3Y -= LIGHT_SPEED;
}

void setCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0); // Позиционируем камеру
}

void setPerspective() {
    glMatrixMode(GL_PROJECTION); // Переключаемся на матрицу проекции
    glLoadIdentity(); // Сбрасываем матрицу
    gluPerspective(fieldOfView, 1500.0f / 1200.0f, 0.1f, 100.0f);  // Устанавливаем перспективу
}

// Отрисовка куба с нормалями
void drawCube() {
    glPushMatrix();
    glTranslatef(-2.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    // Передняя грань
    glNormal3f(0.0f, 0.0f, 1.0f);  // Нормаль для передней грани
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Задняя грань
    glNormal3f(0.0f, 0.0f, -1.0f);  // Нормаль для задней грани
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Левая грань
    glNormal3f(-1.0f, 0.0f, 0.0f);  // Нормаль для левой грани
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Правая грань
    glNormal3f(1.0f, 0.0f, 0.0f);   // Нормаль для правой грани
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Верхняя грань
    glNormal3f(0.0f, 1.0f, 0.0f);   // Нормаль для верхней грани
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Нижняя грань
    glNormal3f(0.0f, -1.0f, 0.0f);  // Нормаль для нижней грани
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glEnd();
    glPopMatrix();
}


// Отрисовка пирамиды
void drawPyramid() {
    glPushMatrix();
    glTranslatef(2.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // Передняя грань
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Правая грань
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Задняя грань
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor3f(1.0f, 1.0f, 0.0f); // Левая грань
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();
    glPopMatrix();
}

// Отрисовка цилиндра с крышкой и низом
void drawCylinder() {
    float radius = 0.5f; // Радиус основания цилиндра
    float height = 1.0f; // Высота цилиндра
    int slices = 30; // Количество сегментов для окружностей (основание и крышка)

    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    
    GLUquadric* quadric = gluNewQuadric();
    gluCylinder(quadric, radius, radius, height, slices, 1); // Тело цилиндра
    gluDeleteQuadric(quadric);

    // Рисуем крышку (верхнюю часть цилиндра)
    glBegin(GL_POLYGON);
    for (int i = 0; i <= slices; i++) {
        float theta = (2.0f * 3.14159f * i) / slices;
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex3f(x, y, height);
    }
    glEnd();

    // Рисуем низ (основание цилиндра)
    glBegin(GL_POLYGON);
    for (int i = 0; i <= slices; i++) {
        float theta = (2.0f * 3.14159f * i) / slices;
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    glPopMatrix();
}

// Отрисовка источников света
void drawLightSources() {
    glPushMatrix();
    glTranslatef(light1X, light1Y, light1Z);
    glColor3f(1.0f, 1.0f, 0.0f); // Желтые источники света
    glutSolidSphere(0.1, 10, 10); // Сфера для первого источника
    glPopMatrix();

    glPushMatrix();
    glTranslatef(light2X, light2Y, light2Z);
    glColor3f(0.0f, 1.0f, 0.0f); // Зеленые источники света
    glutSolidSphere(0.1, 10, 10); // Сфера для второго источника
    glPopMatrix();

    glPushMatrix();
    glTranslatef(light3X, light3Y, light3Z);
    glColor3f(0.0f, 0.0f, 1.0f); // Синие источники света
    glutSolidSphere(0.1, 10, 10); // Сфера для третьего источника
    glPopMatrix();
}

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    sf::RenderWindow window(sf::VideoMode(1500, 1200), "3D View", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Установка цвета фона (черный)
    glClearDepth(1.0f); // Установка глубины для буфера глубины
    glEnable(GL_DEPTH_TEST); // Включение теста глубины для корректного отображения 3D-объектов
    glDepthFunc(GL_LEQUAL); // Установка функции глубины
    glShadeModel(GL_SMOOTH); // Гладкая заливка поверхностей
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Установка высокого качества перспективы


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        handleInput();         // Обработка ввода для камеры
        handleLightMovement(); // Обработка ввода для источников света

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очищаем экран и буфер глубины

        setPerspective(); // Настроим перспективу
        setCamera(); // Настроим камеру

        // Отрисовка объектов
        drawCube();
        drawPyramid();
        drawCylinder();
        drawLightSources();

        window.display(); // Отображаем обновленное окно
    }

    return 0;
}

