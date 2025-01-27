#define GL_SILENCE_DEPRECATION

#include <SFML/Graphics.hpp>
#include <GLUT/glut.h>
#include <iostream>
#include <vector>

#define TRANSFORM_SPEED 0.1f
#define ROTATION_SPEED 2.0f

float scale = 1.0f;
float rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
float translateX = 0.0f, translateY = 0.0f, translateZ = -5.0f;

std::vector<std::string> transformationOrder = {"scale", "rotate", "translate"};

void handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) translateY += TRANSFORM_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) translateY -= TRANSFORM_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) translateX -= TRANSFORM_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) translateX += TRANSFORM_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) translateZ += TRANSFORM_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) translateZ -= TRANSFORM_SPEED;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) rotationX += ROTATION_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) rotationX -= ROTATION_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) rotationY += ROTATION_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) rotationY -= ROTATION_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) rotationZ += ROTATION_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) rotationZ -= ROTATION_SPEED;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && scale < 10.0f) scale += 0.01f; // Ограничение масштаба
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && scale > 0.01f) scale -= 0.01f; // Ограничение масштаба

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        transformationOrder = {"scale", "rotate", "translate"};
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
        transformationOrder = {"translate", "rotate", "scale"};
    }
}

void applyTransformations() {
    for (const auto& transformation : transformationOrder) {
        if (transformation == "scale") {
            glScalef(scale, scale, scale);
        } else if (transformation == "rotate") {
            glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
            glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
            glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);
        } else if (transformation == "translate") {
            glTranslatef(translateX, translateY, translateZ);
        }
    }
}

void drawSphere() {
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH); // Добавление нормалей для освещения
    gluSphere(quadric, 1.0f, 30, 30); // Можно увеличить количество сегментов
    gluDeleteQuadric(quadric);
}

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    sf::RenderWindow window(sf::VideoMode(800, 600), "3D Transformations", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    // Инициализация OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    GLfloat lightPos[] = {1.0f, 1.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Установка матрицы проекции
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800 / 600.f, 0.1f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW); // Вернуться к модели/виду

    // Основной цикл
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        handleInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        // Установка камеры
        gluLookAt(0.0f, 0.0f, 10.0f,
                  0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f);

        glPushMatrix();
        applyTransformations();
        drawSphere();
        glPopMatrix();

        window.display();
    }

    return 0;
}