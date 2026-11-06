#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

// константы
#define INVENTORY_SIZE 10
#define HOURS_IN_DAY 24
#define START_DAY 1
#define START_HOUR 8
#define MIN_ITEM_ID 1
#define MAX_ITEM_ID 9

// новые константы
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32
#define MAX_LINE_LEN 256

// очистка буфера ввода
void clear_input() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// загрузка названий предметов из файла items.txt
int load_items(char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    strcpy(item_names[0], "Пусто");
    strcpy(item_names[1], "Дерево");
    strcpy(item_names[2], "Камень");
    strcpy(item_names[3], "Семена");
    strcpy(item_names[4], "Лейка");
    strcpy(item_names[5], "Мотыга");
    strcpy(item_names[6], "Яблоко");
    strcpy(item_names[7], "Пшеница");
    strcpy(item_names[8], "Молоко");
    strcpy(item_names[9], "Золотая_монета");

    FILE *file = fopen("items.txt", "r");
    if (file == NULL) {
        return 0;
    }

    int id;
    char name[MAX_NAME_LEN];
    while (fscanf(file, "%d %31s", &id, name) == 2) {
        if (id >= 0 && id < MAX_ITEMS) {
            strcpy(item_names[id], name);
        }
    }
    fclose(file);
    return 1;
}

// вывод всего инвентаря с названиями из массива
void print_inventory(int inventory[], char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = inventory[i];
        printf("Слот %d: [%d] - %s\n", i, id, item_names[id]);
    }
}

// пересчет времени
void add_work_time(int work_hours, int *current_day, int *current_hour) {
    *current_hour = *current_hour + work_hours;
    if (*current_hour >= HOURS_IN_DAY) {
        *current_day = *current_day + (*current_hour / HOURS_IN_DAY);
        *current_hour = *current_hour % HOURS_IN_DAY;
    }
}

// устранение дубликатов
int remove_duplicates(int inventory[]) {
    int removed_count = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == 0) {
            continue;
        }
        for (int j = i + 1; j < INVENTORY_SIZE; j++) {
            if (inventory[j] == inventory[i]) {
                inventory[j] = 0;
                removed_count++;
            }
        }
    }
    return removed_count;
}

int main() {
    setlocale(LC_ALL, "");
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    int current_day = START_DAY;
    int current_hour = START_HOUR;
    int inventory[INVENTORY_SIZE] = {1, 2, 1, 3, 0, 2, 6, 3, 1, 4};

    // массив для названий предметов (3 лаба)
    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    if (load_items(item_names) == 0) {
        printf("[Предупреждение] Файл items.txt не найден! Загружены стандартные названия.\n");
    }

    int choice = -1;
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

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка! Нужно ввести целое число, а не буквы.\n");
            clear_input();
            continue;
        }
        clear_input();

        if (choice == 0) {
            printf("Выход из игры. До встречи!\n");
            break;
        }

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

                add_work_time(work_hours, &current_day, &current_hour);
                printf("Вы хорошо потрудились! Теперь: День %d, %02d:00\n", current_day, current_hour);
                break;
            }

            case 3: {
                printf("\nСодержимое вашего рюкзака:\n");
                print_inventory(inventory, item_names);
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

                if (slot_index < 0 || slot_index >= INVENTORY_SIZE) {
                    printf("Ошибка! Такого слота нет. Допустимы слоты от 0 до %d.\n", INVENTORY_SIZE - 1);
                    break;
                }

                printf("Доступные предметы:\n");
                for (int i = MIN_ITEM_ID; i <= MAX_ITEM_ID; i++) {
                    printf("%d - %s\n", i, item_names[i]);
                }
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
                printf("Предмет [%s] успешно положен в слот %d!\n", item_names[item_id], slot_index);
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
                    int old_id = inventory[slot_index];
                    inventory[slot_index] = 0;
                    printf("Предмет [%s] выброшен из слота %d.\n", item_names[old_id], slot_index);
                }
                break;
            }

            case 6: {
                printf("\n--- Инвентарь до устранения дубликатов ---\n");
                print_inventory(inventory, item_names);

                int removed_count = remove_duplicates(inventory);

                printf("\n--- Инвентарь после устранения дубликатов ---\n");
                print_inventory(inventory, item_names);
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