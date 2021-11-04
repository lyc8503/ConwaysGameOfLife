//
// Created by 15533 on 2021/11/2.
//

#include <memory.h>
#include "board.h"
#include "chunk.h"
#include <stdio.h>
#include <malloc.h>

struct CHUNK *chunk0;

// 初始化棋盘
void init_board(struct BOARD *board) {

    chunk0 = (struct CHUNK*) malloc(sizeof(struct CHUNK));
    memset(chunk0, 0, sizeof(struct CHUNK));
    chunk0->x = 0;
    chunk0->y = 0;
    chunk0->cell_num = 0;
    chunk0->recycle_counter = MAX_EMPTY_ROUND;

    board->total_cell_num = 0;
    board->init_chunk = chunk0;
    board->round = 0;
    board->chunk_num = 1;
}



// 以"可视化"的方式, 输出棋盘
void print_board_to_str(struct BOARD *board, char **target) {

    // 遍历棋盘 确定矩形范围
    int min_x = 2147483647, min_y = 2147483647, max_x = -2147483648, max_y = -2147483648;

    struct CHUNK *chunk = board->init_chunk;
    while (chunk != NULL) {

        min_x = (chunk->x < min_x) ? chunk->x : min_x;
        min_y = (chunk->y < min_y) ? chunk->y : min_y;
        max_x = (chunk->x > max_x) ? chunk->x : max_x;
        max_y = (chunk->y > max_y) ? chunk->y : max_y;

        chunk = chunk->next;
    }

    // 计算出对应的长度
    int x_len = CHUNK_SIZE * (max_x - min_x + 1) * 3;
    int y_len = CHUNK_SIZE * (max_y - min_y + 1);
    unsigned long long size = ((x_len + 1) * y_len + 1) * sizeof(char);

    // 分配空间
    *target = (char*) malloc(size);
    memset(*target, ' ', size);
    (*target)[size - 1] = 0;

    // 换行
    for (int i = 1; i <= y_len; i++) {
        (*target)[(x_len + 1) * i - 1] = '\n';
    }

    // 遍历并填充
    struct CHUNK *chunk1 = board->init_chunk;
    while (chunk1 != NULL) {

        int chunk_dx = chunk1->x - min_x;
        int chunk_dy = chunk1->y - min_y;

        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                if (get_cell(chunk1->data, x, y)) {
                    (*target)[(chunk_dx * CHUNK_SIZE + x + 1) * 3 - 2 + (chunk_dy * CHUNK_SIZE + y) * (x_len + 1)] = '@';
                } else {
                    (*target)[(chunk_dx * CHUNK_SIZE + x + 1) * 3 - 2 + (chunk_dy * CHUNK_SIZE + y) * (x_len + 1)] = '-';
                }
            }
        }

        chunk1 = chunk1->next;
    }

}

