#include "ray.h"

void update_ray_coordinates(Ray *ray, float step) {
    ray->x = ray->x + ray->dx * step;
    ray->y = ray->y + ray->dy * step;
}