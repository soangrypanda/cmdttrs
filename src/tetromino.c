#include "tetromino.h"

int handle_mv_tetr(struct t_win_s *win, struct tetromino_s *tetr, float et, char key)
{
    float dx = 0, dy = 0; 
    
    switch(key){
        case(T_LEFT):  dx = -tetr->dx * et; break;
        case(T_RIGHT): dx =  tetr->dx * et; break;
        case(T_UP):    dy = -tetr->dy * et; break;
        case(T_DOWN):  dy =  tetr->dy * et; break;
        default:       break;
    }

    if(can_mv_tetr(tetr, dx, dy))
        mv_tetr(win, tetr, dx, dy);
    else
        return 1;

    return 0;
}

int can_mv_tetr(struct tetromino_s *tetr, float dx, float dy)
{
    uint32_t w          = tetr->brd->w;
    uint32_t h          = tetr->brd->h;
    int32_t nx          = (int32_t) (tetr->x + dx);
    int32_t ny          = (int32_t) (tetr->y + dy);
   
    return (nx < w) * (nx >= 0) * (ny < h) * (ny >= 0); 
}

void mv_tetr(struct t_win_s * win, struct tetromino_s *tetr, float dx, float dy)
{   
/*
    char * restrict w   = win->win_buf;
    
    w[win->w * (int32_t)tetr->y + (int32_t)tetr->x] = bgw_bl;

    tetr->x += dx;
    tetr->y += dy;

    w[win->w * (int32_t)tetr->y + (int32_t)tetr->x] = tetr->cell;
*/
    struct board_s *b   = tetr->brd;
    char * restrict ba  = tetr->brd->brd;
    
    ba[b->w * (int32_t)tetr->y + (int32_t)tetr->x] = '*';

    tetr->x += dx;
    tetr->y += dy;

    ba[b->w * (int32_t)tetr->y + (int32_t)tetr->x] = tetr->cell;
    
    
}

struct board_s * init_brd(void)
{
    struct board_s *brd = calloc(1, sizeof(*brd));
    brd->w              = 15;
    brd->h              = 15;
    brd->brd            = calloc(brd->w * brd->h, sizeof(*(brd->brd)));

    uint32_t w          = brd->w;
    uint32_t h          = brd->h;
    char *b             = brd->brd;

    memset(b, '*', w*h);
    
    return brd;
}

void map_brd_to_win(struct board_s *brd, struct t_win_s *win, uint32_t n)
{
    //FILE *fd = fopen("map.txt", "w");
    char * restrict wa  = win->win_buf;
    char * restrict ba  = brd->brd;
    uint32_t ww =   win->w;
    uint32_t wh =   win->h;
    uint32_t bw =   brd->w;
    uint32_t bh =   brd->h; 
    uint32_t dw =   ww-1 > bw ? bw : ww-2;
    uint32_t dh =   wh-1 > bh ? bh : wh-2;
        
    /* forgive me, Lord, for what I am about to do */
    for(size_t h = 0; h*n < wh-2 && h < bh; ++h) {
        for(size_t w = 0; w*n < ww-2 && w < bw; ++w) {
            char cell = ba[h*bw+w];
            for(size_t nh = 0; nh < n && h*n+nh < wh-2; ++nh) {
                for(size_t nw = 0; nw < n && w*n+nw < ww-2; ++nw) {
                    wa[ww*(1+h*n+nh)+(1+w*n+nw)] = cell; 
                }
            } 
        }
    }

/*  
    For future records and curious ancestors - the following
    is the first version of the algorythm above I don't dare
    to delete. It is probably faster because there is no 4th
    loop, but math is falty - with some of n's the number of
    bytes to memset is calculated wrong. Not sure now how to 
    fix that without messing this code beyond comprehension. 
*/

/*
    for(size_t i = 0; i*n < wh-2 && i < bh; i++)          {
        for(size_t y = 0; y*n < ww-2 && y < bw; y++)      {
            int d = y*n+n>=ww ? n-(ww-y*n-2) : n;
            //int d = ww-y-2;
            fprintf(fd, "%" PRIu32 " %d ww - %" PRIu32 "y - %ld n - %d \n", n, d, ww, y, n);
            for(size_t z = 0; z < n && i*n+z < wh-2; ++z)       {
                memset(&w[ww*(1+i*n+z)+(y*n+1)], b[i*bw+y], d); 
            }
        }
    }
*/
}
