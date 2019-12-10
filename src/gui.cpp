#include "main.h"

/**
 * This file contains functions for the autonomous selection GUI.
 * Once initialization of the screen is done, these functions may be used in a
 * task in opcontrol to allow the screen to be controlled.
 */

/**
 * Creates a button and sets a bunch of useful styles
 * @param  parent - the button's parent (usually the current screen)
 * @param  x - x coord of button
 * @param  y - y coord of button
 * @param  width - width of button
 * @param  height - height of button
 * @param  id - unique number to set for the button
 * @param  title - text to show on button
 * @return btn, the created button
 */
 lv_obj_t * createBtn(lv_obj_t * parent, lv_coord_t x, lv_coord_t y,
                      lv_coord_t width, lv_coord_t height,int id,
                      const char * title) {
     lv_obj_t * btn = lv_btn_create(parent, NULL);
     lv_obj_set_pos(btn, x, y);
     lv_obj_set_size(btn, width, height);
     lv_obj_set_free_num(btn, id);

     lv_obj_t * label = lv_label_create(btn, NULL);
     lv_label_set_text(label, title);
     //align the button's label in the center of the button
     lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

     return btn;
 }

 /**
  * Creates a style intended for use by a button
  * @param  color - backround color of the style's body
  * @param  borderColor - color of the style's border
  * @param  textColor - color of the style's text
  * @return resultStyle, the created style
  */
 lv_style_t btnStyleBuilder(lv_color_t color, lv_color_t borderColor) {
   lv_style_t resultStyle;
   lv_style_copy(&resultStyle, &lv_style_plain_color);

   //defined from user input
   resultStyle.body.main_color = color;
   resultStyle.body.border.color = borderColor;

   return resultStyle;
 }
