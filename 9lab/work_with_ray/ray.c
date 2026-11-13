#include <stdlib.h>
#include <math.h>
#include "ray.h"

/// @brief обновляет координаты луча
/// @param ray луч
/// @param step шаг
void update_ray_coordinates(Ray *ray, float step) {
    ray->x = ray->x + ray->dx * step;
    ray->y = ray->y + ray->dy * step;
}

/// @brief инициализирует память для луча и заполняет данными выделеную память
/// @param x x-координата
/// @param y y-координата
/// @param angle угол запуска (в градусах, переводится в радианы)
/// @param power мощность луча
/// @param symb символ для отображения луча
/// @return указатель на выделенную память или NULL при ошибке выделения
Ray* init_ray(float x, float y, float angle, float power, char symb) {
    Ray *ray = (Ray*)malloc(sizeof(Ray));

    if (ray != NULL) {
        ray->x = x;
        ray->y = y;
        ray->angle = angle * M_PI / 180;
        ray->power = power;
        ray->dx = cosf(ray->angle);
        ray->dy = sinf(ray->angle);
        ray->symb = symb;
    }

    return ray;
}

/// @brief обновляет мощность луча
/// @param ray луч
/// @param multiplier множитель
void update_power(Ray *ray, float multiplier) {
    ray->power *= multiplier;
}