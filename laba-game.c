#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

// Константы 
#define INVENTORY_SIZE 10
#define HOURS_IN_DAY 24
#define START_DAY 1
#define START_HOUR 8
#define MIN_ITEM_ID 1
#define MAX_ITEM_ID 9
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32
#define MAX_LINE_LEN 256
#define MIN_MAP_SIZE 8
#define MAX_MAP_SIZE 15
#define OBSTACLE_CHANCE 15
#define WIND_CHANCE 10
#define WIND_PUSH 3

void clear_input() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

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
    if (file == NULL) return 0;

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

void print_inventory(int inventory[], char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = inventory[i];
        printf("Слот %d: [%d] - %s\n", i, id, item_names[id]);
    }
}

void add_work_time(int work_hours, int *current_day, int *current_hour) {
    *current_hour = *current_hour + work_hours;
    if (*current_hour >= HOURS_IN_DAY) {
        *current_day = *current_day + (*current_hour / HOURS_IN_DAY);
        *current_hour = *current_hour % HOURS_IN_DAY;
    }
}

int remove_duplicates(int inventory[]) {
    int removed_count = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == 0) continue;
        for (int j = i + 1; j < INVENTORY_SIZE; j++) {
            if (inventory[j] == inventory[i]) {
                inventory[j] = 0;
                removed_count++;
            }
        }
    }
    return removed_count;
}

int find_item_id(char item_names[MAX_ITEMS][MAX_NAME_LEN], char target_name[]) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(item_names[i], target_name) == 0) return i;
    }
    return -1;
}

void write_diary(char farmer_name[], int day, int hour, int inventory[], char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    FILE *file = fopen("diary.txt", "a");
    if (file == NULL) {
        printf("Ошибка открытия diary.txt!\n");
        return;
    }
    printf("\n=== ЗАПИСЬ В ДНЕВНИК ФЕРМЕРА ===\n");
    printf("Фермер: %s | День %d, %02d:00\n", farmer_name, day, hour);
    fprintf(file, "=== ЗАПИСЬ В ДНЕВНИК ФЕРМЕРА ===\n");
    fprintf(file, "Фермер: %s | День %d, %02d:00\n", farmer_name, day, hour);

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = inventory[i];
        printf("Слот %d: [%d] - %s\n", i, id, item_names[id]);
        fprintf(file, "Слот %d: [%d] - %s\n", i, id, item_names[id]);
    }
    fprintf(file, "--------------------------------\n\n");
    fclose(file);
    printf("Сводка сохранена в diary.txt!\n");
}

void unpack_map_task() {
    FILE *in = fopen("input.txt", "r");
    if (in == NULL) {
        printf("Ошибка: файл input.txt не найден!\n");
        return;
    }
    FILE *out = fopen("output.txt", "w");
    if (out == NULL) {
        printf("Ошибка создания output.txt!\n");
        fclose(in);
        return;
    }

    char line[MAX_LINE_LEN];
    int expected_len = -1, line_num = 0, error = 0;

    printf("\n--- Распаковка карты (Вариант 10) ---\n");
    fprintf(out, "--- Распаковка карты (Вариант 10) ---\n");

    while (fgets(line, sizeof(line), in) != NULL) {
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;
        line_num++;

        char unpacked[MAX_LINE_LEN];
        int out_idx = 0, i = 0;

        while (line[i] != '\0' && line[i] != '\n' && line[i] != '\r') {
            int count = 0;
            while (line[i] >= '0' && line[i] <= '9') {
                count = count * 10 + (line[i] - '0');
                i++;
            }
            char symbol = line[i];
            if (count == 0 || symbol == '\0' || symbol == '\n' || symbol == '\r') {
                error = 1;
                break;
            }
            i++;
            for (int k = 0; k < count; k++) {
                unpacked[out_idx] = symbol;
                out_idx++;
            }
        }
        unpacked[out_idx] = '\0';

        if (error) {
            printf("Ошибка формата в строке %d!\n", line_num);
            fprintf(out, "Ошибка формата в строке %d!\n", line_num);
            break;
        }
        if (expected_len == -1) expected_len = out_idx;
        else if (out_idx != expected_len) {
            printf("Ошибка в строке %d: геометрия карты нарушена (длина %d вместо %d)!\n", line_num, out_idx, expected_len);
            fprintf(out, "Ошибка в строке %d: геометрия карты нарушена!\n", line_num);
            error = 1;
            break;
        }
        printf("%s\n", unpacked);
        fprintf(out, "%s\n", unpacked);
    }

    if (!error && line_num > 0) {
        printf("Карта успешно распакована! Ширина строк: %d. Сохранено в output.txt\n", expected_len);
        fprintf(out, "Карта успешно распакована! Ширина строк: %d.\n", expected_len);
    }
    fclose(in);
    fclose(out);
}


