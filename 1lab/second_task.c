#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void) {
	int max = INT_MAX;
	max += 2;
	
	printf("max = %d\n", max);

	unsigned int a = 0;
	a -= 2;

	printf("a = %u\n", a);

	char min = CHAR_MIN;
	min -= 1;

	printf("min = %hhd\n", min);

	return EXIT_SUCCESS;
}
