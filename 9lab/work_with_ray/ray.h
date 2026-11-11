#ifndef RAY_H
#define RAY_H

/// @brief хранит float-координаты (x;y), float angle, power и float-шаги по осям (dx, dy)
typedef struct ray {
    float x, y;
    float angle, power;
    float dx, dy;
    char symb;
} Ray;

void update_ray_coordinates(Ray *ray, float step);
Ray* init_ray(float *x, float *y, float *angle, float *power, char *symb);

#endif /*RAY_H*/