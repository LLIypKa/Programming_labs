#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "work_with_ray/ray.h"
#include "work_with_2D_matrix/2D_matrix.h"

//TODO масштабировать до нескольких лучей (осталось сделать вывод и пофиксить расчёт координат)
//TODO придумать что-нибудь с материалами (возможно сделатькарту материалов)
//TODO исправить накопление силы в стене (относится к фиксу для расчёта координат)

#define STEP 0.5f
#define AIR_WEAKING 0.99f
#define CONCRETE_WEAKING 0.5f
#define GLASS_WEAKING 0.8f

void raytrace(Ray**, const char*, float**, int, int); 
int trace_step(Ray*, char*, const char*, float*, int);
void print_covers(float**, int, Ray**, int);
bool out_of_map(int, int, int);

int main(void) {
    FILE* input = fopen("map.txt", "r");

    if (input == NULL) {
        printf("Ошибка чтения файла\n");
        return EXIT_FAILURE;
    }

    int n, rays_count, value_buf;
    float x, y, power, angle;
    char symb;
    Ray **rays;
    char *map;
    float **covers;

    fscanf(input, "%d %d", &n, &rays_count);
    
    rays = (Ray**)malloc(sizeof(Ray*) * rays_count);
    covers = (float**)malloc(sizeof(float*) * rays_count);
    
    for (int i = 0; i < rays_count; i++) {
        fscanf(input, "%f %f %f %f %c", &power, &x, &y,\
            &angle, &symb);

        rays[i] = init_ray(x, y, angle, power, symb);
        covers[i] = (float*)calloc(n * n, sizeof(float));
    }
    
    map = (char*)malloc(sizeof(char) * n * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(input, "%d", &value_buf);
            map[i * n + j] = '0' + (unsigned char) value_buf;
        }
    }

    raytrace(rays, map, covers, n, rays_count);
    print_covers(covers, n, rays, rays_count);

    fclose(input);
    for (int i = 0; i < rays_count; i++) {
        free(covers[i]);
    }
    free(map);
    free(covers);
    for (int i = 0; i < rays_count; i++){
        free(rays[i]);
    }
    free(rays);
    return EXIT_SUCCESS;
}

/// @brief просчитывает путь луча и карту покрытия
/// @param ray луч
/// @param map карта города
/// @param covers карта покрытия
/// @param n размерность карты
/// @param rays_count количество лучей
void raytrace(Ray **rays, const char *map, float **covers, int n, int rays_count) {
    bool trace = true;
    char *map_with_rays = (char*)malloc(sizeof(char) * n * n);

    while (trace) {
        memcpy(map_with_rays, map, n * n * sizeof(char));
        for (int i = 0; i < rays_count; i++) {
            Ray *ray = rays[i];
            trace_step(ray, map_with_rays, map, covers[i], n);
            printf("Ray \'%c\' coordinates (%.2f, %.2f), power: %.12f\n",\
                ray->symb, ray->x, ray->y, ray->power);
            if (fabs(ray->power) <= 1e-12) {
                trace = false;
            }
        }

        print_matrix(map_with_rays, (size_t) (n), (size_t) (n), MATRIX_CHAR);
        usleep(200000);
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
/// @return возвращает код материала или -1 (если выполнилась безуспешно)
int trace_step(Ray *ray, char *map_with_rays, const char *map, float *cover, int n) {
    int cx, cy, cx_new, cy_new;
    char material;
    bool is_need_to_update = false;

    //получим старые координаты
    //из-за отбрасывания вещественной части визуализация не корректна
    cx = (int) ray->x;
    cy = (int) ray->y;

    if (out_of_map(cx, cy, n)) {
        ray->power = 0.0f;
        return -1;
    }

    update_ray_coordinates(ray, STEP);
    cx_new = (int) (ray->x);
    cy_new = (int) (ray->y);

    // Новые координаты за пределами — стен не проверяем
    if (out_of_map(cx_new, cy_new, n)) {
        ray->power = 0.0f; 
        return -1;
    }

    //при разделении на библиотеки лучше бы тоже пихнуть в работу с лучом
    if (map[cx_new * n + cy_new] != '0') {    
        bool wall_vert = (cx_new != cx) && (map[cx_new * n + cy] != '0');  // стена по x
        bool wall_horiz = (cy_new != cy) && (map[cx * n + cy_new] != '0'); // стена по y

        if (wall_vert)  { ray->dx = -ray->dx; is_need_to_update = true; }
        if (wall_horiz) { ray->dy = -ray->dy; is_need_to_update = true; }
    }

    if (is_need_to_update) {
        update_ray_coordinates(ray, STEP);
        cx_new = (int) (ray->x);
        cy_new = (int) (ray->y);

        if (out_of_map(cx_new, cy_new, n)) {
            ray->power = 0.0f; 
            return -1;
        }
    }

    material = map[cx_new * n + cy_new];
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
    cover[cx_new * n + cy_new] += ray->power;
    map_with_rays[cx_new * n + cy_new] = ray->symb;

    return material;
}

void print_covers(float **covers, int n, Ray **rays, int rays_count) {
    for (int z = 0; z < rays_count; z++) {
        printf("Cover map for %c ray\n", rays[z]->symb);
        print_matrix(covers[z], (size_t) n, (size_t) n, MATRIX_FLOAT);
    }
}

/// @brief проверка на выход за пределы карты
/// @param x координата
/// @param y координата
/// @param n размер города
/// @return вышел - true, не вышел - false
bool out_of_map(int x, int y, int n) {
    return x < 0 || x >= n || y < 0 || y >= n;
}