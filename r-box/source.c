#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#define SWAP(a, b) { if (a != b) { a = a ^ b; b = a ^ b; a = a ^ b; } }
#define DEBUG 0

// void __attribute__((constructor)) init_matrix(void);
void init(void);
void print_matrix(void);
void u(int, int);
void d(int, int);
void f(int, int);
void b(int, int);
void l(int, int);
void r(int, int);
int correct(void);
void print_flag(void);
void handler(void);

void (*func_table[8])(int, int) = {(void (*)(int, int))0x41414141, u, d, f, b, l, r, (void (*)(int, int))0x41414141};
unsigned char matrix[6][5][5] = { 
	{
		{ 2, 2, 2, 2, 2 }, 
		{ 2, 3, 3, 3, 3 }, 
		{ 2, 3, 0, 0, 0 }, 
		{ 2, 3, 0, 2, 2 }, 
		{ 2, 3, 0, 2, 3 }
	}, 
	{
		{ 5, 5, 5, 5, 5 }, 
		{ 4, 4, 4, 4, 5 }, 
		{ 1, 1, 1, 4, 5 }, 
		{ 5, 5, 1, 4, 5 }, 
		{ 4, 5, 1, 4, 5 }
	}, 
	{
		{ 3, 0, 2, 3, 0 }, 
		{ 3, 0, 2, 3, 3 }, 
		{ 3, 0, 2, 2, 2 }, 
		{ 3, 0, 0, 0, 0 }, 
		{ 3, 3, 3, 3, 3 }
	}, 
	{
		{ 2, 0, 3, 2, 0 }, 
		{ 0, 0, 3, 2, 0 }, 
		{ 3, 3, 3, 2, 0 }, 
		{ 2, 2, 2, 2, 0 }, 
		{ 0, 0, 0, 0, 0 }
	}, 
	{
		{ 1, 1, 1, 1, 1 }, 
		{ 1, 5, 5, 5, 5 }, 
		{ 1, 5, 4, 4, 4 }, 
		{ 1, 5, 4, 1, 1 }, 
		{ 1, 5, 4, 1, 5 }
	}, 
	{
		{ 4, 4, 4, 4, 4 }, 
		{ 1, 1, 1, 1, 4 }, 
		{ 5, 5, 5, 1, 4 }, 
		{ 4, 4, 5, 1, 4 }, 
		{ 1, 4, 5, 1, 4 }
	}
};

int main(void) {
	unsigned char c;

	init();

	printf("welcome to r-box!\n");
	printf("let's go\n");

	// print_matrix();

	while (1) {
		c = (unsigned char)getc(stdin);
		if (c == '\n')
			break;
		func_table[c >> 5]((c >> 2) & 7, c & 3);
	}

	// print_matrix();

	if (correct()) {
		printf("yeah~\n");
		print_flag();
	}
	else {
		printf("no~\n");
	}

	return 0;
}

/*
void __attribute__((constructor)) init_matrix(void) {
							  1   2   3   4   5
							  6   7   8   9  10
							 11  12  13  14  15
							 16  17  18  19  20
							 21  22  23  24  25


	 26  27  28  29  30		 51  52  53  54  55		 76  77  78  79  80		101 102 103 104 105
	 31  32  33  34  35		 56  57  58  59  60		 81  82  83  84  85		106 107 108 109 110
	 36  37  38  39  40		 61  62  63  64  65		 86  87  88  89  90		111 112 113 114 115
	 41  42  43  44  45		 66  67  68  69  70		 91  92  93  94  95		116 117 118 119 120
	 46  47  48  49  50		 71  72  73  74  75		 96  97  98  99 100		121 122 123 124 125


							126 127 128 129 130
							131 132 133 134 135
							136 137 138 139 140
							141 142 143 144 145
							146 147 148 149 150
		
	int cnt = 1;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 5; k++) {
				matrix[i][j][k] = i;
			}
		}
	}
}
*/ 

void init(void) {
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	signal(SIGALRM, (void*)handler);
	alarm(60);
}


