#include "vterm.h"
#include "string.h"
#include "stdio.h"
#include "limits.h"

struct terminal {
    VTerm *vterm;
    VTermScreen *screen;
    int width;
    int height;
    int redraw_row_start;
    int redraw_row_end;
};

static void update_redraw_lines(struct terminal *terminal,
                                int start_row, int end_row)
{
    if (start_row < terminal->redraw_row_start) {
        terminal->redraw_row_start = start_row;
    }
    if (terminal->redraw_row_end < end_row) {
        terminal->redraw_row_end = end_row;
    }
}

static void print_cell(VTermScreen *screen, VTermScreenCell *cell)
{
    printf("chars: %s\n", cell->chars);
    printf("width: %d\n", cell->width);
    printf("bold: %d\n", cell->attrs.bold);
    printf("underline: %d\n", cell->attrs.underline);
    printf("italic: %d\n", cell->attrs.italic);
    printf("blink: %d\n", cell->attrs.blink);
    printf("reverse: %d\n", cell->attrs.reverse);
    printf("strike: %d\n", cell->attrs.strike);
    printf("font: %d\n", cell->attrs.font);
    printf("dwl: %d\n", cell->attrs.dwl);
    printf("dhl: %d\n", cell->attrs.dhl);

    // printf("%d\n", VTERM_COLOR_IS_INDEXED(&(cell->fg)));
    // printf("%d\n", VTERM_COLOR_IS_RGB(&(cell->fg)));

    // printf("%d\n", cell->fg.type);
    // printf("%d\n", cell->fg.indexed.idx);

    VTermColor fg = cell->fg;
    VTermColor bg = cell->bg;

    vterm_screen_convert_color_to_rgb(screen, &fg);
    printf("%d %d %d\n", fg.rgb.red, fg.rgb.green, fg.rgb.blue);

    vterm_screen_convert_color_to_rgb(screen, &bg);
    printf("%d %d %d\n", bg.rgb.red, bg.rgb.green, bg.rgb.blue);
}

static void reset(struct terminal *terminal)
{
    terminal->redraw_row_start = INT_MAX;
    terminal->redraw_row_end = 0;
}

static void redraw(struct terminal *terminal)
{
    if (terminal->redraw_row_start > terminal->redraw_row_end)
        return;

    for (int row = terminal->redraw_row_start;
         row < terminal->redraw_row_end;
         row++) {
        for (int col = 0; col < terminal->width; col++) {
            VTermScreenCell cell;
            VTermPos pos = {row, col};
            vterm_screen_get_cell(terminal->screen, pos, &cell);
            printf("\n\n%d %d\n", row, col);
            print_cell(terminal->screen, &cell);
        }
    }

    reset(terminal);
}

static int cb_damage(VTermRect rect, void *terminal)
{
    printf("damage (%d %d) (%d %d)\n",
           rect.start_row,
           rect.end_row,
           rect.start_col,
           rect.end_col);
    update_redraw_lines(terminal, rect.start_row, rect.end_row);
    return 1;
}

static int cb_moverect(VTermRect dest, VTermRect src, void *terminal)
{
    printf("moverect\n");
    return 1;
}

static int cb_movecursor(VTermPos pos, VTermPos oldpos, int visible, void *terminal)
{
    printf("movecursor pos = (%d %d) oldpos = (%d %d) visible = %d\n",
           pos.row,
           pos.col,
           oldpos.row,
           oldpos.col,
           visible);
    return 1;
}

static int cb_settermprop(VTermProp prop, VTermValue *val, void *terminal)
{
    printf("settermprop\n");
    return 1;
}

static int cb_bell(void *terminal)
{
    printf("bell\n");
    return 1;
}

static int cb_resize(int rows, int cols, void *terminal)
{
    printf("resize\n");
    return 1;
}

static int cb_sb_pushline(int cols, const VTermScreenCell *cells, void *terminal)
{
    printf("sb_pushline\n");
    return 1;
}

static int cb_sb_popline(int cols, VTermScreenCell *cells, void *terminal)
{
    printf("sb_popline\n");
    return 1;
}

static VTermScreenCallbacks callbacks = {
    cb_damage,
    cb_moverect,
    cb_movecursor,
    cb_settermprop,
    cb_bell,
    cb_resize,
    cb_sb_pushline,
    cb_sb_popline,
};

int main(void)
{
    const char *str = "\033[31mHello \033[32mWorld\033[0m";

    VTerm *vterm = vterm_new(80, 24);
    vterm_set_utf8(vterm, 1);

    VTermScreen *screen = vterm_obtain_screen(vterm);

    struct terminal terminal;
    terminal.screen = screen;
    terminal.vterm = vterm;
    terminal.width = 80;
    terminal.height = 24;
    terminal.redraw_row_start = INT_MAX;
    terminal.redraw_row_end = 0;

    vterm_screen_set_callbacks(screen, &callbacks, &terminal);
    vterm_screen_reset(screen, 1);
    vterm_input_write(vterm, str, strlen(str) + 1);
    vterm_screen_flush_damage(screen);

    redraw(&terminal);

    puts("-----------------------");

    vterm_input_write(vterm, str, strlen(str) + 1);
    vterm_screen_flush_damage(screen);

    redraw(&terminal);

    return 0;
}
