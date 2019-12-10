#include "main.h"

#ifndef GUI_HPP
#define GUI_HPP

lv_obj_t * createBtn(lv_obj_t * parent, lv_coord_t x, lv_coord_t y,
                     lv_coord_t width, lv_coord_t height,int id,
                     const char * title);

lv_style_t btnStyleBuilder(lv_color_t color, lv_color_t borderColor);

#endif
