#include "page_menu.h"
#include "ui_manager.h"
#include "app_data.h"
#include "lvgl.h"

/* ==================== menu tree types ==================== */

typedef enum {
    MENU_SUB,
    MENU_LEAF,
    MENU_BACK,
} menu_type_t;

struct menu_level;

typedef struct {
    const char            *label_en;
    const char            *label_zh;
    menu_type_t            type;
    const struct menu_level *children;
    int                   *value_ptr;
    int                    value;
} menu_item_t;

typedef struct menu_level {
    const char      *title_en;
    const char      *title_zh;
    const menu_item_t *items;
    uint8_t          count;
} menu_level_t;

/* ==================== sub-menus ==================== */

static const menu_item_t items_lang[] = {
    {"English",  "\xe8\x8b\xb1\xe6\x96\x87",  MENU_LEAF, NULL, &app_data.language, 0}, /* 英文 */
    {"\xe4\xb8\xad\xe6\x96\x87", "\xe4\xb8\xad\xe6\x96\x87", MENU_LEAF, NULL, &app_data.language, 1}, /* 中文 */
    {"[BACK]",   "\xe8\xbf\x94\xe5\x9b\x9e",  MENU_BACK}, /* 返回 */
};

static const menu_level_t level_lang = {
    .title_en = "Language",
    .title_zh = "\xe8\xaf\xad\xe8\xa8\x80",  /* 语言 */
    .items    = items_lang,
    .count    = sizeof(items_lang) / sizeof(items_lang[0]),
};

/* ==================== main menu ==================== */

static const menu_item_t items_main[] = {
    {"01 MODE SET",      "01 \xe6\xa8\xa1\xe5\xbc\x8f\xe9\x80\x89\xe6\x8b\xa9",   MENU_SUB, NULL},        /* 模式选择 */
    {"02 PRESET SET",    "02 \xe9\xa2\x84\xe8\xae\xbe\xe5\x80\xbc\xe8\xae\xbe\xe5\xae\x9a", MENU_SUB, NULL}, /* 预设值设定 */
    {"03 PARA SET",      "03 \xe5\x8f\x82\xe6\x95\xb0\xe8\xae\xbe\xe7\xbd\xae",   MENU_SUB, NULL},        /* 参数设置 */
    {"04 FOWARD SPEED",  "04 \xe6\x8b\xa7\xe7\xb4\xa7\xe9\x80\x9f\xe5\xba\xa6",   MENU_SUB, NULL},        /* 拧紧速度 */
    {"05 BOLT INFO",     "05 \xe5\xb7\xa5\xe4\xbd\x8d\xe4\xb8\x8e\xe8\x9e\xba\xe6\xa0\x93\xe4\xbf\xa1\xe6\x81\xaf", MENU_SUB, NULL}, /* 工位与螺栓信息 */
    {"06 ADMIN",         "06 \xe9\xab\x98\xe7\xba\xa7",           MENU_SUB, NULL},  /* 高级 */
    {"07 BOLT CHART",    "07 \xe8\x9e\xba\xe6\xa0\x93\xe5\x8c\xb9\xe9\x85\x8d",   MENU_SUB, NULL},        /* 螺栓匹配 */
    {"08 SAFE LIMIT",    "08 \xe5\xae\x89\xe5\x85\xa8\xe9\x99\x90\xe5\x88\xb6",   MENU_SUB, NULL},        /* 安全限制 */
    {"09 WIFI SET",      "09 WIFI\xe8\xae\xbe\xe7\xbd\xae",      MENU_SUB, NULL},  /* WIFI设置 */
    {"10 BLE SET",       "10 BLE\xe8\xae\xbe\xe7\xbd\xae",        MENU_SUB, NULL},  /* BLE设置 */
    {"11 LOG",           "11 \xe8\xae\xb0\xe5\xbd\x95",           MENU_SUB, NULL},  /* 记录 */
    {"12 LANGUAGE",      "12 \xe8\xaf\xad\xe8\xa8\x80",           MENU_SUB, &level_lang}, /* 语言 */
    {"13 INFO",          "13 \xe4\xbf\xa1\xe6\x81\xaf",           MENU_LEAF, NULL},  /* 信息 */
    {"14 [EXIT]",        "14 [\xe9\x80\x80\xe5\x87\xba]",         MENU_BACK},        /* 退出 */
};

