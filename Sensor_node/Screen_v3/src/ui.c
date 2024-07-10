// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 8.3.6
// Project name: Srceen_AIoT_PTIT

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Image8;
lv_obj_t * ui_Image12;
lv_obj_t * ui_Label1;
lv_obj_t * ui_Image14;


// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
lv_obj_t * ui_Screen2;
lv_obj_t * ui_Image1;
lv_obj_t * ui_ImageArmHour;
lv_obj_t * ui_ImageArmMinute;
lv_obj_t * ui_ImageArmSecond;


// SCREEN: ui_Screen3
void ui_Screen3_screen_init(void);
lv_obj_t * ui_Screen3;
lv_obj_t * ui_Label2;
lv_obj_t * ui_TEMP;
lv_obj_t * ui_Label4;
lv_obj_t * ui_Label5;
lv_obj_t * ui_Label6;
lv_obj_t * ui_HUMI;
lv_obj_t * ui_Label8;
lv_obj_t * ui_Label12;
lv_obj_t * ui_Label13;


// SCREEN: ui_Screen4
void ui_Screen4_screen_init(void);
lv_obj_t * ui_Screen4;
lv_obj_t * ui_Day;
lv_obj_t * ui_Date;
lv_obj_t * ui_month;
lv_obj_t * ui_hour_1;
lv_obj_t * ui_hour_2;
lv_obj_t * ui_Label16;
lv_obj_t * ui_minute_1;
lv_obj_t * ui_minute_2;


// SCREEN: ui_Screen5
void ui_Screen5_screen_init(void);
lv_obj_t * ui_Screen5;
lv_obj_t * ui_Image2;
lv_obj_t * ui_Image3;


// SCREEN: ui_Screen6
void ui_Screen6_screen_init(void);
lv_obj_t * ui_Screen6;
lv_obj_t * ui_Image15;
lv_obj_t * ui_Label3;
lv_obj_t * ui____initial_actions0;
const lv_img_dsc_t * ui_imgset_battery_[5] = {&ui_img_battery_0_png, &ui_img_battery_100_png, &ui_img_battery_25_png, &ui_img_battery_50_png, &ui_img_battery_75_png};

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui_Screen2_screen_init();
    ui_Screen3_screen_init();
    ui_Screen4_screen_init();
    ui_Screen5_screen_init();
    ui_Screen6_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen1);
}
