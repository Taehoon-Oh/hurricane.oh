#include <stdio.h>
#include <time.h>

extern void jcalc(char * r, char * a, char * b);

char jnum_a[33000];
char jnum_b[33000];
char jnum_r[33000];

static unsigned int random(void) {
	static unsigned long long seed = 5;
	return (unsigned int)((seed = seed * 25214903917ULL + 11ULL) >> 16);
}

void makeBuf(char * buf) {

	unsigned int b = -1;
	while (++b < 15000) {
		unsigned int i = random() % 32;
		if (i < 10) i = i + '0';
		else i = i + 'A' - 10;
		buf[b] = i;
	}
	buf[b] = 0;
	if (buf[0] == '0') makeBuf(buf);
}

int main() {

	clock_t begin = clock();
	for (int tc = 0; tc < 10; tc++) {
		makeBuf(jnum_a);
		makeBuf(jnum_b);

		jcalc(jnum_r, jnum_a, jnum_b);
	}
	clock_t time = ((clock() - begin) / (CLOCKS_PER_SEC / 1000));
	printf("time=%d\n", time);
	return 0;
}