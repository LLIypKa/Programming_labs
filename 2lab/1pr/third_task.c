#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
	float x;
	float y;

	printf("Введите x и y, чтобы проверить лежат ли они между окружностями (x - 0)^2 +(y - 0)^2 = 1^2 и (x - 0)^2 + (y - 0)^2 = 2^2\n");
	scanf("%f %f", &x, &y);

	if ((pow(x, 2) + pow(y, 2) >= 1) && (pow(x, 2) + pow(y, 2) <= 4)) {
		printf("Точка (%f, %f) принадлежит данной области\n", x, y);
	}
	else {
		printf("Точка (%f, %f) не принадлежит данной области\n", x, y);
	}

	return EXIT_SUCCESS;
}
