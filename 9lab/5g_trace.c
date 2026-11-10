#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>

#define STEP 0.5
#define AIR_WEAKING 0.99f
#define CONCRETE_WEAKING 0.5f
#define GLASS_WEAKING 0.8f

typedef struct ray {
    float x, y;
    float angle, power;
    float dx, dy;
    char symb;
} Ray;

//работа с картами и лучом, либо разделять, либо тут оставлять

void raytrace(Ray*, int**, float**, int*); 

// вывод карты

void print_map(int**, int*, Ray*);

// работа с лучом

void update_ray_coordinates(Ray*);

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

    ray->dx = cos(ray->angle);
    ray->dy = sin(ray->angle);

    raytrace(ray, map, cover, &n);

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
void raytrace(Ray *ray, int **map, float **cover, int *n) {
    float old_x, old_y;
    int cx, cy;
    bool is_need_to_update = false;

    while (fabs(ray->power) > 1e-12) {
        is_need_to_update = false;
        print_map(map, n, ray);
        usleep(200000);
        fflush(stdout);
        
        cx = (int) ray->x;
        cy = (int) ray->y;

        update_ray_coordinates(ray);
        
        int cx_new = (int) (ray->x + ray->dx * STEP);
        int cy_new = (int) (ray->y + ray->dy * STEP);

        //при разделении на библиотеки лучше бы тоже пихнуть в работу с лучом
        if (map[cx_new][cy_new] != 0) {    
            if (cx_new != cx) {
                ray->dx = -ray->dx;
                is_need_to_update = true;
            }
            if (cy_new != cy) {
                ray->dy = -ray->dy;
                is_need_to_update = true;
            }
        }

        if (is_need_to_update) {
            update_ray_coordinates(ray);
            int cx_new = (int) (ray->x + ray->dx * STEP);
            int cy_new = (int) (ray->y + ray->dy * STEP);
        }

        ray->power *= CONCRETE_WEAKING;
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

    printf("Ray coordinates (%.2f, %.2f)\n", ray->x, ray->y);
}

/// @brief обновляет координаты луча
/// @param ray луч
void update_ray_coordinates(Ray *ray) {
    ray->x = ray->x + ray->dx * STEP;
    ray->y = ray->y + ray->dy * STEP;
}