#ifndef RAY_H
#define RAY_H

/// @brief хранит float-координаты (x;y), float angle, power и float-шаги по осям (dx, dy)
typedef struct ray {
    float x, y;
    float angle, power;
    float dx, dy;
    char symb;
} Ray;

/// @brief обновляет координаты луча
/// @param ray луч
/// @param step шаг
void update_ray_coordinates(Ray *ray, float step);

#endif /*RAY_H*/