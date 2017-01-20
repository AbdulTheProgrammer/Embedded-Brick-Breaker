/* GIMP RGB C-Source image dump (ball.c) */

#define BALL_WIDTH (24)
#define BALL_HEIGHT (24)
#define BALL_BYTES_PER_PIXEL (3) /* 3:RGB, 4:RGBA */
#define BALL_PIXEL_DATA ((unsigned char*) BALL_pixel_data)
static const unsigned char BALL_pixel_data[24 * 24 * 3 + 1] =
("\377\377\377\377\377\377\377\377\377\377\377\377\377\377\337\377\070\376\317"
 "\373\206\371A\370A\370\206\371\317\373\070\376\337\377\377\377\377\377\377"
 "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
 "\337\377\262\374\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\262\374\337\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
 "\377\377\377\333\376a\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370a\370\333\376\377\377\377\377\377\377\377\377\377"
 "\377\377\377\377\377\070\376\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\070\376\377\377\377\377\377"
 "\377\377\377\377\377\333\376\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\333\376\377\377"
 "\377\377\377\377\337\377a\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000"
 "\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370a\370\337"
 "\377\377\377\377\377\262\374\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\262"
 "\374\377\377\337\377\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000"
 "\370\337\377\070\376\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000"
 "\370\070\376\317\373\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000"
 "\370\317\373\206\371\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000"
 "\370\206\371A\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000"
 "\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "A\370A\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370A\370\206"
 "\371\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\206\371\317\373"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000"
 "\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\317\373\070\376"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000"
 "\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\070\376\337\377"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000"
 "\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\337\377\377\377"
 "\262\374\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\262\374\377\377\377"
 "\377\337\377a\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000"
 "\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370a\370\337\377\377\377"
 "\377\377\377\377\333\376\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000"
 "\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\333\376\377\377"
 "\377\377\377\377\377\377\377\377\070\376\000\370\000\370\000\370\000\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\070\376\377\377"
 "\377\377\377\377\377\377\377\377\377\377\377\377\333\376a\370\000\370\000\370"
 "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370a\370\333\376"
 "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\337"
 "\377\262\374\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\262"
 "\374\337\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
 "\377\377\377\377\377\377\377\377\337\377\070\376\317\373\206\371A\370A\370"
 "\206\371\317\373\070\376\337\377\377\377\377\377\377\377\377\377\377\377\377"
 "\377\377\377");