// Выделение динамической памяти под двумерную карту
char** create_map(int height, int width) {
    char **map = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        map[i] = (char*)malloc(width * sizeof(char));
    }
    return map;
}

// Освобождение динамической памяти (в обратном порядке)
void free_map(char **map, int height) {
    if (map == NULL) return;
    for (int i = 0; i < height; i++) {
        free(map[i]);
    }
    free(map);
}

// Генерация этажа Башни со сквозняками (Вариант 10)
void generate_tower_floor(char **map, int height, int width, int *hero_y, int *hero_x) {
    char winds[4] = {'^', 'v', '<', '>'};

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                map[y][x] = '#';
            } else {
                int r = rand() % 100;
                if (r < OBSTACLE_CHANCE) {
                    map[y][x] = (rand() % 2 == 0) ? 'T' : 'R';
                } else if (r < OBSTACLE_CHANCE + WIND_CHANCE) {
                    map[y][x] = winds[rand() % 4];
                } else {
                    map[y][x] = '.';
                }
            }
        }
    }

    *hero_y = 1 + rand() % (height - 2);
    *hero_x = 1 + rand() % (width - 2);
    map[*hero_y][*hero_x] = 'D';

    int up_y, up_x;
    do {
        up_y = 1 + rand() % (height - 2);
        up_x = 1 + rand() % (width - 2);
    } while (up_y == *hero_y && up_x == *hero_x);

    map[up_y][up_x] = 'U';
}

// Отрисовка карты на экране
void print_map(char **map, int height, int width, int hero_y, int hero_x, int floor_num) {
    printf("\n=== БАШНЯ: ЭТАЖ %d (Размер %dx%d) ===\n", floor_num, height, width);
    printf("Обозначения: @ - Герой, # - Стена, T/R - Препятствия, U - Вверх, D - Вниз (Выход)\n");
    printf("Сквозняк (ветер): ^ (вверх), v (вниз), < (влево), > (вправо)\n\n");

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == hero_y && x == hero_x) {
                printf("@ ");
            } else {
                printf("%c ", map[y][x]);
            }
        }
        printf("\n");
    }
}

// Проверка проходимости клетки
int is_walkable(char cell) {
    if (cell == '#' || cell == 'T' || cell == 'R') {
        return 0;
    }
    return 1;
}