static const menu_level_t level_main = {
    .title_en = "Menu",
    .title_zh = "\xe8\x8f\x9c\xe5\x8d\x95",  /* 菜单 */
    .items    = items_main,
    .count    = sizeof(items_main) / sizeof(items_main[0]),
};

/* ==================== navigation stack ==================== */

#define STACK_MAX  4
static const menu_level_t *stack_level[STACK_MAX];
static int                 stack_sel[STACK_MAX];
static int                 stack_depth = 0;

static const menu_level_t *cur_level;
static int                 cur_sel;

/* ==================== LVGL objects ==================== */

#define VIEW_H  134
#define VIEW_Y  26
#define ITEM_H  22

static lv_obj_t *screen      = NULL;
static lv_obj_t *title_label = NULL;
static lv_obj_t *menu_cont   = NULL;
static lv_obj_t *item_objs[20];
static lv_obj_t *item_labels[20];
static uint8_t   item_count  = 0;

/* pick label by language */
static const char *label_text(const menu_item_t *item)
{
    return (app_data.language == 1) ? item->label_zh : item->label_en;
}

static const char *title_text(const menu_level_t *level)
{
    return (app_data.language == 1) ? level->title_zh : level->title_en;
}

/* ==================== helpers ==================== */

static void scroll_to_sel(void)
{
    lv_coord_t top = cur_sel * ITEM_H;
    lv_coord_t bot = top + ITEM_H;
    lv_coord_t y   = lv_obj_get_scroll_y(menu_cont);

    if(top < y)
        lv_obj_scroll_to_y(menu_cont, top, LV_ANIM_ON);
    else if(bot > y + VIEW_H)
        lv_obj_scroll_to_y(menu_cont, bot - VIEW_H, LV_ANIM_ON);
}

