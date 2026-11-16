#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "work_with_ray/ray.h"
#include "work_with_2D_matrix/2D_matrix.h"
#include <time.h>

//АЛАРМ - сделать нормировку
//TODO придумать что-нибудь с материалами (возможно сделать карту материалов)
//по-хорошему, если готовить код к проду, надо бы еще fscanf проверять

#define SLEEP_TIME_MS 75
#define STEP 0.5f
#define AIR_WEAKING 0.99f
#define CONCRETE_WEAKING 0.5f
#define GLASS_WEAKING 0.8f
#define EXP 0.01f

static int raytrace(Ray**, const char*, float**, float*, int, int, double*); 
static int trace_step(Ray*, char*, const char*, float*, int);
static void print_ray_covers(float**, int, Ray**, int);
static bool out_of_map(Ray*, int);
static bool check_and_process_collision(Ray*, float, float, const char*, int);
static void init_power_update(Ray*, char);
static double now_s(void);
static void calculate_full_cover(float**, int, float*, int);

int main(void) {
    FILE* input = fopen("map.txt", "r");

    if (input == NULL) {
        printf("Ошибка чтения файла\n");
        return EXIT_FAILURE;
    }

    int n = 0, rays_count = 0, value_buf, ret_code = EXIT_SUCCESS;
    float x, y, power, angle;
    double algorithm_time = 0.0;
    char symb;
    Ray **rays = NULL;
    char *map = NULL;
    float **rays_covers = NULL;
    float *full_cover = NULL;

    fscanf(input, "%d %d", &n, &rays_count);
    if (rays_count <= 0 || n <= 0) {
        printf("Некорректные данные: размерность карты и количество лучей должны быть > 0\n");
        ret_code = EXIT_FAILURE;
        goto CLEANUP;
    }
    
    rays = calloc(rays_count, sizeof(Ray*));
    if (rays == NULL) {
        printf("Ошибка выделения памяти для лучей\n");
        ret_code = EXIT_FAILURE;
        goto CLEANUP;
    }

    rays_covers = calloc(rays_count, sizeof(float*));
    full_cover = calloc(n * n, sizeof(float));
    if (rays_covers == NULL || full_cover == NULL) {
        printf("Ошибка выделения памяти для карт покрытия\n");
        ret_code = EXIT_FAILURE;
        goto CLEANUP;
    }
    
    for (int i = 0; i < rays_count; i++) {
        fscanf(input, "%f %f %f %f %c", &power, &x, &y,\
            &angle, &symb);

        rays[i] = init_ray(x, y, angle, power, symb);
        if (rays[i] == NULL) {
            printf("Ошибка выделения памяти для луча\n");
            ret_code = EXIT_FAILURE;
            goto CLEANUP;
        }
        rays_covers[i] = calloc(n * n, sizeof(float));
        if (rays_covers[i] == NULL) {
            printf("Ошибка выделения памяти для карт покрытия\n");
            ret_code = EXIT_FAILURE;
            goto CLEANUP;
        }
    }
    
    map = malloc(sizeof(char) * n * n);
    if (map == NULL) {
        printf("Ошибка выделения памяти для карты\n");
        ret_code = EXIT_FAILURE;
        goto CLEANUP;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(input, "%d", &value_buf);
            map[i * n + j] = '0' + (unsigned char) value_buf;
        }
    }

    if (raytrace(rays, map, rays_covers, full_cover, n, rays_count, &algorithm_time) != EXIT_SUCCESS) {
        ret_code = EXIT_FAILURE;
        goto CLEANUP;
    }
    printf("Время, потраченное на расчёты пути луча и карт покрытия: %6.2f мс\n", algorithm_time * 1000.0);
    print_ray_covers(rays_covers, n, rays, rays_count);
    printf("Полная карта покрытия\n");
    print_matrix(full_cover, (size_t) n, (size_t) n, MATRIX_FLOAT);

    CLEANUP:
    fclose(input);
    if (rays_covers) {
        for (int i = 0; i < rays_count; i++) {
            if (rays_covers[i]) free(rays_covers[i]);
        }
        free(rays_covers);
    }
    if (full_cover) free(full_cover);
    if (rays) {
        for (int i = 0; i < rays_count; i++){
            if (rays[i]) free(rays[i]);
        }
        free(rays);
    }
    if (map) free(map);
    return ret_code;
}

