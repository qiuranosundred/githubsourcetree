#include "lv_test_ui.h"
#include <stdio.h>

/* ==================== 画面枚举 ==================== */
typedef enum {
    PAGE_HOME = 0,
    PAGE_COUNTER,
    PAGE_SLIDER,
    PAGE_SWITCH,
    PAGE_INFO,
    PAGE_COUNT
} page_t;

/* ==================== 全局变量 ==================== */
static lv_obj_t * screens[PAGE_COUNT];
static lv_obj_t * current_screen = NULL;
static lv_timer_t * auto_switch_timer = NULL;
static page_t current_page = PAGE_HOME;
static uint8_t page_index = 0;

/* 各页面控件 */
static lv_obj_t * counter_label;
static uint32_t counter_value = 0;
static lv_obj_t * counter_bar;      /* 保存进度条对象 */
static lv_obj_t * slider_label;
static lv_obj_t * slider;            /* 保存滑块对象 */
static lv_obj_t * switch_obj;        /* 保存开关对象 */
static lv_obj_t * switch_label;
static lv_obj_t * info_label;

/* 定时器回调函数声明 */
static void slider_label_update_cb(lv_timer_t * t);
static void switch_toggle_cb(lv_timer_t * t);

/* 定时器句柄 */
static lv_timer_t * slider_label_timer = NULL;
static lv_timer_t * switch_timer = NULL;
static lv_timer_t * update_counter_timer = NULL;

/* ==================== 页面切换函数 ==================== */
static void switch_to_page(page_t page)
{
    if(page >= PAGE_COUNT) return;
    if(screens[page] == NULL) return;

    /* 加载新页面 */
    lv_scr_load(screens[page]);
    current_screen = screens[page];
    current_page = page;
}

/* ==================== 自动切换回调 ==================== */
static void auto_switch_cb(lv_timer_t * timer)
{
    /* 切换到下一个页面 */
    page_index++;
    if(page_index >= PAGE_COUNT) {
        page_index = 0;
    }
    switch_to_page((page_t)page_index);
}

/* ==================== 滑块标签更新回调 ==================== */
static void slider_label_update_cb(lv_timer_t * t)
{
    if(current_page == PAGE_SLIDER && slider != NULL && slider_label != NULL) {
        int32_t val = lv_slider_get_value(slider);
        lv_label_set_text_fmt(slider_label, "Value: %ld", val);
    }
}

/* ==================== 开关自动切换回调 ==================== */
static void switch_toggle_cb(lv_timer_t * t)
{
    if(current_page == PAGE_SWITCH && switch_obj != NULL) {
        /* 切换开关状态 */
        if(lv_obj_has_state(switch_obj, LV_STATE_CHECKED)) {
            lv_obj_clear_state(switch_obj, LV_STATE_CHECKED);
            lv_label_set_text(switch_label, "State: OFF");
        } else {
            lv_obj_add_state(switch_obj, LV_STATE_CHECKED);
            lv_label_set_text(switch_label, "State: ON");
        }
    }
}

/* ==================== 创建页面1：主页 ==================== */
static void create_home_page(void)
{
    screens[PAGE_HOME] = lv_obj_create(NULL);
    lv_obj_t * scr = screens[PAGE_HOME];

    /* 背景 */
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1E1E2E), 0);
    lv_obj_set_style_bg_grad_color(scr, lv_color_hex(0x2D2D3D), 0);
    lv_obj_set_style_bg_grad_dir(scr, LV_GRAD_DIR_VER, 0);

    /* 垂直居中布局 */
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* 标题 */
    lv_obj_t * title = lv_label_create(scr);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x00D4FF), 0);
    lv_label_set_text(title, "Auto Demo");

    /* 大图标（模拟表盘） */
    lv_obj_t * arc = lv_arc_create(scr);
    lv_obj_set_size(arc, 80, 80);
    lv_arc_set_range(arc, 0, 100);
    lv_arc_set_value(arc, 75);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x00D4FF), 0);

    /* 中间文字 */
    lv_obj_t * center_label = lv_label_create(arc);
    lv_label_set_text(center_label, "75%");
    lv_obj_center(center_label);
    lv_obj_set_style_text_color(center_label, lv_color_hex(0xFFFFFF), 0);

    /* 副标题 */
    lv_obj_t * subtitle = lv_label_create(scr);
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0xAAAAAA), 0);
    lv_label_set_text(subtitle, "Auto Switching");

    /* 提示 */
    lv_obj_t * hint = lv_label_create(scr);
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(hint, lv_color_hex(0x666666), 0);
    lv_label_set_text(hint, "3s change page");
}

