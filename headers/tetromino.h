#ifndef CMDTTRS_TETROMINO_H_SENTRY
#define CMDTTRS_TETROMINO_H_SENTRY


#include <win_mod.h>
#include <cell_mod.h>
#include <err_mod.h>
#include <math.h>
#include <string.h>
#include "test_win.h"

enum cells { dot = '.', blank = ' ', t1 = 'T', brdr = '/' };
enum moves { T_LEFT = 'a', T_RIGHT = 'd', T_UP = 'w', T_DOWN = 's' };
struct tetromino_s {
    struct board_s  *brd;
    float           x, y, dx, dy;
    char            cell;
};

struct board_s {
    char    *brd;
    uint32_t w, h;
};

#define init_tetr(tetr) struct tetromino_s *tetr = calloc(1, sizeof(*tetr));   \
                        check_alloc(tetr, 0, "init_tetr")

#define dest_tetr(tetr) free(tetr)

int handle_mv_tetr(struct t_win_s *win, struct tetromino_s *tetr, float et, char key);
int can_mv_tetr(struct tetromino_s *tetr, float dx, float dy);
void mv_tetr(struct t_win_s * win, struct tetromino_s *tetr, float dx, float dy);

struct board_s * init_brd(void);
void map_brd_to_win(struct board_s *brd, struct t_win_s *win, uint32_t n);


#endif