static void rebuild_items(void)
{
    uint8_t i;

    for(i = 0; i < item_count; i++)
    {
        if(item_objs[i]) lv_obj_del(item_objs[i]);
        item_objs[i]   = NULL;
        item_labels[i] = NULL;
    }

    item_count = cur_level->count;
    if(item_count > 20) item_count = 20;

    for(i = 0; i < item_count; i++)
    {
        item_objs[i] = lv_obj_create(menu_cont);
        lv_obj_set_size(item_objs[i], 112, ITEM_H - 2);
        lv_obj_set_pos(item_objs[i], 5, i * ITEM_H);
        lv_obj_set_style_border_width(item_objs[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_width(item_objs[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(item_objs[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(item_objs[i], 3, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(item_objs[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        item_labels[i] = lv_label_create(item_objs[i]);
        lv_label_set_text(item_labels[i], label_text(&cur_level->items[i]));
        lv_obj_center(item_labels[i]);
        lv_obj_set_style_text_font(item_labels[i], &lv_font_montserrat_14, LV_PART_MAIN);
    }

    lv_obj_scroll_to_y(menu_cont, 0, LV_ANIM_OFF);
}

static void update_selection(void)
{
    uint8_t i;
    for(i = 0; i < item_count; i++)
    {
        if(i == cur_sel)
        {
            lv_obj_set_style_bg_color(item_objs[i], lv_color_hex(0x2195F6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(item_labels[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        }
        else
        {
            lv_obj_set_style_bg_color(item_objs[i], lv_color_hex(0xEEEEEE), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(item_labels[i], lv_color_hex(0x333333), LV_PART_MAIN);
        }
    }
    scroll_to_sel();
}

static void refresh_menu_text(void)
{
    uint8_t i;
    lv_label_set_text(title_label, title_text(cur_level));
    lv_obj_center(title_label);
    for(i = 0; i < item_count; i++)
    {
        lv_label_set_text(item_labels[i], label_text(&cur_level->items[i]));
        lv_obj_center(item_labels[i]);
    }
}

/* ==================== navigation ==================== */

static void push_level(const menu_level_t *level)
{
    if(stack_depth < STACK_MAX)
    {
        stack_level[stack_depth] = cur_level;
        stack_sel[stack_depth]   = cur_sel;
        stack_depth++;
    }
    cur_level = level;
    cur_sel   = 0;
    rebuild_items();
    update_selection();
}

static void pop_level(void)
{
    if(stack_depth > 0)
    {
        stack_depth--;
        cur_level = stack_level[stack_depth];
        cur_sel   = stack_sel[stack_depth];
    }
    /* refresh text in case language changed in sub-menu */
    refresh_menu_text();
    update_selection();
    lv_obj_scroll_to_y(menu_cont, cur_sel * ITEM_H, LV_ANIM_OFF);
}

/* ==================== lifecycle ==================== */

static void page_menu_on_enter(void)
{
    if(screen == NULL)
    {
        screen = lv_obj_create(NULL);
        lv_obj_set_size(screen, 128, 160);
        lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);

        lv_obj_t *bar = lv_obj_create(screen);
        lv_obj_set_size(bar, 128, 26);
        lv_obj_set_pos(bar, 0, 0);
        lv_obj_set_style_bg_color(bar, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        title_label = lv_label_create(bar);
        lv_obj_set_style_text_color(title_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_text_font(title_label, &lv_font_montserrat_14, LV_PART_MAIN);

        menu_cont = lv_obj_create(screen);
        lv_obj_set_size(menu_cont, 128, VIEW_H);
        lv_obj_set_pos(menu_cont, 0, VIEW_Y);
        lv_obj_set_style_bg_opa(menu_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(menu_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(menu_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_scrollbar_mode(menu_cont, LV_SCROLLBAR_MODE_AUTO);
        lv_obj_set_scroll_dir(menu_cont, LV_DIR_VER);
    }

    stack_depth = 0;
    cur_level   = &level_main;
    cur_sel     = 0;
    lv_label_set_text(title_label, title_text(cur_level));
    lv_obj_center(title_label);
    rebuild_items();
    update_selection();
    lv_scr_load(screen);
}

static void page_menu_on_exit(void)
{
}

/* ==================== key ==================== */

static void page_menu_on_key(input_event_t event)
{
    const menu_item_t *item;

    switch(event)
    {
        case INPUT_KEY_UP:  /* Key1: down / next */
            cur_sel++;
            if(cur_sel >= cur_level->count)
                cur_sel = 0;
            update_selection();
            break;

        case INPUT_KEY_OK:  /* Key3: up / prev */
            if(cur_sel == 0)
                cur_sel = cur_level->count - 1;
            else
                cur_sel--;
            update_selection();
            break;

        case INPUT_KEY_DOWN:  /* Key2: confirm */
            item = &cur_level->items[cur_sel];

            switch(item->type)
            {
                case MENU_SUB:
                    if(item->children)
                        push_level(item->children);
                    break;

                case MENU_LEAF:
                    if(item->value_ptr)
                        *item->value_ptr = item->value;
                    pop_level();
                    break;

                case MENU_BACK:
                    if(stack_depth == 0)
                        ui_manager_switch_page(PAGE_MAIN);
                    else
                        pop_level();
                    break;
            }
            break;

        default:
            break;
    }
}

static void page_menu_on_update(void)
{
}

page_t page_menu =
{
    .on_enter  = page_menu_on_enter,
    .on_exit   = page_menu_on_exit,
    .on_key    = page_menu_on_key,
    .on_update = page_menu_on_update,
};
