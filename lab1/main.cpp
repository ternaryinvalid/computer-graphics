#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

// Функция для отрисовки окружности с помощью алгоритма Брезенхэма
void drawCircle(RenderWindow& window, int centerX, int centerY, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius; // Начальное значение d

    // Рисуем точки в восьми секторах
    auto plotPoint = [&](int xOffset, int yOffset) {
        VertexArray points(PrimitiveType::Points, 8); // Создаем массив из 8 точек

        points[0] = Vertex(Vector2f(centerX + xOffset, centerY + yOffset), Color::Black);
        points[1] = Vertex(Vector2f(centerX - xOffset, centerY + yOffset), Color::Black);
        points[2] = Vertex(Vector2f(centerX + xOffset, centerY - yOffset), Color::Black);
        points[3] = Vertex(Vector2f(centerX - xOffset, centerY - yOffset), Color::Black);
        points[4] = Vertex(Vector2f(centerX + yOffset, centerY + xOffset), Color::Black);
        points[5] = Vertex(Vector2f(centerX - yOffset, centerY + xOffset), Color::Black);
        points[6] = Vertex(Vector2f(centerX + yOffset, centerY - xOffset), Color::Black);
        points[7] = Vertex(Vector2f(centerX - yOffset, centerY - xOffset), Color::Black);

        window.draw(points); // Рисуем все точки за один вызов
    };

    // Начинаем рисовать
    while (x <= y) {
        plotPoint(x, y);
        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void handleInput(Event event, string& inputString, int& radius) {
    if (event.type == Event::TextEntered) {
        if (event.text.unicode < 128) { // Проверяем на допустимые символы
            if (event.text.unicode == 'b') { // Обработка Backspace
                if (!inputString.empty())
                    inputString.pop_back();
            } else {
                inputString += static_cast<char>(event.text.unicode); // Добавляем символ в строку
            }
        }
    } else if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Enter) { // Обработка Enter
            int newRadius = stoi(inputString); // Пробуем преобразовать строку в число
            radius = max(5, newRadius); // Устанавливаем новый радиус
            inputString.clear(); // Очищаем строку после ввода
        }
    }
}

int main() {
    RenderWindow window(VideoMode(800, 600), "Circle Drawing with Bresenham's Algorithm");
    
    // Загружаем шрифт
    Font font;
    if (!font.loadFromFile("fonts/Roboto-Regular.ttf")) {
        return -1;
    }

    Text radiusText;
    radiusText.setFont(font);
    radiusText.setCharacterSize(24);
    radiusText.setFillColor(Color::Black);
    radiusText.setPosition(10, 10);

    Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(Color::Black);
    inputText.setPosition(10, 50);

    int radius = 50; // Начальный радиус
    Vector2i center(300, 200); // Центр окружности

    string inputString; // Строка для ввода радиуса

    // Ввод центра окружности
    cout << "Введите координаты центра окружности (x y): ";
    cin >> center.x >> center.y;

    while (window.isOpen()) {    
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            handleInput(event, inputString, radius); // Обработка ввода
        }

        // Обновляем текст с текущим радиусом
        radiusText.setString("Current Radius: " + to_string(radius));
        inputText.setString("Enter Radius: " + inputString);

        window.clear(Color::White);
        drawCircle(window, center.x, center.y, radius); // Отрисовка окружности

        window.draw(radiusText); // Отображаем текст с радиусом
        window.draw(inputText); // Отображаем текстовое поле для ввода
        window.display();
    }

    return 0;
}