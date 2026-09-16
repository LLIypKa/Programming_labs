#define _CRT_SECURE_NO_WARNINGS
#define CONDITION_COMPLETE "YES"
#define CONDITION_IS_NOT_COMPLETE "NO"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
	int year = 0;

	printf("Введите год: ");
	scanf("%d", &year);

	if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) {
		printf("\n%s\n", CONDITION_COMPLETE);
	}
	else {
		printf("\n%s\n", CONDITION_IS_NOT_COMPLETE);
	}

	return EXIT_SUCCESS;
}