/* ==================== 创建页面2：计数器演示 ==================== */
static void create_counter_page(void)
{
    screens[PAGE_COUNTER] = lv_obj_create(NULL);
    lv_obj_t * scr = screens[PAGE_COUNTER];

    lv_obj_set_style_bg_color(scr, lv_color_hex(0x2D2D3D), 0);
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(scr, 15, 0);

    /* 标题 */
    lv_obj_t * title = lv_label_create(scr);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFF6B6B), 0);
    lv_label_set_text(title, "Auto Counter");

    /* 计数器数字（大号） */
    counter_label = lv_label_create(scr);
    lv_obj_set_style_text_font(counter_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(counter_label, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text_fmt(counter_label, "%lu", counter_value);

    /* 进度条 - 保存为全局变量 */
    counter_bar = lv_bar_create(scr);
    lv_obj_set_size(counter_bar, 100, 12);
    lv_bar_set_range(counter_bar, 0, 100);
    lv_bar_set_value(counter_bar, counter_value % 101, LV_ANIM_ON);

    /* 说明文字 */
    lv_obj_t * desc = lv_label_create(scr);
    lv_obj_set_style_text_font(desc, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(desc, lv_color_hex(0x888888), 0);
    lv_label_set_text(desc, "Counting auto");
}

/* ==================== 创建页面3：滑块演示 ==================== */
static void create_slider_page(void)
{
    screens[PAGE_SLIDER] = lv_obj_create(NULL);
    lv_obj_t * scr = screens[PAGE_SLIDER];

    lv_obj_set_style_bg_color(scr, lv_color_hex(0x3D3D4D), 0);
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(scr, 15, 0);

    /* 标题 */
    lv_obj_t * title = lv_label_create(scr);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x6BCB77), 0);
    lv_label_set_text(title, "Auto Slider");

    /* 滑块 - 保存为全局变量 */
    slider = lv_slider_create(scr);
    lv_obj_set_width(slider, 110);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);

    /* 滑块数值显示 */
    slider_label = lv_label_create(scr);
    lv_obj_set_style_text_font(slider_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(slider_label, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(slider_label, "Value: 50");

    /* 说明文字 */
    lv_obj_t * desc = lv_label_create(scr);
    lv_obj_set_style_text_font(desc, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(desc, lv_color_hex(0x888888), 0);
    lv_label_set_text(desc, "Running auto");

    /* 创建动画 - 直接更新滑块值 */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, slider);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_slider_set_value);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_set_time(&a, 4000);
    lv_anim_set_playback_time(&a, 4000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);

    /* 创建定时器更新标签（纯C方式） */
    if(slider_label_timer == NULL) {
        slider_label_timer = lv_timer_create(slider_label_update_cb, 50, NULL);
    }
}

/* ==================== 创建页面4：开关演示 ==================== */
static void create_switch_page(void)
{
    screens[PAGE_SWITCH] = lv_obj_create(NULL);
    lv_obj_t * scr = screens[PAGE_SWITCH];

    lv_obj_set_style_bg_color(scr, lv_color_hex(0x4D4D5D), 0);
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(scr, 20, 0);

    /* 标题 */
    lv_obj_t * title = lv_label_create(scr);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFA500), 0);
    lv_label_set_text(title, "Auto Switch");

    /* 开关 - 保存为全局变量 */
    switch_obj = lv_switch_create(scr);
    lv_obj_set_size(switch_obj, 50, 26);

    /* 开关状态显示 */
    switch_label = lv_label_create(scr);
    lv_obj_set_style_text_font(switch_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(switch_label, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(switch_label, "State: OFF");

    /* 创建定时器自动切换开关（纯C方式） */
    if(switch_timer == NULL) {
        switch_timer = lv_timer_create(switch_toggle_cb, 2000, NULL);
    }

    /* 说明文字 */
    lv_obj_t * desc = lv_label_create(scr);
    lv_obj_set_style_text_font(desc, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(desc, lv_color_hex(0x888888), 0);
    lv_label_set_text(desc, "Blinking auto");
}

/* ==================== 创建页面5：系统信息 ==================== */
static void create_info_page(void)
{
    screens[PAGE_INFO] = lv_obj_create(NULL);
    lv_obj_t * scr = screens[PAGE_INFO];

    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1A1A2E), 0);
    lv_obj_set_style_bg_grad_color(scr, lv_color_hex(0x16213E), 0);
    lv_obj_set_style_bg_grad_dir(scr, LV_GRAD_DIR_VER, 0);
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(scr, 8, 0);

    /* 标题 */
    lv_obj_t * title = lv_label_create(scr);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x00D4FF), 0);
    lv_label_set_text(title, "System Info");

    /* 信息文本 */
    info_label = lv_label_create(scr);
    lv_obj_set_style_text_font(info_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(info_label, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(info_label, "STM32\nLVGL v8\n128x160\nST7735S");
    lv_label_set_recolor(info_label, true);
    lv_obj_set_style_text_align(info_label, LV_TEXT_ALIGN_CENTER, 0);
}

/* ==================== 更新任务（自动增加计数器） ==================== */
static void update_counter_cb(lv_timer_t * timer)
{
    /* 每0.5秒增加计数器 */
    counter_value++;
    if(counter_value > 999) counter_value = 0;

    /* 更新当前页面的计数器显示（如果是在计数器页面） */
    if(current_page == PAGE_COUNTER && counter_label != NULL) {
        lv_label_set_text_fmt(counter_label, "%lu", counter_value);

        /* 同步更新进度条 */
        if(counter_bar != NULL) {
            lv_bar_set_value(counter_bar, counter_value % 101, LV_ANIM_ON);
        }
    }
}

/* ==================== 主函数 ==================== */
void create_test_ui(void)
{
    /* 创建所有页面 */
    create_home_page();
    create_counter_page();
    create_slider_page();
    create_switch_page();
    create_info_page();

    /* 默认显示主页 */
    switch_to_page(PAGE_HOME);

    /* 创建自动切换定时器（3秒切换一次） */
    auto_switch_timer = lv_timer_create(auto_switch_cb, 3000, NULL);

    /* 创建计数器更新定时器（0.5秒更新一次） */
    update_counter_timer = lv_timer_create(update_counter_cb, 500, NULL);
}