/// @brief просчитывает путь луча и карту покрытия
/// @param ray луч
/// @param map карта города
/// @param covers карта покрытия
/// @param full_cover карта покрытия
/// @param n размерность карты
/// @param rays_count количество лучей
/// @param time время, затраченное на вычисления
int raytrace(Ray **rays, const char *map, float **covers, \
        float *full_cover, int n, int rays_count, double *time) {
    bool trace = true;
    char *map_with_rays = malloc(sizeof(char) * n * n);

    if (map_with_rays == NULL) {
        printf("Ошибка обработки данных\n");
        return EXIT_FAILURE;
    }

    while (trace) {
        memcpy(map_with_rays, map, n * n * sizeof(char));
        printf("\033[2J\033[H"); 
        fflush(stdout);
        for (int i = 0; i < rays_count; i++) {
            Ray *ray = rays[i];
            double t0 = now_s();
            int result = trace_step(ray, map_with_rays, map, covers[i], n);
            double t1 = now_s();
            *time += t1 - t0;
            if (result == EXIT_FAILURE || ray->power <= EXP) {
                trace = false;
                continue;
            }
            printf("Ray \'%c\' coordinates (%.2f, %.2f), power: %.12f\n",\
                ray->symb, ray->x, ray->y, ray->power);
        }

        struct timespec ts = {
            .tv_sec  = SLEEP_TIME_MS / 1000,
            .tv_nsec = (SLEEP_TIME_MS % 1000) * 1000000L
        };
        print_matrix(map_with_rays, (size_t) (n), (size_t) (n), MATRIX_CHAR);
        nanosleep(&ts, NULL);
    } 

    double t0 = now_s();
    calculate_full_cover(covers, rays_count, full_cover, n);
    double t1 = now_s();
    *time += t1 - t0;

    free(map_with_rays);
    return EXIT_SUCCESS;
}

/// @brief делает шаг трассировки для конкретного луча
/// @param ray луч
/// @param map_with_rays карта города ( подразумевается измененная, с другими лучами)
/// @param map карта города без изменений
/// @param cover карта покрытия луча
/// @param n размерность карт
/// @return возвращает код возврата EXIT_SUCCESS или EXIT_FAILURE
int trace_step(Ray *ray, char *map_with_rays, const char *map, float *cover, int n) {
    float cx, cy;
    int cx_new, cy_new;
    char material;

    //получим старые координаты
    //из-за отбрасывания вещественной части визуализация не корректна
    cx = ray->x;
    cy = ray->y;

    update_ray_coordinates(ray, STEP);
    cx_new = (int) (ray->x);
    cy_new = (int) (ray->y);

    // Новые координаты за пределами — стен не проверяем
    if (out_of_map(ray, n)) {
        ray->power = 0.0f; 
        return EXIT_FAILURE;
    }

    if (check_and_process_collision(ray, cx, cy, map, n)) {
        material = map[cy_new * n + cx_new];
        init_power_update(ray, material);

        return EXIT_SUCCESS;
    }

    material = map[cy_new * n + cx_new];
    init_power_update(ray, material);
    cover[cy_new * n + cx_new] += ray->power;
    map_with_rays[cy_new * n + cx_new] = ray->symb;

    return EXIT_SUCCESS;
}

/// @brief проверяет, столкнулся ли луч с препятствием
/// @param ray луч со новыми координатам
/// @param x старая x-координата луча
/// @param y старая y-координата луча
/// @param map карта
/// @param n размерность карты
/// @return true - столкнулся, false - не столкнулся
bool check_and_process_collision(Ray *ray, float x, float y, \
        const char *map, int n) {
    int x_new = (int)ray->x;
    int y_new = (int)ray->y;

    if (map[((int)y_new) * n + ((int)x_new)] != '0') {    
        bool wall_vert = (x_new != (int)x) && (map[(int)y * n + x_new] != '0');  // стена по x
        bool wall_horiz = (y_new != (int)y) && (map[y_new * n + ((int)x)] != '0'); // стена по y
        if (x_new != (int)x && y_new != (int)y && !wall_vert && !wall_horiz) {
            wall_vert = wall_horiz = true;
        }

        if (wall_vert) ray->dx = -ray->dx;
        if (wall_horiz) ray->dy = -ray->dy; 

        ray->x = x;
        ray->y = y;

        return wall_horiz || wall_vert;
    }

    return false;
}

/// @brief проверяет поступающий материал, инициирует обращение к ray-методам для 
/// обновления мощности луча
/// @param ray луч
/// @param material полученный материал
void init_power_update(Ray *ray, char material) {
    switch (material)
    {
        case '0':
            update_power(ray, AIR_WEAKING);
            break;
        case '1':
            update_power(ray, CONCRETE_WEAKING);
            break;
        case '2':
            update_power(ray, GLASS_WEAKING);
            break;
    }
}

/// @brief вызывает печать каты покрытия для каждого луча 
/// @param covers массив карт покрытия
/// @param n роазмерность карты покрытия
/// @param rays массив лучей
/// @param rays_count размерность массива лучей и карт покрытия
void print_ray_covers(float **covers, int n, Ray **rays, int rays_count) {
    for (int z = 0; z < rays_count; z++) {
        printf("Cover map for %c ray\n", rays[z]->symb);
        print_matrix(covers[z], (size_t) n, (size_t) n, MATRIX_FLOAT);
    }
}

/// @brief проверка на выход за пределы карты
/// @param ray луч
/// @param n размер города
/// @return вышел - true, не вышел - false
bool out_of_map(Ray *ray, int n) {
    return ray->x < 0.0f || ray->x >= n || ray->y < 0.0f || ray->y >= n;
}

static double now_s(void) {
    struct timespec ts;     
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
} 

/// @brief расчёт общей карты покрытия
/// @param rays_covers карта покрытия каждого луча
/// @param rays_count количество лучей
/// @param full_cover общая карта покрытия
/// @param n размерность карт
static void calculate_full_cover(float **rays_covers, int rays_count, float *full_cover, int n) {
    for (int z = 0; z < rays_count; z++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                full_cover[j * n + i] += rays_covers[z][j * n + i];
            }
        }
    }
}