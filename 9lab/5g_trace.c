#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "work_with_ray/ray.h"
#include "work_with_2D_matrix/2D_matrix.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//TODO масштабировать до нескольких лучей
//TODO раскидать по файлам ray.c и matrix.c (при раскидывании сделать общие функции через void*, enum и размерности)
//TODO придумать что-нибудь с материалами (возможно сделатькарту материалов)
//TODO исправить накопление силы в стене

#define STEP 0.5f
#define AIR_WEAKING 0.99f
#define CONCRETE_WEAKING 0.5f
#define GLASS_WEAKING 0.8f

void raytrace(Ray**, int*, float**, int, int); 
int trace_step(Ray*, int*, float*, int);
void print_cover(float*, int*, Ray*);
void update_power(Ray*, int);
bool out_of_map(int, int, int);

int main(void) {
    FILE* input = fopen("map.txt", "r");

    if (input == NULL) {
        printf("Ошибка чтения файла\n");
        return EXIT_FAILURE;
    }

    int n, rays_count;
    float x, y, power, angle, dx, dy;
    char symb;
    Ray **rays;
    int *map;
    float **covers;

    fscanf(input, "%d %d", &n, &rays_count);
    
    rays = (Ray**)malloc(sizeof(Ray*) * rays_count);
    covers = (float**)malloc(sizeof(float*) * rays_count);
    
    for (int i = 0; i < rays_count; i++) {
        fscanf(input, "%f %f %f %f %c", &power, &x, &y,\
            &angle, &symb);

        rays[i] = init_ray(&x, &y, &angle, &power, &symb);
        covers[i] = (float*)calloc(n * n, sizeof(float));
    }
    
    map = (int*)malloc(sizeof(int) * n * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(input, "%d", &map[i * n + j]);
        }
    }

    raytrace(rays, map, covers, n, rays_count);
    //print_cover(covers[0], &n, rays[0]);
    //print_matrix(covers[0], n, n, FLOAT);

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
void raytrace(Ray **rays, int *map, float **covers, int n, int rays_count) {
    bool trace = true;
    int *map_with_rays = (int*)malloc(sizeof(int) * n * n);

    while (trace) {
        memcpy(map_with_rays, map, n * n * sizeof(int));
        for (int i = 0; i < rays_count; i++) {
            Ray *ray = rays[i];
            trace_step(ray, map_with_rays, covers[i], n);
            printf("Ray \'%c\' coordinates (%.2f, %.2f), power: %.12f\n",\
                ray->symb, ray->x, ray->y, ray->power);
            if (fabs(ray->power) <= 1e-12) {
                trace = false;
            }
        }

        print_matrix(map_with_rays, (size_t) (n), (size_t) (n), INT);
        usleep(200000);
        fflush(stdout);
    } 
}

/// @brief делает шаг трассировки для конкретного луча
/// @param ray луч
/// @param map_with_rays карта города ( подразумевается измененная, с другими лучами)
/// @param cover карта покрытия луча
/// @param n размерность карт
/// @return возвращает код материала или -1 (если выполнилась безуспешно) 
int trace_step(Ray *ray, int *map, float *cover, int n) {
    int cx, cy, cx_new, cy_new, material;
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
    if (map[cx_new * n + cy_new] != 0) {    
        bool wall_vert = (cx_new != cx) && (map[cx_new * n + cy] != 0);  // стена по x
        bool wall_horiz = (cy_new != cy) && (map[cx * n + cy_new] != 0); // стена по y

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
    update_power(ray, material);
    cover[cx_new * n + cy_new] += ray->power;

    return material;
}

void print_cover(float *cover, int *n, Ray *ray) {
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {    
            printf(" %.12f ", cover[i * *n + j]);
        }
        printf("\n");
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

//после проработки материалов перенести в ray.h/ray.c
void update_power(Ray *ray, int material) {
    switch (material)
    {
        case 0:
            ray->power *= AIR_WEAKING;
            break;
        case 1:
            ray->power *= CONCRETE_WEAKING;
            break;
        case 2:
            ray->power *= GLASS_WEAKING;
            break;
    }
}