void print_matrix(void) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 5; k++) {
				printf("%3d ", matrix[i][j][k]);
			}
			puts("");
		}
		puts("");
	}
}


void u(int layer, int cnt) {
	#if DEBUG
		printf("U : layer=%d cnt=%d\n", layer, cnt);
	#endif

	if ((layer == 0) || (layer > 6)) {
		printf("illegal instruction\n");
		exit(0);
	}

	while (cnt--) {
		if (layer == 1) {
			for (int i = 0; i < 4; i++) { 
				for (int j = 0; j < 4; j++)	
					SWAP(matrix[layer - 1][0][0], matrix[layer - 1][0][j + 1]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer - 1][0][0], matrix[layer - 1][j + 1][4]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer - 1][0][0], matrix[layer - 1][4][3 - j]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer - 1][0][0], matrix[layer - 1][3 - j][0]);
			}
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer - 1][1][1], matrix[layer - 1][1][j + 2]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer - 1][1][1], matrix[layer - 1][j + 2][3]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer - 1][1][1], matrix[layer - 1][3][2 - j]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer - 1][1][1], matrix[layer - 1][2 - j][1]);
			}
		}
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 5; j++)
				SWAP(matrix[1][layer - 1][0], matrix[4 - (i % 4)][layer - 1][4 - j]);
		}
	}
}

void d(int layer, int cnt) {
	#if DEBUG
		printf("D : layer=%d cnt=%d\n", layer, cnt);
	#endif

	if ((layer == 0) || (layer > 6)) {
		printf("illegal instruction\n");
		exit(0);
	}

	while (cnt--) {
		if (layer == 1) {
			for (int i = 0; i < 4; i++) { 
				for (int j = 0; j < 4; j++)
					SWAP(matrix[6 - layer][0][0], matrix[6 - layer][0][j + 1]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[6 - layer][0][0], matrix[6 - layer][j + 1][4]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[6 - layer][0][0], matrix[6 - layer][4][3 - j]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[6 - layer][0][0], matrix[6 - layer][3 - j][0]);
			}
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++)
					SWAP(matrix[6 - layer][1][1], matrix[6 - layer][1][j + 2]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[6 - layer][1][1], matrix[6 - layer][j + 2][3]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[6 - layer][1][1], matrix[6 - layer][3][2 - j]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[6 - layer][1][1], matrix[6 - layer][2 - j][1]);
			}
		}
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 5; j++)
				SWAP(matrix[1][5 - layer][0], matrix[(i % 4) + 1][5 - layer][j]);
		}
	}
}

void f(int layer, int cnt) {
	#if DEBUG
		printf("F : layer=%d cnt=%d\n", layer, cnt);
	#endif

	if ((layer == 0) || (layer > 6)) {
		printf("illegal instruction\n");
		exit(0);
	}

	while (cnt--) {
		if (layer == 1) {
			for (int i = 0; i < 4; i++) { 
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 1][0][0], matrix[layer + 1][0][j + 1]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 1][0][0], matrix[layer + 1][j + 1][4]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 1][0][0], matrix[layer + 1][4][3 - j]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 1][0][0], matrix[layer + 1][3 - j][0]);
			}
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 1][1][1], matrix[layer + 1][1][j + 2]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 1][1][1], matrix[layer + 1][j + 2][3]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 1][1][1], matrix[layer + 1][3][2 - j]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 1][1][1], matrix[layer + 1][2 - j][1]);
			}
		}
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++)
				SWAP(matrix[0][5 - layer][0], matrix[0][5 - layer][j]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[0][5 - layer][0], matrix[3][j][layer - 1]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[0][5 - layer][0], matrix[5][layer - 1][4 - j]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[0][5 - layer][0], matrix[1][4 - j][5 - layer]);
		}
	}
}

