#include "test_win.h"

struct win_arr_s *win_arr;
pthread_mutex_t win_arr_mut     = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t main_scr_mut    = PTHREAD_MUTEX_INITIALIZER;

//TEMP VARS!!!
//struct win_s *msp;


void init_win(struct t_win_s *win, int x, int y, int w, int h, struct win_s *scrn)
{
    win->x = x;
    win->y = y;
    win->w = w;
    win->h = h;  

    pthread_mutex_lock(&win_arr_mut);
    win->no = win_arr->len;
    pthread_mutex_unlock(&win_arr_mut);
    add_win_to_arr(win);

    win->map = calloc(h, sizeof(*(win->map)));
    win->win_buf = calloc(h * w, sizeof(*win->win_buf));  

    win->scr = scrn;
    map_winbuf_to_scr(win, scrn);
}

void map_winbuf_to_scr(struct t_win_s *win, struct win_s *scrn)
{
    uint32_t wh = win->h;
    uint32_t wy = win->y;
    uint32_t wx = win->x;
    uint32_t sw = scrn->w;
    size_t i    = 0;

    for( ; i < wh; ++i) {
        win->map[i] = &scrn->win[ (wy+ i) * sw + wx];
    }
}

void put_scr_to_winbuf(struct t_win_s *win)
{
    char * restrict buf = win->win_buf;
    size_t  wh  = win->h;
    size_t  ww  = win->w;
    size_t  i   = 0;

    for ( ; i < wh; ++i) {
        memmove(buf + i*ww, win->map[i], ww); 
    }
}

void put_winbuf_to_scr(struct t_win_s *win)
{
    char * restrict buf   = win->win_buf;
    size_t  wh  = win->h;
    size_t  ww  = win->w;
    size_t  i   = 0;
    
    pthread_mutex_lock(&main_scr_mut);

    for ( ; i < wh; ++i ) {
        memmove(win->map[i], buf + i*ww, ww);  
    }    

    pthread_mutex_unlock(&main_scr_mut);
}

u_int32_t count = 0;
void add_win_to_arr(struct t_win_s *win)
{
    pthread_mutex_lock(&win_arr_mut);

    u_int32_t len = win_arr->len;
    u_int32_t max = win_arr->max;
    
    if ((len + 1) >= max) {
        win_arr->arr = realloc(win_arr->arr, (win_arr->max <<= 2) );
    }
    
    win_arr->arr[len] = win;
    win_arr->len++; 

    pthread_mutex_unlock(&win_arr_mut);
}

void fill_win(struct t_win_s * win, char what)
{
    size_t  size    = win->h;
    size_t  i       = 0;
    int     win_w   = win->w;
    
    for ( ; i < size; ++i ) {
        memset(win->win_buf+win_w*i, what, win_w);
    } 
}

void make_win_brdr(struct t_win_s *win, char hb, char vb)
{
    FILE *fd = fopen("log.txt", "a");
    fprintf(fd, "no is %d\n", win->no);
    char * restrict a   = win->win_buf;
    size_t  h           = win->h - 1;
    size_t  w           = win->w;
    size_t  i           = 1; 

    memset(&a[0], hb, w);    
    memset(&a[h*w], hb, w);    

    for ( ; i < h; ++i) {
        fprintf(fd, "h is %lu and i is %lu\n", h, i);
        a[w*i]      = vb;
        a[w*i+w-1]  = vb; 
    } 
}

void handle_mv_win(struct t_win_s *win, char key)
{
    switch(key) {
        case(UP):       mv_win(win, 0, -1); break;
        case(DOWN):     mv_win(win, 0,  1); break;
        case(LEFT):     mv_win(win, -1, 0); break;
        case(RIGHT):    mv_win(win, 1,  0); break;
        case('r'):      fill_win(win, scrbg_bl); break;
    };
}

int mv_win(struct t_win_s *win, int dx, int dy )
{
    if(!can_mv_win(win, dx, dy))
        return 1;
    
    pthread_mutex_lock(&main_scr_mut); 

    char * restrict scr = win->scr->win;
    uint32_t sw         = win->scr->w;
    uint32_t ww         = win->w;
    uint32_t wh         = win->h;
    uint32_t wx         = win->x; 
    uint32_t wy         = win->y;

    for(size_t i = 0; i < wh; ++i) {
        memset(&scr[sw*(wy+i) + wx], scrbg_bl, ww); 
    }

    win->x += dx;
    win->y += dy; 
  
    pthread_mutex_unlock(&main_scr_mut); 

    map_winbuf_to_scr(win, win->scr);
    
    return 0;
}

int can_mv_win(struct t_win_s *win, int dx, int dy)
{
    int scr_w = win->scr->w;
    int scr_h = win->scr->h;

    int ww = win->w;
    int wh = win->h;
    int nx = win->x + dx;
    int ny = win->y + dy;

    return (nx >= 0) * (ww + nx <= scr_w) * (ny >= 0) * (wh + ny <= scr_h);
}

void toggle_win_foc(char key)
{
    pthread_mutex_lock(&win_arr_mut);

    FILE *fd = fopen("log.txt", "a");
    fprintf(fd, "IN TOGGLE OLD I is %" PRIu32 "\n", win_arr->i);

    u_int32_t old_i = win_arr->i, len = win_arr->len; 

    switch(key) {
        case(TOGL_F):   win_arr->i = (old_i + 1) % len;   
                        break;
        case(TOGL_B):   win_arr->i = abs(((int32_t) (old_i - 1)) % len); //IS THIS IS FORBIDDEN?
                        break;
    };

    pthread_mutex_unlock(&win_arr_mut);

    fprintf(fd, "IN TOGGLE NEW I is %" PRIu32 "\n", win_arr->i);
    struct t_win_s **arr = win_arr->arr;
    make_win_brdr(arr[old_i], bdw_uf, bdw_uf); 
    make_win_brdr(arr[win_arr->i], bdw_f, bdw_f); 
}

void render_all_wins(void)
{
    pthread_mutex_lock(&win_arr_mut);

    uint32_t size   = win_arr->len;
    uint32_t indx   = win_arr->i + 1;
    uint32_t i      = 0;
    
    for ( ; i < size; ++i) {
        render_win(win_arr->arr[indx % size]); 
        indx++;
    }

    pthread_mutex_unlock(&win_arr_mut);
}

void render_win(struct t_win_s *win)
{
    put_winbuf_to_scr(win);   
}

void test_test(void)
{
    
    pthread_mutex_lock(&win_arr_mut);

    FILE *fd = fopen("log.txt", "a"); 
    fprintf(fd, "max = %" PRIu32 "len = %" PRIu32 "\n", win_arr->max, win_arr->len); 
    for(size_t i = 0; i < win_arr->len; ++i) {
        fprintf(fd, "win no is %d while i is %lu\n", win_arr->arr[i]->no, i);
    }

    pthread_mutex_unlock(&win_arr_mut);
}

