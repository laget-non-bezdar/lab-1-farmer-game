#include <stdio.h>
#include <locale.h>
#include <windows.h>

// значения, которые там даны
#define INVENTORY_SIZE 10
#define HOURS_IN_DAY 24
#define START_DAY 1
#define START_HOUR 8
#define MIN_ITEM_ID 1
#define MAX_ITEM_ID 9

// функция для вывода названия предмета по ID
void print_item_name(int id) {
    switch (id) {
        case 0: printf("Пусто"); break;
        case 1: printf("Дерево"); break;
        case 2: printf("Камень"); break;
        case 3: printf("Семена"); break;
        case 4: printf("Лейка"); break;
        case 5: printf("Мотыга"); break;
        case 6: printf("Яблоко"); break;
        case 7: printf("Пшеница"); break;
        case 8: printf("Молоко"); break;
        case 9: printf("Золотая монета"); break;
        default: printf("Неизвестный предмет"); break;
    }
}

// функция для вывода всего инвентаря
void print_inventory(int inventory[]) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == 0) {
            printf("Слот %d: [0] (Пусто)\n", i);
        } else {
            printf("Слот %d: [%d] (", i, inventory[i]);
            print_item_name(inventory[i]);
            printf(")\n");
        }
    }
}

// очистка буфера ввода (е мае защита от дурака)
void clear_input() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int main() {
    // русский язык не помешает поди
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    // инициализация начальных переменных
    int current_day = START_DAY;
    int current_hour = START_HOUR;

    // создаем массив
    int inventory[INVENTORY_SIZE] = {0};

    // раскладываем предметы заранее специально под вариант 10 (с дубликатами)
    inventory[0] = 1; // Дерево
    inventory[1] = 2; // Камень
    inventory[2] = 1; // Дерево (дубликат)
    inventory[3] = 3; // Семена
    inventory[4] = 0; // Пусто
    inventory[5] = 2; // Камень (дубликат)
    inventory[6] = 6; // Яблоко
    inventory[7] = 3; // Семена (дубликат)
    inventory[8] = 1; // Дерево (дубликат)
    inventory[9] = 4; // Лейка

    int choice = -1;

    printf("=== Первые шаги весёлого фермера ===\n");
    // цикл 
    while (1) {
        printf("\n---------------- МЕНЮ ----------------\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Устранить дубликаты (Вариант 10)\n");
        printf("[0] Выход\n");
        printf("--------------------------------------\n");
        printf("Выберите пункт меню: ");

        // защита от дураков не дураков
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка! Нужно ввести целое число, а не буквы.\n");
            clear_input();
            continue;
        }
        clear_input(); // убираем лишние символы

        // если выбрали выход, то прерываем цикл
        if (choice == 0) {
            printf("Выход из игры. До встречи!\n");
            break;
        }

        // обработка выбора через switch (по методичке)
        switch (choice) {
            case 1: {
                printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
                break;
            }

            case 2: {
                int work_hours = 0;
                printf("Сколько часов вы хотите поработать? ");

                if (scanf("%d", &work_hours) != 1 || work_hours <= 0) {
                    printf("Ошибка! Количество часов должно быть положительным числом.\n");
                    clear_input();
                    break;
                }
                clear_input();

                current_hour = current_hour + work_hours;

                // если часов стало 24+, то новый день
                if (current_hour >= HOURS_IN_DAY) {
                    current_day = current_day + (current_hour / HOURS_IN_DAY);
                    current_hour = current_hour % HOURS_IN_DAY;
                }

                printf("Вы хорошо потрудились! Теперь: День %d, %02d:00\n", current_day, current_hour);
                break;
            }

            case 3: {
                printf("\nСодержимое вашего рюкзака:\n");
                print_inventory(inventory);
                break;
            }

            case 4: {
                int slot_index = 0;
                int item_id = 0;

                printf("Введите номер слота (от 0 до %d): ", INVENTORY_SIZE - 1);
                if (scanf("%d", &slot_index) != 1) {
                    printf("Ошибка! Нужно ввести число.\n");
                    clear_input();
                    break;
                }
                clear_input();

                // проверка границ
                if (slot_index < 0 || slot_index >= INVENTORY_SIZE) {
                    printf("Ошибка! Такого слота нет. Допустимы слоты от 0 до %d.\n", INVENTORY_SIZE - 1);
                    break;
                }

                printf("Доступные предметы:\n");
                printf("1 - Дерево, 2 - Камень, 3 - Семена, 4 - Лейка, 5 - Тяпка,\n");
                printf("6 - Яблоко, 7 - Пшеница, 8 - Молоко, 9 - Золотая монета\n");
                printf("Введите ID предмета (от %d до %d): ", MIN_ITEM_ID, MAX_ITEM_ID);

                if (scanf("%d", &item_id) != 1) {
                    printf("Ошибка! Нужно ввести число.\n");
                    clear_input();
                    break;
                }
                clear_input();

                if (item_id < MIN_ITEM_ID || item_id > MAX_ITEM_ID) {
                    printf("Ошибка! ID предмета должен быть от %d до %d.\n", MIN_ITEM_ID, MAX_ITEM_ID);
                    break;
                }

                inventory[slot_index] = item_id;
                printf("Предмет [");
                print_item_name(item_id);
                printf("] успешно положен в слот %d!\n", slot_index);
                break;
            }

            case 5: {
                int slot_index = 0;
                printf("Введите номер слота для очистки (от 0 до %d): ", INVENTORY_SIZE - 1);

                if (scanf("%d", &slot_index) != 1) {
                    printf("Ошибка! Нужно ввести число.\n");
                    clear_input();
                    break;
                }
                clear_input();

                if (slot_index < 0 || slot_index >= INVENTORY_SIZE) {
                    printf("Ошибка! Такого слота нет. Допустимы слоты от 0 до %d.\n", INVENTORY_SIZE - 1);
                    break;
                }

                if (inventory[slot_index] == 0) {
                    printf("Слот %d уже пуст.\n", slot_index);
                } else {
                    printf("Предмет [");
                    print_item_name(inventory[slot_index]);
                    printf("] выброшен из слота %d.\n", slot_index);
                    inventory[slot_index] = 0;
                }
                break;
            }

            case 6: {
                // вариант 10 пошло задание короче
                printf("\n--- Инвентарь ДО устранения дубликатов ---\n");
                print_inventory(inventory);

                int removed_count = 0;

                // берем каждый слот по очереди 
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    // пустые слоты скипаем, ибо задание такое
                    if (inventory[i] == 0) {
                        continue;
                    }

                    // проверяем следующие слоты справа от него
                    for (int j = i + 1; j < INVENTORY_SIZE; j++) {
                        // если нашли точно такой же предмет — "делим на ноль" его XD
                        if (inventory[j] == inventory[i]) {
                            inventory[j] = 0;
                            removed_count++;
                        }
                    }
                }

                printf("\n--- Инвентарь ПОСЛЕ устранения дубликатов ---\n");
                print_inventory(inventory);
                printf("Убрано дубликатов: %d\n", removed_count);
                break;
            }

            default: {
                printf("Такого пункта нет в меню! Выберите число от 0 до 6\n");
                break;
            }
        }
    }

    return 0;
}