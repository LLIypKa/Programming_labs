#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//TODO масштабировать до нескольких лучей
//TODO раскидать по файлам ray.c и matrix.c
//TODO придумать что-нибудь с материалами (возможно сделатькарту материалов)

#define STEP 0.5f
#define AIR_WEAKING 0.99f
#define CONCRETE_WEAKING 0.5f
#define GLASS_WEAKING 0.8f

/// @brief хранит float-координаты (x;y), float angle, power и float-шаги по осям (dx, dy)
typedef struct ray {
    float x, y;
    float angle, power;
    float dx, dy;
    char symb;
} Ray;

void raytrace(Ray*, int**, float**, int*); 
void print_map(int**, int*, Ray*);
void update_ray_coordinates(Ray *ray, float step);
void print_cover(float**, int*, Ray*);
void update_power(Ray*, int);

int main(void) {
    FILE* input = fopen("map.txt", "r");

    if (input == NULL) {
        printf("Ошибка чтения файла\n");
        return EXIT_FAILURE;
    }

    int n, rays_count;
    Ray *ray;
    int **map;
    float **cover;
    ray = (Ray*)malloc(sizeof(Ray));
    fscanf(input, "%d %d", &n, &rays_count);
    fscanf(input, "%f %f %f %f %c", &ray->power, &ray->x, &ray->y, &ray->angle, &ray->symb);
    map = (int**)malloc(sizeof(int*) * n);
    cover = (float**)malloc(sizeof(float*) * n);
    for (int i = 0; i < n; i++) {
        map[i] = (int*)malloc(sizeof(int) * n);
        cover[i] = (float*)malloc(sizeof(float) * n);
        for (int j = 0; j < n; j++) {
            fscanf(input, "%d", &map[i][j]);
            cover[i][j] = 0;
        }
    }

    ray->angle = ray->angle * M_PI / 180;
    ray->dx = cos(ray->angle);
    ray->dy = sin(ray->angle);

    raytrace(ray, map, cover, &n);
    print_cover(cover, &n, ray);

    fclose(input);
    for (int i = 0; i < n; i++) {
        free(map[i]);
        free(cover[i]);
    }
    free(map);
    free(cover);
    free(ray);
    return EXIT_SUCCESS;
}

/// @brief просчитывает путь луча и карту покрытия
/// @param ray луч
/// @param map карта города
/// @param cover карта покрытия
/// @param n размерность карты
void raytrace(Ray *ray, int **map, float **cover,  int *n) {
    int cx, cy;
    bool is_need_to_update = false;
    int material;

    while (fabs(ray->power) > 1e-12) {
        //получим старые координаты для массива cover и заполним по ним данные
        //из-за отбрасывания вещественной части визуализация не корректна
        cx = (int) ray->x;
        cy = (int) ray->y;
        material = map[cx][cy];
        update_power(ray, material);
        cover[cx][cy] += ray->power;
        
        is_need_to_update = false;
        print_map(map, n, ray);
        usleep(200000);
        fflush(stdout);

        update_ray_coordinates(ray, STEP);
        
        int cx_new = (int) (ray->x);
        int cy_new = (int) (ray->y);

        //при разделении на библиотеки лучше бы тоже пихнуть в работу с лучом
        if (map[cx_new][cy_new] != 0) {    
            bool wall_vert = (cx_new != cx) && (map[cx_new][cy] != 0);  // стена по x
            bool wall_horiz = (cy_new != cy) && (map[cx][cy_new] != 0); // стена по y

            if (wall_vert)  { ray->dx = -ray->dx; is_need_to_update = true; }
            if (wall_horiz) { ray->dy = -ray->dy; is_need_to_update = true; }
        }

        if (is_need_to_update) {
            update_ray_coordinates(ray, STEP);
            material = map[cx_new][cy_new];
            update_power(ray, material);
        }
    } 
}

void print_map(int **map, int *n, Ray *ray) {
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            if (i == (int)ray->x && j == (int)ray->y) {
                printf(" %c ", ray->symb);
            }
            else {
                printf(" %d ", map[i][j]);
            }
        }
        printf("\n");
    }

    printf("Ray coordinates (%.2f, %.2f), power: %.12f\n", ray->x, ray->y, ray->power);
}

/// @brief обновляет координаты луча
/// @param ray луч
/// @param step шаг
void update_ray_coordinates(Ray *ray, float step) {
    ray->x = ray->x + ray->dx * step;
    ray->y = ray->y + ray->dy * step;
}

void print_cover(float **cover, int *n, Ray *ray) {
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {    
            printf(" %.12f ", cover[i][j]);
        }
        printf("\n");
    }
}

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