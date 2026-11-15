#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "work_with_ray/ray.h"
#include "work_with_2D_matrix/2D_matrix.h"

//TODO придумать что-нибудь с материалами (возможно сделать карту материалов)

#define STEP 0.5f
#define AIR_WEAKING 0.99f
#define CONCRETE_WEAKING 0.5f
#define GLASS_WEAKING 0.8f
#define EXP 0.01f

void raytrace(Ray**, const char*, float**, int, int); 
int trace_step(Ray*, char*, const char*, float*, int);
void print_covers(float**, int, Ray**, int);
bool out_of_map(Ray*, int);
bool check_and_process_collision(Ray*, float, float, const char*, int);
void init_power_update(Ray*, char);

int main(void) {
    FILE* input = fopen("map.txt", "r");

    if (input == NULL) {
        printf("Ошибка чтения файла\n");
        return EXIT_FAILURE;
    }

    int n = 0, rays_count = 0, value_buf, ret_code = EXIT_SUCCESS;
    float x, y, power, angle;
    char symb;
    Ray **rays = NULL;
    char *map = NULL;
    float **covers = NULL;

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

    covers = calloc(rays_count, sizeof(float*));
    if (covers == NULL) {
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
        covers[i] = calloc(n * n, sizeof(float));
        if (covers[i] == NULL) {
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

    raytrace(rays, map, covers, n, rays_count);
    print_covers(covers, n, rays, rays_count);

    CLEANUP:
    fclose(input);
    if (covers) {
        for (int i = 0; i < rays_count; i++) {
            if (covers[i]) free(covers[i]);
        }
        free(covers);
    }
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
/// @param n размерность карты
/// @param rays_count количество лучей
void raytrace(Ray **rays, const char *map, float **covers, int n, int rays_count) {
    bool trace = true;
    char *map_with_rays = malloc(sizeof(char) * n * n);

    if (map_with_rays == NULL) {
        printf("Ошибка обработки данных");
        return;
    }

    while (trace) {
        memcpy(map_with_rays, map, n * n * sizeof(char));
        for (int i = 0; i < rays_count; i++) {
            Ray *ray = rays[i];
            int result = trace_step(ray, map_with_rays, map, covers[i], n);
            if (result == EXIT_FAILURE || ray->power <= EXP) {
                trace = false;
                continue;
            }
            printf("Ray \'%c\' coordinates (%.2f, %.2f), power: %.12f\n",\
                ray->symb, ray->x, ray->y, ray->power);
        }

        print_matrix(map_with_rays, (size_t) (n), (size_t) (n), MATRIX_CHAR);
        usleep(20000);
        fflush(stdout);
    } 

    free(map_with_rays);
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
        material = map[cx_new * n + cy_new];
        init_power_update(ray, material);

        return EXIT_SUCCESS;
    }

    material = map[cx_new * n + cy_new];
    init_power_update(ray, material);
    cover[cx_new * n + cy_new] += ray->power;
    map_with_rays[cx_new * n + cy_new] = ray->symb;

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

    if (map[((int)x_new) * n + ((int)y_new)] != '0') {    
        bool wall_vert = (x_new != (int)x) && (map[x_new * n + (int)y] != '0');  // стена по x
        bool wall_horiz = (y_new != (int)y) && (map[((int)x) * n + y_new] != '0'); // стена по y

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
void print_covers(float **covers, int n, Ray **rays, int rays_count) {
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