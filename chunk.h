//
// Created by 15533 on 2021/11/2.
//

#ifndef CONWAYSGAMEOFLIFE_CHUNK_H
#define CONWAYSGAMEOFLIFE_CHUNK_H

#define CHUNK_SIZE 32
#define MAX_EMPTY_ROUND 10  // 连续 10 轮都没有被使用到的区块将会被回收

struct CHUNK {
    unsigned int data[CHUNK_SIZE];  // 每个 int 32 bit 代表一列
    int x, y;  // 区块坐标

    int cell_num;  // 当前包含的细胞数目
    int recycle_counter;  // 回收计数器

    struct CHUNK *next, *prev;  // 链表
};

struct BOARD;
struct CHUNK* init_chunk(struct BOARD *board, int x, int y);
int get_cell(const unsigned int *data, int x, int y);
int get_cell_in_chunk(struct CHUNK *chunk, int x, int y, struct BOARD *board);
void set_cell(unsigned int *data, int x, int y);
struct CHUNK* find_chunk(struct BOARD *board, int x, int y);
void free_chunk(struct CHUNK *chunk, struct BOARD *board);
void init_cell_in_board(struct BOARD *board, long long x, long long y);

#endif //CONWAYSGAMEOFLIFE_CHUNK_H