void b(int layer, int cnt) {
	#if DEBUG
		printf("B : layer=%d cnt=%d\n", layer, cnt);
	#endif

	if ((layer == 0) || (layer > 6)) {
		printf("illegal instruction\n");
		exit(0);
	}

	while (cnt--) {
		if (layer == 1) {
			for (int i = 0; i < 4; i++) { 
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 3][0][0], matrix[layer + 3][0][j + 1]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 3][0][0], matrix[layer + 3][j + 1][4]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 3][0][0], matrix[layer + 3][4][3 - j]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 3][0][0], matrix[layer + 3][3 - j][0]);
			}
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 3][1][1], matrix[layer + 3][1][j + 2]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 3][1][1], matrix[layer + 3][j + 2][3]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 3][1][1], matrix[layer + 3][3][2 - j]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 3][1][1], matrix[layer + 3][2 - j][1]);
			}
		}
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++)
				SWAP(matrix[0][layer - 1][0], matrix[1][j][layer - 1]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[0][layer - 1][0], matrix[5][5 - layer][j]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[0][layer - 1][0], matrix[3][4 - j][5 - layer]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[0][layer - 1][0], matrix[0][layer - 1][4 - j]);
		}
	}
}

void l(int layer, int cnt) {
	#if DEBUG
		printf("L : layer=%d cnt=%d\n", layer, cnt);
	#endif

	if ((layer == 0) || (layer > 6)) {
		printf("illegal instruction\n");
		exit(0);
	}

	while (cnt--) {
		if (layer == 1) {
			for (int i = 0; i < 4; i++) { 
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer][0][0], matrix[layer][0][j + 1]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer][0][0], matrix[layer][j + 1][4]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer][0][0], matrix[layer][4][3 - j]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer][0][0], matrix[layer][3 - j][0]);
			}
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer][1][1], matrix[layer][1][j + 2]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer][1][1], matrix[layer][j + 2][3]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer][1][1], matrix[layer][3][2 - j]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer][1][1], matrix[layer][2 - j][1]);
			}
		}
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++)
				SWAP(matrix[2][0][layer - 1], matrix[2][j][layer - 1]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[2][0][layer - 1], matrix[5][j][layer - 1]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[2][0][layer - 1], matrix[4][4 - j][5 - layer]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[2][0][layer - 1], matrix[0][j][layer - 1]);
		}
	}
}

void r(int layer, int cnt) {
	#if DEBUG
		printf("R : layer=%d cnt=%d\n", layer, cnt);
	#endif

	if ((layer == 0) || (layer > 6)) {
		printf("illegal instruction\n");
		exit(0);
	}

	while (cnt--) {
		if (layer == 1) {
			for (int i = 0; i < 4; i++) { 
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 2][0][0], matrix[layer + 2][0][j + 1]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 2][0][0], matrix[layer + 2][j + 1][4]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 2][0][0], matrix[layer + 2][4][3 - j]);
				for (int j = 0; j < 4; j++)
					SWAP(matrix[layer + 2][0][0], matrix[layer + 2][3 - j][0]);
			}
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 2][1][1], matrix[layer + 2][1][j + 2]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 2][1][1], matrix[layer + 2][j + 2][3]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 2][1][1], matrix[layer + 2][3][2 - j]);
				for (int j = 0; j < 2; j++)
					SWAP(matrix[layer + 2][1][1], matrix[layer + 2][2 - j][1]);
			}
		}
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++)
				SWAP(matrix[2][0][5 - layer], matrix[0][4 - j][5 - layer]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[2][0][5 - layer], matrix[4][j][layer - 1]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[2][0][5 - layer], matrix[5][4 - j][5 - layer]);
			for (int j = 0; j < 5; j++)
				SWAP(matrix[2][0][5 - layer], matrix[2][4 - j][5 - layer]);
		}
	}
}

int correct(void) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; j < 5; j++) {
				if ((matrix[i][2][2] ^ matrix[i][j][k]) != 0)
					return 0;
			}
		}
	}

	return 1;
}

void print_flag(void) {
	char flag[0x100];
	int fd;

	memset(flag, 0, sizeof(flag));
	fd = open("/home/r-box/flag", O_RDONLY);
	read(fd, flag, sizeof(flag));
	printf("%s", flag);
	close(fd);
}

void handler(void) {
	puts("time is up~");
	exit(0);
}
