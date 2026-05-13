#include "page_main.h"
#include "ui_manager.h"
#include "app_data.h"
#include "input_key.h"
#include "lvgl.h"
#include "rtc.h"

#include <stdio.h>

/* ---------- auto-repeat tuning ---------- */
#define REPEAT_THRESHOLD   60   /* ticks before accelerating */
#define INTERVAL_SLOW      12   /* ~60ms: +1 each step */
#define INTERVAL_FAST       4   /* ~20ms: +10 each step */
#define REFRESH_INTERVAL   40   /* ~200ms: top-bar update */

static lv_obj_t *screen = NULL;
static lv_obj_t *label_time;
static lv_obj_t *label_wifi;
static lv_obj_t *label_temp;
static lv_obj_t *label_torque;
static lv_obj_t *label_title;
static lv_obj_t *label_hint_mid;

static uint8_t repeat_active = 0;
static int8_t  repeat_dir    = 0;
static uint8_t repeat_key    = 0;
static uint8_t repeat_tick   = 0;
static uint8_t refresh_tick  = 0;

/* ---------- helpers ---------- */

static void update_torque_display(void)
{
    char buf[12];
    snprintf(buf, sizeof(buf), "%ld", (long)app_data.target_torque);
    lv_label_set_text(label_torque, buf);
    lv_obj_align(label_torque, LV_ALIGN_CENTER, 0, -6);
}

static void update_top_bar(void)
{
    char buf[16];

    /* time */
    RTC_TimeTypeDef stime;
    HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN);
    snprintf(buf, sizeof(buf), "%02d:%02d", stime.Hours, stime.Minutes);
    lv_label_set_text(label_time, buf);

    /* wifi */
    if(app_data.wifi_connected)
    {
        lv_label_set_text(label_wifi, "WiFi");
        lv_obj_set_style_text_color(label_wifi, lv_color_hex(0x00C853), LV_PART_MAIN);
    }
    else
    {
        lv_label_set_text(label_wifi, "---");
        lv_obj_set_style_text_color(label_wifi, lv_color_hex(0x888888), LV_PART_MAIN);
    }

    /* temperature */
    snprintf(buf, sizeof(buf), "%dC", app_data.temperature);
    lv_label_set_text(label_temp, buf);
}

static void refresh_language(void)
{
    if(app_data.language == 0)
    {
        lv_label_set_text(label_title, "Target Torque");
        lv_label_set_text(label_hint_mid, "Key2:Menu");
    }
    else
    {
        lv_label_set_text(label_title, "\xe7\x9b\xae\xe6\xa0\x87\xe6\x89\xad\xe7\x9f\xa9");   /* 目标扭矩 */
        lv_label_set_text(label_hint_mid, "Key2:\xe8\x8f\x9c\xe5\x8d\x95");                  /* Key2:菜单 */
    }
}

/* ---------- page lifecycle ---------- */

