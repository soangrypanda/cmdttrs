#include <stdio.h>
#include <win_mod.h>
#include <cell_mod.h>
#include <elpsd_t.h>

#include "tetromino.h"
//#include "test_win.h"

#define MAP_N 1

int main(void)
{
    init_win_mod();
    struct win_s *main_scr = init_mainscreen();
    //msp = main_scr;
    fill_win(main_scr, scrbg_bl); 

    win_arr = calloc(1, sizeof(*win_arr));
    win_arr->max = WIN_ARR_MAX;
    win_arr->len = 0;
    win_arr->i   = 0;
    win_arr->arr = calloc(WIN_ARR_MAX, sizeof(*win_arr->arr));

    struct win_s *t_win = calloc(1, sizeof(*t_win));
    init_win(t_win, 1, 1, 20, 20, main_scr);
    test_test();
    fill_win(t_win, bgw_bl);
    make_win_brdr(t_win, bdw_uf, bdw_uf);

    struct win_s *t_win1 = calloc(1, sizeof(*t_win1));
    init_win(t_win1, 30, 1, 10, 10, main_scr);
    test_test();
    fill_win(t_win1, bgw_bl);
    make_win_brdr(t_win1, bdw_uf, bdw_uf);
/*
    struct win_s *t_win2 = calloc(1, sizeof(*t_win2));
    init_win(t_win2, 45, 1, 5, 10, main_scr);
    add_win_to_arr(t_win2);
    test_test();
    fill_win(t_win2, bgw_bl);
    make_win_brdr(t_win2, bdw_uf, bdw_uf);

    struct win_s *t_win3 = calloc(1, sizeof(*t_win3));
    init_win(t_win3, 52, 1, 5, 5, main_scr);
    add_win_to_arr(t_win3);
    test_test();
    fill_win(t_win3, bgw_bl);
    make_win_brdr(t_win3, bdw_uf, bdw_uf);
*/
    make_win_brdr(win_arr->arr[win_arr->i], bdw_f, bdw_f); 

    init_tetr(tetr);
    tetr->x = 1; tetr->y = 1;
    tetr->cell= t1;
    tetr->dx = 40, tetr->dy = 40; 
    tetr->brd = init_brd();
    
    float et;
    set_timer();
    
    char key = 0;
    
    while((key = getch()) != blank) {

        et = get_elapsed_time();

        handle_mv_tetr(t_win, tetr, et, key);
        map_brd_to_win(tetr->brd, t_win, MAP_N);

        if ( (key == 'q') || (key == 'Q') )
            toggle_win_foc(key);
        if ( (key == 'h') || (key == 'j') || (key == 'k') || (key == 'l') || (key == 'r') )
            handle_mv_win(win_arr->arr[win_arr->i], key);

        render_screen(main_scr);
        render_all_wins();
        refresh();
    }

    dest_tetr(tetr);
    //destroy_win(main_scr);     
    endwin();
}
