#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// Функция для настройки терминала (неблокирующий ввод)
void setNonBlockingMode(bool enable) {
    // static переменные сохраняют свои значения между вызовами функции
    static struct termios oldt, newt;  // Структуры для хранения настроек терминала
    
    if (enable) {
        // Получаем текущие настройки терминала и сохраняем в oldt
        tcgetattr(STDIN_FILENO, &oldt);
        // Копируем настройки в newt для модификации
        newt = oldt;
        // Отключаем флаги: 
        // ICANON - канонический режим (буферизация до Enter)
        // ECHO - отображение вводимых символов на экране
        newt.c_lflag &= ~(ICANON | ECHO);
        // Применяем новые настройки немедленно (TCSANOW)
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        // Восстанавливаем оригинальные настройки терминала
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

// Функция для проверки нажатия клавиши
bool kbhit() {
    // Структура для хранения времени ожидания
    struct timeval tv;
    // Структура для набора файловых дескрипторов
    fd_set fds;
    // Устанавливаем время ожидания: 0 секунд, 0 микросекунд (неблокирующая проверка)
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    // Очищаем набор дескрипторов
    FD_ZERO(&fds);
    // Добавляем стандартный ввод (STDIN_FILENO) в набор дескрипторов
    FD_SET(STDIN_FILENO, &fds);
    // Проверяем, готов ли дескриптор для чтения
    // STDIN_FILENO+1 - максимальный номер дескриптора + 1
    // &fds - набор дескрипторов для проверки на чтение
    // NULL - не проверяем на запись
    // NULL - не проверяем на ошибки  
    // &tv - время ожидания (0 = немедленно)
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    // Возвращаем true если STDIN_FILENO готов для чтения (есть данные)
    return FD_ISSET(STDIN_FILENO, &fds);
}

// Функция для чтения символа
char getch() {
    // Переменная для хранения прочитанного символа
    char ch;
    // Читаем 1 байт из стандартного ввода в переменную ch
    // read возвращает количество успешно прочитанных байтов
    int bytesRead = read(STDIN_FILENO, &ch, 1);
    // Если прочитан хотя бы 1 байт, возвращаем символ, иначе нулевой символ
    return (bytesRead > 0) ? ch : '\0';
}
