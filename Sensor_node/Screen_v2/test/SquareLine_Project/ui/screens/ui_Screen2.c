// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 8.3.11
// Project name: Srceen_AIoT_PTIT

#include "../ui.h"

void ui_Screen2_screen_init(void)
{
    ui_Screen2 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Image1 = lv_img_create(ui_Screen2);
    lv_img_set_src(ui_Image1, &ui_img_bg_png);
    lv_obj_set_width(ui_Image1, LV_SIZE_CONTENT);   /// 240
    lv_obj_set_height(ui_Image1, LV_SIZE_CONTENT);    /// 240
    lv_obj_set_align(ui_Image1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_ImageArmHour = lv_img_create(ui_Screen2);
    lv_img_set_src(ui_ImageArmHour, &ui_img_armhour_png);
    lv_obj_set_width(ui_ImageArmHour, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ImageArmHour, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ImageArmHour, 0);
    lv_obj_set_y(ui_ImageArmHour, -35);
    lv_obj_set_align(ui_ImageArmHour, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageArmHour, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_ImageArmHour, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_pivot(ui_ImageArmHour, 9, 77);
    lv_img_set_angle(ui_ImageArmHour, 5);

    ui_ImageArmMinute = lv_img_create(ui_Screen2);
    lv_img_set_src(ui_ImageArmMinute, &ui_img_armminute_png);
    lv_obj_set_width(ui_ImageArmMinute, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ImageArmMinute, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ImageArmMinute, 0);
    lv_obj_set_y(ui_ImageArmMinute, -49);
    lv_obj_set_align(ui_ImageArmMinute, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageArmMinute, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_ImageArmMinute, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_pivot(ui_ImageArmMinute, 9, 105);
    lv_img_set_angle(ui_ImageArmMinute, 4500);

    ui_ImageArmSecond = lv_img_create(ui_Screen2);
    lv_img_set_src(ui_ImageArmSecond, &ui_img_armsecond_png);
    lv_obj_set_width(ui_ImageArmSecond, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ImageArmSecond, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ImageArmSecond, -71);
    lv_obj_set_y(ui_ImageArmSecond, -35);
    lv_obj_set_align(ui_ImageArmSecond, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageArmSecond, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_ImageArmSecond, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_pivot(ui_ImageArmSecond, 5, 115);
    lv_img_set_angle(ui_ImageArmSecond, 800);

}