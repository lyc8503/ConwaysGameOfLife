//
// Created by 15533 on 2021/11/2.
//

#include "board.h"
#include "chunk.h"
#include "round.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>


void next_round(struct BOARD *board) {
    ++board->round;

    // 遍历棋盘 按需创建新区块
    struct CHUNK *chunk = board->init_chunk;
    while (chunk != NULL) {

        if (chunk->cell_num == 0) {  // 空区块无需检查
            chunk = chunk->next;
            continue;
        }

        // 四个角落的处理
        if (get_cell(chunk->data, 0, 0)) {
            init_chunk(board, chunk->x-1, chunk->y-1);
            init_chunk(board, chunk->x-1, chunk->y);
            init_chunk(board, chunk->x, chunk->y-1);
        }

        if (get_cell(chunk->data, CHUNK_SIZE - 1, 0)) {
            init_chunk(board, chunk->x+1, chunk->y-1);
            init_chunk(board, chunk->x+1, chunk->y);
            init_chunk(board, chunk->x, chunk->y-1);
        }

        if (get_cell(chunk->data, 0, CHUNK_SIZE - 1)) {
            init_chunk(board, chunk->x-1, chunk->y+1);
            init_chunk(board, chunk->x-1, chunk->y);
            init_chunk(board, chunk->x, chunk->y+1);
        }

        if (get_cell(chunk->data, CHUNK_SIZE - 1, CHUNK_SIZE - 1)) {
            init_chunk(board, chunk->x+1, chunk->y+1);
            init_chunk(board, chunk->x+1, chunk->y);
            init_chunk(board, chunk->x, chunk->y+1);
        }

        // 左边界
        if (chunk->data[0] != 0) {
            init_chunk(board, chunk->x-1, chunk->y);
        }

        // 右边界
        if (chunk->data[CHUNK_SIZE - 1] != 0) {
            init_chunk(board, chunk->x+1, chunk->y);
        }

        // 上边界
        for (int x = 0; x < CHUNK_SIZE; x++) {
            if (get_cell(chunk->data, x, 0)) {
                init_chunk(board, chunk->x, chunk->y-1);
                break;
            }
        }

        // 下边界
        for (int x = 0; x < CHUNK_SIZE; x++) {
            if (get_cell(chunk->data, x, CHUNK_SIZE - 1)) {
                init_chunk(board, chunk->x, chunk->y+1);
                break;
            }
        }

        chunk = chunk->next;
    }

    // 遍历棋盘 进行计算 暂存结果 更新细胞数
    // TODO: recycle counter
    unsigned int *temp = (unsigned int*) malloc(sizeof(unsigned int) * board->chunk_num * CHUNK_SIZE);
    memset(temp, 0, sizeof(unsigned int) * board->chunk_num * CHUNK_SIZE);
    chunk = board->init_chunk;
    int i = 0, cell_counter = 0;
    while (chunk != NULL) {
        cell_counter = 0;

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                int neighbours = get_cell_in_chunk(chunk, x - 1, y - 1, board)
                               + get_cell_in_chunk(chunk, x - 1, y, board)
                               + get_cell_in_chunk(chunk, x - 1, y + 1, board)
                               + get_cell_in_chunk(chunk, x, y - 1, board)
                               + get_cell_in_chunk(chunk, x, y + 1, board)
                               + get_cell_in_chunk(chunk, x + 1, y - 1, board)
                               + get_cell_in_chunk(chunk, x + 1, y, board)
                               + get_cell_in_chunk(chunk, x + 1, y + 1, board);

//                printf("neighbours: %d (%d, %d)\n", neighbours, x, y);
                if ((neighbours == 2 && get_cell(chunk->data, x, y)) || neighbours == 3) {
                    set_cell(temp + CHUNK_SIZE * i, x, y);
                    cell_counter++;
                }
            }
        }
        i++;
        chunk->cell_num = cell_counter;

        chunk->recycle_counter--;
        if (chunk->cell_num > 0) {
            chunk->recycle_counter = MAX_EMPTY_ROUND;
        }

        chunk = chunk->next;
    }

    // 写入数据
    chunk = board->init_chunk;
    i = 0;
    int total_cell_counter = 0;
    while (chunk != NULL) {
        memcpy(chunk->data, temp + CHUNK_SIZE * i, sizeof(unsigned int) * CHUNK_SIZE);
        i++;
        total_cell_counter += chunk->cell_num;
        chunk = chunk->next;
    }

    board->total_cell_num = total_cell_counter;

    // 释放内存
    free(temp);

    // 回收未用到的区块
    chunk = board->init_chunk;
    while (chunk != NULL) {
        if (chunk->recycle_counter <= 0) {
            free_chunk(chunk, board);
        }
        chunk = chunk->next;
    }
}
