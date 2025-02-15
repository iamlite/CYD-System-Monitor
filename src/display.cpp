#include "display.h"
#include <Arduino.h>

TFT_eSPI tft = TFT_eSPI();
lv_disp_draw_buf_t draw_buf;
lv_color_t *buf1;
lv_color_t *buf2;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.pushImageDMA(area->x1, area->y1, w, h, (uint16_t *)color_p);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

void init_display()
{
    tft.begin();
    tft.setRotation(3);
    tft.initDMA();
    tft.fillScreen(TFT_BLACK);

    // Allocate display buffers
    extern const uint16_t screenHeight;
    extern const uint16_t screenWidth;
    buf1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenHeight * 10, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    buf2 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenHeight * 10, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screenHeight * 10);

    // Initialize display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenHeight;  // or actual horizontal resolution
    disp_drv.ver_res = screenWidth;   // or actual vertical resolution
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
}