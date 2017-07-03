#include <sys/types.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>

#include "m-box.h"

void init(void);
void mix_matrix(void);
int correct(void);
void print_flag(void);
void handler(void);

unsigned char input_matrix[N][N];
double inverse_matrix[N][N];

int main(void) {
    init();

    printf("welcome to m-box!\n");
    printf("let's go\n");

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            input_matrix[i][j] = (unsigned char)getc(stdin);

    getc(stdin);

    mix_matrix();

    if (correct()) {
        printf("yeah~\n");
        print_flag();
    }
    else
        printf("no~\n");

    return 0;
}

void init(void) {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    signal(SIGALRM, (void*)handler);
    alarm(60);
}

void mix_matrix(void) {
    double tmp_inv_matrix[N][N];

    srand(60516051 + ptrace(0, 0, 1, 0));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            input_matrix[i][j] ^= (rand() & 0xff);
    }

    // round
    for (int i = 0; i < 9; i++) {
       // add matrix
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                input_matrix[index_matrix[i][j][k] / N][index_matrix[i][j][k] % N] += add_matrix[i][k][j];
            }
        }

        // swap column
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N / 2; k++) {
                int tmp = input_matrix[j][k];
                input_matrix[j][k] = input_matrix[N - j - 1][N - k - 1];
                input_matrix[N - j - 1][N - k - 1] = tmp;
            }
        }

        // sub matrix
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                input_matrix[index_matrix[i][j][k] / N][index_matrix[i][j][k] % N] -= sub_matrix[i][k][j];
            }
        }

        // swap row
        for (int j = 0; j < N / 2; j++) {
            for (int k = 0; k < N; k++) {
                int tmp = input_matrix[j][k];
                input_matrix[j][k] = input_matrix[N - j - 1][N - k - 1];
                input_matrix[N - j - 1][N - k - 1] = tmp;
            }
        }

        // xor matrix
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                input_matrix[index_matrix[i][j][k] / N][index_matrix[i][j][k] % N] ^= xor_matrix[i][index_matrix[i][j][k] / N][index_matrix[i][j][k] % N];
            }
        }
    }

    // check some conditions
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (input_matrix[i][j] < ' ' || input_matrix[i][j] > '~') {
                printf("no~\n");
                exit(0);
            }
        }
    }

    if (
        input_matrix[0][0] != '5' ||
        input_matrix[1][1] != 'u' ||
        input_matrix[2][2] != 'n' ||
        input_matrix[3][3] != 'K' ||
        input_matrix[4][4] != 'n' ||
        input_matrix[5][5] != '0' ||
        input_matrix[6][6] != 'w' ||
        input_matrix[7][7] != 'n'
        ) {
        printf("no~\n");
        exit(0);
    }

    // calculate inverse matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++){
            tmp_inv_matrix[i][j] = input_matrix[i][j];
            inverse_matrix[i][j] = (i == j) ? 1 : 0;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                tmp_inv_matrix[i][k] += tmp_inv_matrix[j][k];
                inverse_matrix[i][k] += inverse_matrix[j][k];
            }
            break;
        }
    }

    for (int i = 0; i < N; i++) {
        double c = tmp_inv_matrix[i][i];
        for (int j = 0; j < N; j++) {
            tmp_inv_matrix[i][j] /= c;
            inverse_matrix[i][j] /= c;
        }

        for (int j = 0; j < N; j++) {
            if (i == j)
                continue;
            if (tmp_inv_matrix[j][i] == 0)
                continue;

            c = tmp_inv_matrix[j][i];
            for (int k = 0; k < N; k++) {
                tmp_inv_matrix[j][k] = tmp_inv_matrix[j][k] - tmp_inv_matrix[i][k] * c;
                inverse_matrix[j][k] = inverse_matrix[j][k] - inverse_matrix[i][k] * c;
            }
        }
    }

    // for optimize, like 0.99999999... --> 1
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (inverse_matrix[i][j] >= 0)
                inverse_matrix[i][j] += 0.000001;
            else
                inverse_matrix[i][j] -= 0.000001;
        }
    }
}

int correct(void) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // determinant should be 1
            if (fabs(inverse_matrix[i][j] - (int)inverse_matrix[i][j]) > 0.00001)
                return 0;
        }
    }

    return 1;
}

void print_flag(void) {
    char flag[0x100];
    int fd;

    memset(flag, 0, sizeof(flag));
    fd = open("/home/m-box/flag", O_RDONLY);
    read(fd, flag, sizeof(flag));
    printf("%s", flag);
    close(fd);
}

void handler(void) {
    puts("time is up~");
    exit(0);
}
