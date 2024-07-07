# Библиотека AlashKeypadMatrix
Библиотека AlashKeypadMatrix предназначена для управления матричными клавиатурами в проектах с Arduino. Она упрощает работу с состояниями клавиш и событиями через структурированный подход:

Состояния Клавиш: Определяет состояния клавиш как IDLE (ожидание), PRESSED (нажатие), HOLD (удержание) и RELEASED (отпускание).
Инициализация: Предоставляет конструкторы для инициализации клавиш и обновления их состояний.
Совместимость с Arduino: Обрабатывает разные версии Arduino для последовательных операций с цифровыми входами/выходами.
Макроопределения: Определяет состояния OPEN (открыто) и CLOSED (закрыто) для ясности в операциях с матрицами.
Обработка событий: Поддерживает слушателей событий для реагирования на изменения состояний клавиш.
Вспомогательные функции: Включает функции для установки времени дребезга, времени удержания и проверки состояния нажатия клавиш.
Целью этой библиотеки является упрощение интеграции матричных клавиатур в проекты Arduino, обеспечивая эффективную обработку пользовательских вводов через четкий и структурированный интерфейс API.