// Главная функция приключения (с механикой сквозняка и сменой этажей)
void start_adventure() {
    int floor_num = 1;
    int height = MIN_MAP_SIZE + rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1);
    int width = MIN_MAP_SIZE + rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1);
    int hero_y = 1, hero_x = 1;

    char **map = create_map(height, width);
    generate_tower_floor(map, height, width, &hero_y, &hero_x);

    char cmd;
    while (1) {
        print_map(map, height, width, hero_y, hero_x, floor_num);
        printf("\nВы стоите на клетке: [%c]\n", map[hero_y][hero_x]);
        printf("Управление: W (вверх), A (влево), S (вниз), D (вправо), Q (сбежать домой): ");

        if (scanf(" %c", &cmd) != 1) {
            clear_input();
            continue;
        }
        clear_input();

        if (cmd == 'q' || cmd == 'Q') {
            if (map[hero_y][hero_x] == 'D') {
                printf("Вы спустились по лестнице D и успешно вернулись домой на ферму!\n");
                free_map(map, height);
                break;
            } else {
                printf("Нельзя сбежать! Чтобы выйти домой (Q), нужно встать на лестницу вниз (D)!\n");
                continue;
            }
        }

        int dy = 0, dx = 0;
        if (cmd == 'w' || cmd == 'W') dy = -1;
        else if (cmd == 's' || cmd == 'S') dy = 1;
        else if (cmd == 'a' || cmd == 'A') dx = -1;
        else if (cmd == 'd' || cmd == 'D') dx = 1;
        else {
            printf("Неизвестная команда! Используйте клавиши W, A, S, D или Q.\n");
            continue;
        }

        int new_y = hero_y + dy;
        int new_x = hero_x + dx;

        if (new_y < 0 || new_y >= height || new_x < 0 || new_x >= width || !is_walkable(map[new_y][new_x])) {
            printf("Бам! Там стена или препятствие (%c), пройти нельзя!\n", map[new_y][new_x]);
            continue;
        }

        hero_y = new_y;
        hero_x = new_x;

        // Механика Варианта 10: сквозняк
        char current_cell = map[hero_y][hero_x];
        if (current_cell == '^' || current_cell == 'v' || current_cell == '<' || current_cell == '>') {
            int wind_dy = 0, wind_dx = 0;
            if (current_cell == '^') wind_dy = -1;
            else if (current_cell == 'v') wind_dy = 1;
            else if (current_cell == '<') wind_dx = -1;
            else if (current_cell == '>') wind_dx = 1;

            printf("Ух! Вы попали на сквозняк (%c)! Ветер сдувает вас...\n", current_cell);

            for (int step = 0; step < WIND_PUSH; step++) {
                int next_y = hero_y + wind_dy;
                int next_x = hero_x + wind_dx;

                if (next_y >= 0 && next_y < height && next_x >= 0 && next_x < width && is_walkable(map[next_y][next_x])) {
                    hero_y = next_y;
                    hero_x = next_x;
                    if (map[hero_y][hero_x] == 'U') break;
                } else {
                    printf("Ветер ударил вас о препятствие!\n");
                    break;
                }
            }
        }

        // Переход на новый этаж Башни при наступании на U
        if (map[hero_y][hero_x] == 'U') {
            printf("\n>>> Вы нашли лестницу вверх (U)! Поднимаемся на следующий этаж... <<<\n");
            free_map(map, height);

            floor_num++;
            height = MIN_MAP_SIZE + rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1);
            width = MIN_MAP_SIZE + rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1);

            map = create_map(height, width);
            generate_tower_floor(map, height, width, &hero_y, &hero_x);
        }
    }
}

