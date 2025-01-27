# 📊 Компьютерная графика на C++ (OpenGL & SFML)

Этот репозиторий содержит решения для лабораторных работ по курсу **Компьютерная графика**. Каждая лабораторная работа фокусируется на различных аспектах 2D и 3D графики, таких как алгоритмы рендеринга, работа с матрицами трансформаций, освещением, трассировкой лучей и взаимодействием с пользователем. Все работы реализованы с использованием **C++**, **OpenGL** и **SFML**, что позволяет создавать графические приложения с использованием современных технологий.

---

## 🚀 Лабораторные работы

### 1. **Алгоритм Брезенхэма для окружности**
**Цель лабораторной работы:** Изучение алгоритма Брезенхэма для отрисовки окружности с заданным радиусом. Реализация динамического изменения радиуса окружности и анимации пульсации.

**Описание:**
- Реализован алгоритм Брезенхэма для эффективного рисования окружности с центром в произвольной точке экрана.
- Добавлена возможность изменения радиуса окружности в реальном времени через интерфейс.
- Для улучшения визуализации была добавлена анимация пульсации окружности, при которой радиус увеличивается и уменьшается по времени.

**Особенности реализации:**
- Используется **C++**, **OpenGL** и **SFML** для рендеринга и взаимодействия с пользователем.
- Проект обновляется в реальном времени с динамическими изменениями.

---

### 2. **Построение 3D-сцены с несколькими источниками света (без освещения)**
**Цель лабораторной работы:** Изучение основ работы с 3D-графикой: построение сцены с несколькими объектами, использование перспективной проекции для их отображения.

**Описание:**
- Реализована 3D-сцена, на которой отображаются простые геометрические объекты, такие как куб, пирамида и цилиндр.
- Для отображения объектов используется **перспективная проекция**, что позволяет создать ощущение глубины на экране.
- Для улучшения визуализации были добавлены несколько источников света, которые отображаются как простые объекты (сферы), без расчета освещения.

**Дополнительные функции:**
- Возможность перемещения источников света по сцене, что позволяет наблюдать изменения на 3D-объектах.
- Программирование на **C++** с использованием **OpenGL** и **SFML** для взаимодействия с пользователем.

---

### 3. **Реализация матриц трансформаций в 3D**
**Цель лабораторной работы:** Ознакомление с базовыми матричными операциями для трансформаций объектов в 3D-пространстве.

**Описание:**
- Реализована возможность применения базовых 3D-трансформаций: перемещения, масштабирования и поворота для заданного объекта (например, сферы).
- Используются **матричные операции** для изменения порядка применения трансформаций, что позволяет лучше понять, как эти операции взаимодействуют друг с другом.

**Особенности:**
- Программа позволяет изменять порядок применения матриц (сначала поворот, затем перемещение или наоборот).
- Визуализация всех применяемых трансформаций в реальном времени.
- Работа с **OpenGL** и **SFML** для отображения объектов и управления ими.

---

### 4. **Бликовое освещение с контролируемой интенсивностью**
**Цель лабораторной работы:** Изучение основ освещения в 3D-графике, использование бликового освещения (specular lighting) для создания эффекта бликов на поверхностях объектов.

**Описание:**
- Реализовано **бликовое освещение** (specular lighting) для 3D-объекта (пирамиды), позволяющее создавать реалистичные блески на поверхности.
- Программа позволяет управлять интенсивностью блика и параметрами материала объекта (например, сила зеркального блика, specular power).
- Возможность изменения положения источника света и наблюдения за изменениями на объекте.

**Особенности:**
- Все параметры освещения настраиваются через интерфейс.
- Используется **OpenGL** и **SFML** для отображения объекта и управления источниками света.

---

### 5. **Трассировка лучей с распределённым освещением**
**Цель лабораторной работы:** Реализация алгоритма **трассировки лучей** для создания более реалистичной 3D-графики, включая эффекты отражений, теней и освещения.

**Описание:**
- В этой лабораторной работе реализован **алгоритм трассировки лучей**, который позволяет физически корректно рассчитывать отражения, преломления и тени.
- Используется **распределённое освещение**, что позволяет получить более реалистичные эффекты, такие как блёстки или шероховатости на поверхности.
- Для симуляции эффектов шероховатых поверхностей реализовано случайное распределение лучей.

**Особенности:**
- Программа строит сцену с несколькими объектами (сферы, плоскости), где используется трассировка лучей для расчета освещенности и отражений.
- Возможность контроля уровня детализации освещения через управление количеством случайных выборок лучей.
- Реализовано на **C++** с использованием **OpenGL** и **SFML** для визуализации.

---

## ⚙️ Инструкции по запуску

1. **Клонируйте репозиторий:**

   ```bash
   git clone https://github.com/ternaryinvalid/computer-graphics-new.git

