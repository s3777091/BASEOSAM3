#define image_arr_len 3
#define frame_arr_len 32
// default qemu display resolution
#define MAX_DISPLAY_WIDTH 1024
#define MAX_DISPLAY_HEIGHT 768

#define SCROLL_STEP 10

#define BLACK   0x00000000
#define WHITE   0xFFFFFFFF
#define RED     0xFFFF0000
#define BLUE    0xFF0000FF
#define LIME    0xFF00FF00
#define YELLOW  0xFFFFFF00
#define CYAN    0xFF00FFFF
#define MAGENTA 0xFFFF00FF
#define GREY    0xFF808080
#define MAROON  0xFF800000
#define OLIVE   0xFF808000
#define GREEN   0xFF008000
#define PURPLE  0xFF800080
#define TEAL    0xFF008080
#define NAVY    0xFF000080

void change_display(char c);
void printImage(int x1, int y1, int image_pointer);
void displayVideo();
void display_keymap_bar();
void displayInfo();

void wait_msec(unsigned int n);