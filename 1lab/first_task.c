#include <stdio.h>
#include <stdlib.h>
#define NUMBER 555

int main(void) {
	printf("|%d|\n", NUMBER);
	printf("|%7d|\n", NUMBER);
	printf("|%-7d|\n", NUMBER);
	
	int words = 59;
	int letters = 1004;
	int digits = 8;

	double a = 0.1;
	double b = 0.2;
	double float_sum = a + b;
	
	printf("\nWords:%9d\nLetters:%5d\nDigits:%8d\n\n", words, letters, digits);

	printf("a = %.6f; b = %.6f\n", a, b);
	printf("a + b = %.17f\n", float_sum);

	return EXIT_SUCCESS;
}