int main() {
    setlocale(LC_ALL, "");
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
    srand((unsigned int)time(NULL));

    int current_day = START_DAY;
    int current_hour = START_HOUR;
    int inventory[INVENTORY_SIZE] = {1, 2, 1, 3, 0, 2, 6, 3, 1, 4};

    char farmer_name[MAX_NAME_LEN];
    char item_names[MAX_ITEMS][MAX_NAME_LEN];

    if (load_items(item_names) == 0) {
        printf("[Предупреждение] Файл items.txt не найден! Включены стандартные названия.\n");
    }

    printf("Введите имя вашего фермера (одним словом): ");
    scanf("%31s", farmer_name);
    clear_input();

    int choice = -1;

    while (1) {
        printf("\n---------------- МЕНЮ (Фермер: %s) ----------------\n", farmer_name);
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Устранить дубликаты (Вариант 10)\n");
        printf("[7] Поиск предмета в рюкзаке по названию\n");
        printf("[8] Записать состояние в дневник (diary.txt)\n");
        printf("[9] Распаковать сжатую карту (Вариант 10)\n");
        printf("[10] Отправиться в приключение (Башня — Вариант 10)\n");
        printf("[0] Выход\n");
        printf("----------------------------------------------------\n");
        printf("Выберите пункт меню: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка! Нужно ввести целое число, а не буквы.\n");
            clear_input();
            continue;
        }
        clear_input();

        if (choice == 0) {
            printf("Выход из игры. До встречи, %s!\n", farmer_name);
            break;
        }

        switch (choice) {
            case 1: printf("Текущее время: День %d, %02d:00\n", current_day, current_hour); break;
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
            case 3:
                printf("\nСодержимое вашего рюкзака:\n");
                print_inventory(inventory, item_names);
                break;
            case 4: {
                int slot_index = 0, item_id = 0;
                printf("Введите номер слота (от 0 до %d): ", INVENTORY_SIZE - 1);
                if (scanf("%d", &slot_index) != 1) { printf("Ошибка!\n"); clear_input(); break; }
                clear_input();
                if (slot_index < 0 || slot_index >= INVENTORY_SIZE) { printf("Ошибка слота!\n"); break; }
                for (int i = MIN_ITEM_ID; i <= MAX_ITEM_ID; i++) printf("%d - %s\n", i, item_names[i]);
                printf("Введите ID предмета (от %d до %d): ", MIN_ITEM_ID, MAX_ITEM_ID);
                if (scanf("%d", &item_id) != 1) { printf("Ошибка!\n"); clear_input(); break; }
                clear_input();
                if (item_id < MIN_ITEM_ID || item_id > MAX_ITEM_ID) { printf("Ошибка ID!\n"); break; }
                inventory[slot_index] = item_id;
                printf("Предмет [%s] успешно положен в слот %d!\n", item_names[item_id], slot_index);
                break;
            }
            case 5: {
                int slot_index = 0;
                printf("Введите номер слота для очистки (от 0 до %d): ", INVENTORY_SIZE - 1);
                if (scanf("%d", &slot_index) != 1) { printf("Ошибка!\n"); clear_input(); break; }
                clear_input();
                if (slot_index < 0 || slot_index >= INVENTORY_SIZE) { printf("Ошибка слота!\n"); break; }
                if (inventory[slot_index] == 0) printf("Слот %d уже пуст.\n", slot_index);
                else {
                    int old_id = inventory[slot_index];
                    inventory[slot_index] = 0;
                    printf("Предмет [%s] выброшен из слота %d.\n", item_names[old_id], slot_index);
                }
                break;
            }
            case 6: {
                printf("\n--- Инвентарь ДО устранения дубликатов ---\n");
                print_inventory(inventory, item_names);
                int removed = remove_duplicates(inventory);
                printf("\n--- Инвентарь ПОСЛЕ устранения дубликатов ---\n");
                print_inventory(inventory, item_names);
                printf("Убрано дубликатов: %d\n", removed);
                break;
            }
            case 7: {
                char search_name[MAX_NAME_LEN];
                printf("Введите название предмета для поиска: ");
                scanf("%31s", search_name);
                clear_input();
                int id = find_item_id(item_names, search_name);
                if (id == -1) printf("Предмета нет в каталоге!\n");
                else {
                    int found = 0;
                    printf("Предмет \"%s\" (ID %d) найден в слотах: ", search_name, id);
                    for (int i = 0; i < INVENTORY_SIZE; i++) {
                        if (inventory[i] == id) { printf("[%d] ", i); found = 1; }
                    }
                    if (!found) printf("нет в рюкзаке (0 шт.).");
                    printf("\n");
                }
                break;
            }
            case 8: write_diary(farmer_name, current_day, current_hour, inventory, item_names); break;
            case 9: unpack_map_task(); break;
            case 10: start_adventure(); break;
            default: printf("Такого пункта нет в меню! Выберите число от 0 до 10.\n"); break;
        }
    }
    return 0;
}