#include <stdlib.h>
#include <stdio.h>

typedef struct ray {
    int x, y;
    float angle, power;
    float dx, dy;
} Ray;

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
    fscanf(input, "%f %d %d %f", &ray->power, &ray->x, &ray->y, &ray->angle);
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