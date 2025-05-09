// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Button1 = lv_btn_create(ui_Screen1);
    lv_obj_set_width(ui_Button1, 100);
    lv_obj_set_height(ui_Button1, 50);
    lv_obj_set_x(ui_Button1, -10);
    lv_obj_set_y(ui_Button1, -10);
    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label = lv_label_create(ui_Button1);
    lv_obj_set_width(ui_Label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label, "CAN TX");

    lv_obj_add_event_cb(ui_Button1, ui_event_Button1, LV_EVENT_ALL, NULL);

    ui_LogTextArea = lv_textarea_create(ui_Screen1);
    lv_obj_set_pos(ui_LogTextArea, 0, 280);
    lv_obj_set_size(ui_LogTextArea, 800, 200);
    lv_obj_add_flag(ui_LogTextArea, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_scrollbar_mode(ui_LogTextArea, LV_SCROLLBAR_MODE_AUTO);

}