static void page_main_on_enter(void)
{
    if(screen == NULL)
    {
        screen = lv_obj_create(NULL);
        lv_obj_set_size(screen, 128, 160);
        lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);
        lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);

        /* ---- top bar ---- */
        lv_obj_t *top_bar = lv_obj_create(screen);
        lv_obj_set_size(top_bar, 128, 24);
        lv_obj_set_pos(top_bar, 0, 0);
        lv_obj_set_style_bg_color(top_bar, lv_color_hex(0x1A3A5C), LV_PART_MAIN);
        lv_obj_set_style_border_width(top_bar, 0, LV_PART_MAIN);
        lv_obj_set_style_radius(top_bar, 0, LV_PART_MAIN);

        label_time = lv_label_create(top_bar);
        lv_obj_set_style_text_color(label_time, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_text_font(label_time, &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_align(label_time, LV_ALIGN_LEFT_MID, 4, 0);

        label_wifi = lv_label_create(top_bar);
        lv_obj_set_style_text_font(label_wifi, &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_align(label_wifi, LV_ALIGN_CENTER, 0, 0);

        label_temp = lv_label_create(top_bar);
        lv_obj_set_style_text_color(label_temp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_text_font(label_temp, &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_align(label_temp, LV_ALIGN_RIGHT_MID, -4, 0);

        /* ---- title ---- */
        label_title = lv_label_create(screen);
        lv_obj_set_style_text_color(label_title, lv_color_hex(0x666666), LV_PART_MAIN);
        lv_obj_set_style_text_font(label_title, &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 38);

        /* ---- big torque number ---- */
        label_torque = lv_label_create(screen);
        lv_obj_set_style_text_color(label_torque, lv_color_hex(0x1A3A5C), LV_PART_MAIN);
        lv_obj_set_style_text_font(label_torque, &lv_font_montserrat_20, LV_PART_MAIN);

        /* ---- unit ---- */
        lv_obj_t *unit = lv_label_create(screen);
        lv_label_set_text(unit, "N.m");
        lv_obj_set_style_text_color(unit, lv_color_hex(0x999999), LV_PART_MAIN);
        lv_obj_set_style_text_font(unit, &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_align(unit, LV_ALIGN_CENTER, 0, 30);

        /* ---- bottom hints ---- */
        lv_obj_t *hint_left = lv_label_create(screen);
        lv_label_set_text(hint_left, "Key1:-");
        lv_obj_set_style_text_color(hint_left, lv_color_hex(0x888888), LV_PART_MAIN);
        lv_obj_set_style_text_font(hint_left, &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_align(hint_left, LV_ALIGN_BOTTOM_LEFT, 4, -4);

        label_hint_mid = lv_label_create(screen);
        lv_obj_set_style_text_color(label_hint_mid, lv_color_hex(0x888888), LV_PART_MAIN);
        lv_obj_set_style_text_font(label_hint_mid, &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_align(label_hint_mid, LV_ALIGN_BOTTOM_MID, 0, -4);

        lv_obj_t *hint_right = lv_label_create(screen);
        lv_label_set_text(hint_right, "Key3:+");
        lv_obj_set_style_text_color(hint_right, lv_color_hex(0x888888), LV_PART_MAIN);
        lv_obj_set_style_text_font(hint_right, &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_align(hint_right, LV_ALIGN_BOTTOM_RIGHT, -4, -4);
    }

    repeat_active = 0;
    refresh_tick  = 0;

    refresh_language();
    update_top_bar();
    update_torque_display();
    lv_scr_load(screen);
}

static void page_main_on_exit(void)
{
    repeat_active = 0;
}

/* ---------- key handling ---------- */

static void start_repeat(int8_t dir, uint8_t key)
{
    repeat_active = 1;
    repeat_dir    = dir;
    repeat_key    = key;
    repeat_tick   = 0;
}

static void page_main_on_key(input_event_t event)
{
    switch(event)
    {
        case INPUT_KEY_UP:
            app_data.target_torque -= 1;
            if(app_data.target_torque < 0)
                app_data.target_torque = 0;
            update_torque_display();
            break;

        case INPUT_KEY_UP_LONG:
            app_data.target_torque -= 1;
            if(app_data.target_torque < 0)
                app_data.target_torque = 0;
            update_torque_display();
            start_repeat(-1, 0);  /* key 1 → KEY_ID_UP */
            break;

        case INPUT_KEY_OK:
            app_data.target_torque += 1;
            update_torque_display();
            break;

        case INPUT_KEY_OK_LONG:
            app_data.target_torque += 1;
            update_torque_display();
            start_repeat(1, 2);   /* key 3 → KEY_ID_OK */
            break;

        case INPUT_KEY_DOWN:
            ui_manager_switch_page(PAGE_MENU);
            break;

        default:
            break;
    }
}

/* ---------- update (called ~5ms) ---------- */

static void page_main_on_update(void)
{
    /* auto-repeat */
    if(repeat_active)
    {
        if(input_key_is_held(repeat_key))
        {
            repeat_tick++;

            uint8_t interval;
            int32_t delta;

            if(repeat_tick < REPEAT_THRESHOLD)
            {
                interval = INTERVAL_SLOW;
                delta    = 1;
            }
            else
            {
                interval = INTERVAL_FAST;
                delta    = 10;
            }

            if((repeat_tick % interval) == 0)
            {
                app_data.target_torque += delta * repeat_dir;
                if(app_data.target_torque < 0)
                    app_data.target_torque = 0;
                update_torque_display();
            }
        }
        else
        {
            repeat_active = 0;
        }
    }

    /* periodic top-bar refresh */
    refresh_tick++;
    if(refresh_tick >= REFRESH_INTERVAL)
    {
        refresh_tick = 0;
        update_top_bar();
    }
}

page_t page_main =
{
    .on_enter  = page_main_on_enter,
    .on_exit   = page_main_on_exit,
    .on_key    = page_main_on_key,
    .on_update = page_main_on_update,
};
