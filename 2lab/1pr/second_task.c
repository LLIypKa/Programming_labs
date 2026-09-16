#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	float degrees = 0.0d;
	char scale;

	printf("Введите температуру, указывая шкалу после числа (например:\n1) 36.6c (36.6C) - 36.6 грудусов по цельсию\n2) 36.6f (36.6F) - 36.6 по фаренгейту)\n");
	scanf("%f%c", &degrees, &scale);

	if (scale == 'c' || scale == 'C') {
		degrees = (degrees * 1.8) + 32;
		scale = 'f';
	}
	else {
		degrees = (degrees - 32) / 1.8;
		scale = 'c';
	}

	printf("Выходная температура: %f%c\n", degrees, scale);

	return EXIT_SUCCESS;
}
