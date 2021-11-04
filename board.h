//
// Created by 15533 on 2021/11/2.
//

#ifndef CONWAYSGAMEOFLIFE_BOARD_H
#define CONWAYSGAMEOFLIFE_BOARD_H

struct CHUNK;

struct BOARD {
    struct CHUNK *init_chunk;
    long long round;
    long long chunk_num;
    long long total_cell_num;
};


void init_board(struct BOARD *board);
void print_board_to_str(struct BOARD *board, char **target);

#endif //CONWAYSGAMEOFLIFE_BOARD_H
