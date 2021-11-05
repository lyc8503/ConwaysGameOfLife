//
// Created by 15533 on 2021/11/2.
//

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "chunk.h"
#include "board.h"


struct CHUNK* init_chunk(struct BOARD *board, int x, int y) {

    struct CHUNK *last_chunk = board->init_chunk;  // 找到最后一个区块
    while(last_chunk->next != NULL) {
        if (last_chunk->x == x && last_chunk->y == y) {  // 区块已经存在, 不创建
            return NULL;
        }
        last_chunk = last_chunk->next;
    }

    if (last_chunk->x == x && last_chunk->y == y) {  // 区块已经存在, 不创建
        return NULL;
    }

    struct CHUNK *chunk;
    chunk = (struct CHUNK*) malloc(sizeof(struct CHUNK));
    board->chunk_num++;

    // 初始化区块
    printf("[CHUNK] new chunk at: (%d, %d)\n", x, y);

    memset(chunk, 0, sizeof(struct CHUNK));

    chunk->x = x;
    chunk->y = y;
    chunk->cell_num = 1;  // 第一次无论如何还是检查一次
    chunk->recycle_counter = MAX_EMPTY_ROUND;

    chunk->prev = last_chunk;
    last_chunk->next = chunk;
    chunk->next = NULL;
    return chunk;
}

// 查找目标区块
struct CHUNK* find_chunk(struct BOARD *board, int x, int y) {

    struct CHUNK *chunk = board->init_chunk;
    while (chunk != NULL) {
        if (chunk->x == x && chunk->y == y) {
            return chunk;
        }
        chunk = chunk->next;
    }

    return NULL;
}

int get_cell(const unsigned int *data, int x, int y) {
    return (data[x] & (1 << y)) > 0;
}

// 自动获取相邻区块的细胞状态
int get_cell_in_chunk(struct CHUNK *chunk, int x, int y, struct BOARD *board) {
    if (x >= 0 && x <= CHUNK_SIZE - 1 && y >= 0 && y <= CHUNK_SIZE - 1) {
        return get_cell(chunk->data, x, y);
    } else {
        int chunk_x = chunk->x;
        int chunk_y = chunk->y;

        // 跨区块处理
        if (x < 0) {
            x += CHUNK_SIZE;
            chunk_x -= 1;
        }
        if (x > CHUNK_SIZE - 1) {
            x -= CHUNK_SIZE;
            chunk_x += 1;
        }
        if (y < 0) {
            y += CHUNK_SIZE;
            chunk_y -= 1;
        }
        if (y > CHUNK_SIZE - 1) {
            y -= CHUNK_SIZE;
            chunk_y += 1;
        }

        struct CHUNK *target_chunk = find_chunk(board, chunk_x, chunk_y);

        if (target_chunk == NULL) {  // 如果目标区块还未被初始化 代表其不会被用到 全部为空
            return 0;
        } else {
//            target_chunk->recycle_counter = MAX_EMPTY_ROUND;
            return get_cell(target_chunk->data, x, y);
        }
    }
}

void set_cell(unsigned int *data, int x, int y) {
    data[x] |= (1 << y);
}

// 仅限初始化时可用的设置细胞方法
void init_cell_in_board(struct BOARD *board, long long x, long long y) {
    int chunk_x = (int) (x / CHUNK_SIZE);
    int x_in_chunk = (int) (x % CHUNK_SIZE);
    int chunk_y = (int) (y / CHUNK_SIZE);
    int y_in_chunk = (int) (y % CHUNK_SIZE);

    struct CHUNK *chunk = find_chunk(board, chunk_x, chunk_y);

    if (chunk == NULL) {  // 如果目标区块不存在创建
        chunk = init_chunk(board, chunk_x, chunk_y);
    }

    // 设置目标细胞
    set_cell(chunk->data, x_in_chunk, y_in_chunk);
    chunk->cell_num++;
}

void free_chunk(struct CHUNK *chunk, struct BOARD *board) {
    printf("[CHUNK] delete chunk at: (%d, %d)\n", chunk->x, chunk->y);

    if (board->chunk_num == 1) {  // 只有一个区块时不能回收
        return;
    }

    if (chunk->prev == NULL) {
        board->init_chunk = chunk->next;
        chunk->next->prev = NULL;
    } else {
        chunk->prev->next = chunk->next;
    }

    if (chunk->next != NULL) {
        chunk->next->prev = chunk->prev;
    }

    free(chunk);
    board->chunk_num--;
}
