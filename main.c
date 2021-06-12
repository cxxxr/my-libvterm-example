#include "vterm.h"
#include "string.h"
#include "stdio.h"

int cb_damage(VTermRect rect, void *user)
{
    printf("damage (%d %d) (%d %d)\n", rect.start_row, rect.end_row, rect.start_col, rect.end_col);
    return 0;
}

int cb_moverect(VTermRect dest, VTermRect src, void *user)
{
    printf("moverect\n");
    return 1;
}

int cb_movecursor(VTermPos pos, VTermPos oldpos, int visible, void *user)
{
    printf("movecursor pos = (%d %d) oldpos = (%d %d) visible = %d\n", pos.row, pos.col, oldpos.row, oldpos.col, visible);
    return 1;
}

int cb_settermprop(VTermProp prop, VTermValue *val, void *user)
{
    printf("settermprop\n");
    return 1;
}

int cb_bell(void *user)
{
    printf("bell\n");
    return 1;
}

int cb_resize(int rows, int cols, void *user)
{
    printf("resize\n");
    return 1;
}

int cb_sb_pushline(int cols, const VTermScreenCell *cells, void *user)
{
    printf("sb_pushline\n");
    return 1;
}

int cb_sb_popline(int cols, VTermScreenCell *cells, void *user)
{
    printf("sb_popline\n");
    return 1;
}

VTermScreenCallbacks callbacks = {
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
    vterm_screen_set_callbacks(screen, &callbacks, NULL);
    vterm_screen_reset(screen, 1);
    vterm_input_write(vterm, str, strlen(str) + 1);
    vterm_screen_flush_damage(screen);

    VTermPos pos = {0, 0};
    VTermScreenCell cell;
    vterm_screen_get_cell(screen, pos, &cell);

    // for (int i = 0; i < 6; i++) {
    //     printf("chars %d: %d\n", i, cell.chars[i]);
    // }

    // printf("width: %d\n", cell.width);
    // printf("bold: %d\n", cell.attrs.bold);
    // printf("underline: %d\n", cell.attrs.underline);
    // printf("italic: %d\n", cell.attrs.italic);
    // printf("blink: %d\n", cell.attrs.blink);
    // printf("reverse: %d\n", cell.attrs.reverse);
    // printf("strike: %d\n", cell.attrs.strike);
    // printf("font: %d\n", cell.attrs.font);
    // printf("dwl: %d\n", cell.attrs.dwl);
    // printf("dhl: %d\n", cell.attrs.dhl);

    // printf("%d\n", VTERM_COLOR_IS_INDEXED(&(cell.fg)));
    // printf("%d\n", VTERM_COLOR_IS_RGB(&(cell.fg)));

    // printf("%d\n", cell.fg.type);
    // printf("%d\n", cell.fg.indexed.idx);

    // VTermColor col;
    // vterm_screen_convert_color_to_rgb(screen, &col);
    // printf("%d %d %d\n", col.rgb.red, col.rgb.green, col.rgb.blue);

    return 0;
}
