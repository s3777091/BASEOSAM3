#ifndef SCREEN_H
#define SCREEN_H
#define image_arr_len 3
#define frame_arr_len 32
// default qemu display resolution
#define MAX_DISPLAY_WIDTH 1024
#define MAX_DISPLAY_HEIGHT 768

#define SCROLL_STEP 10

void scrollImage(char c);
void printImage(int x1, int y1, int image_pointer);
void displayVideo();

void wait_msec(unsigned int n);


#endif // SCREEN_H