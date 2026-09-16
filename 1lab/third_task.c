#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char ch = 63;
	unsigned char uch = 'r';
	short j = 'b', k = 98;
	
	printf("%c == %d\n", ch, ch);
	printf("%c == %d\n", uch, uch);
	printf("%c == %d\n", j, k);

	return EXIT_SUCCESS;
}
