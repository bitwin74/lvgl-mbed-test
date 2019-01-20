#include "mbed.h"
#include "lvgl/lvgl.h"
#include "drivers/display/ST7565.h"

DigitalOut myled(LED1);
Ticker     one_ms;

void tick_cb() {
	lv_tick_inc(1);
}

void draw_hello_world(void) {
	/*Create a Label on the currently active screen*/
	lv_obj_t *label1 = lv_label_create(lv_scr_act(), NULL);

	/*Modify the Label's text*/
	lv_label_set_text(label1, "Hello world!");

	/* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
	lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
}

int main() {
	lv_init(); // init lvgl

	one_ms.attach(&tick_cb, 0.001); // setup ticker @ 1ms interval

	st7565_init(); // init display driver

	lv_disp_drv_t disp_drv;      /*Descriptor of a display driver*/
	lv_disp_drv_init(&disp_drv); /*Basic initialization*/

	/*Set up the functions to access to your display*/
	disp_drv.disp_flush = st7565_flush; /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/
	disp_drv.disp_fill  = st7565_fill;  /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
	disp_drv.disp_map   = st7565_map;   /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/

	lv_disp_drv_register(&disp_drv);

	draw_hello_world();

	while (1) {
		lv_task_handler();
		LV_DRV_DELAY_MS(5); // 5ms
	}
}
