//
// Created by 15533 on 2021/11/2.
//

#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "chunk.h"
#include "round.h"

struct BOARD board;

int main() {

    // 初始化棋盘
    init_board(&board);

    printf("Put the initial pattern in input.txt\n");
    printf("Old results will be deleted if you continue. Beware!\n");
    printf("Enter calculation times (0 means infinity):");
    long long rounds = 0;
    scanf("%lld", &rounds);

    // 读取输入文件
    long long x = 0, y = 0;
    FILE *in_file = fopen("input.txt", "r");

    if (in_file == NULL) {
        printf("Failed to open file input.txt\n");
        system("pause");
        return -1;
    }

    int c;
    while ((c = fgetc(in_file)) != EOF) {
        switch (c) {
            case '1':
                init_cell_in_board(&board, x++, y);
                break;
            case '\r':
                break;
            case '0':
                x++;
                break;
            case '\n':
                x = 0;
                y++;
                break;
            default:
                printf("Illegal input.\n");
                system("pause");
                return -1;
        }
    }

    printf("Enter file save mode(0.full 1.last 100 rounds):");
    int save_mode = 1;
    scanf("%d", &save_mode);

    // 打开输出文件
//    int f_index = 0;
    FILE *output_file = NULL;
    system("del round*-*.txt");

    while (--rounds != 0) {

        if (board.round % 100 == 1) {
            fclose(output_file);
            char filename[50] = {};
            sprintf(filename, "round%lld-%lld.txt", board.round, board.round + 99);
            output_file = fopen(filename, "w");

            if (save_mode) {
                sprintf(filename, "round%lld-%lld.txt", board.round - 200, board.round - 101);
                remove(filename);
            }
        }

        char *tmp;
        print_board_to_str(&board, &tmp);

        fprintf(output_file, "Round %lld\n", board.round);
        fprintf(output_file, "%s", tmp);
        free(tmp);

        printf(">>>>> Round %lld\n", board.round);
        next_round(&board);
        printf("[Stats] chunk %lld, cell %lld\n", board.chunk_num, board.total_cell_num);
    }

    fclose(output_file);
    system("pause");
    return 0;
}
