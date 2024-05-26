#include "./lib/screen.h"
#include "data/imagedata.h"
#include "data/videodata.h"
#include "lib/font.h"

// frame resolution to display silde
const int frame_width = MAX_DISPLAY_WIDTH;
const int frame_height = 730;

// Array of all bitmaps for convenience. 
const unsigned long* image_epd_bitmap[] = {
	epd_bitmap_image1,
    epd_bitmap_image2
};

const unsigned int image_width[] = {
    755,
    755,
    755
};

const unsigned int image_height[] = {
    450,
    450,
    450
};

const unsigned long* frame_epd_bitmap[] = {
	epd_bitmap_frame[15]
};

const unsigned int video_width = 615;
const unsigned int video_height = 480;

int media_pointer = 0; // default start with image no.0; media_pointer = -1 means displaying video
int image_x = 0;
int image_y = 0;

void change_display(char c) {
    if (c == 'a') {
        if (media_pointer == -1) 
            media_pointer = image_arr_len - 1;
        else 
            media_pointer--;
        image_x = 0;
        image_y = 0;

        // change background to black
        drawRectARGB32(0,0,frame_width - 1,frame_height - 1,BLACK,1);

        if (media_pointer == -1) 
            displayVideo();
        else
            printImage(0, 0, media_pointer);
    } 
    else if (c == 'd') {
        if (media_pointer == image_arr_len - 1) 
            media_pointer = -1;
        else 
            media_pointer++; 
        image_x = 0;
        image_y = 0;

        // change background to black
        drawRectARGB32(0,0,frame_width - 1,frame_height - 1,BLACK,1);
        
        if (media_pointer == -1) 
            displayVideo();
        else
            printImage(0, 0, media_pointer);
    } 
    else if (c == 's') {
        if (media_pointer >= 0) {
            image_y -= SCROLL_STEP;
            if (image_y < frame_height - image_height[media_pointer])
                image_y = frame_height - image_height[media_pointer];
            printImage(image_x, image_y, media_pointer);
        }   
    }
    else if (c == 'w') {
        if (media_pointer >= 0) {
            image_y += SCROLL_STEP;
            if (image_y > 0)
                image_y = 0;
            printImage(image_x, image_y, media_pointer);
        }    
    }
    else if (c == 'r') {
        if (media_pointer == -1) 
            displayVideo();
    }
}

void printImage(int image_x, int image_y, int media_pointer) {
    int i = 0;
    int w = image_width[media_pointer];
    int h = image_height[media_pointer];  
    
    // print image
    for (int y = image_y; y < (image_y + h); y++)
        for (int x = image_x; x < (image_x + w); x++) {
            if (x >= 0 && y >= 0 && x < frame_width && y < frame_height)
                drawPixelARGB32(x, y, image_epd_bitmap[media_pointer][i]);
            i++;
        }
}

void displayVideo() {
    int frame_pointer = 0;
                    
    while (frame_pointer < frame_arr_len) {
        int i = 0;
        for (int y = 0; y < video_height; y++)
            for (int x = 0; x < video_width; x++) {
                if (x >= 0 && y >= 0 && x < frame_width && y < frame_height)
                    drawPixelARGB32(x, y, frame_epd_bitmap[frame_pointer][i]);
                i++;
            }
        frame_pointer++;
        wait_msec(70);
    }
}

void display_keymap_bar() {
    const unsigned int FONT_SCALE = 2;
    char* keymap1 = " [w],[s]:scroll | [a],[d]:switch slide | [r]:replay video";
    char* keymap2 = " [p]:play game | [i]:info ";
    drawString(keymap1, 4, frame_height + 4, WHITE, FONT_SCALE);
    drawString(keymap2, 4, frame_height + 4 + FONT_SCALE * FONT_HEIGHT, WHITE, FONT_SCALE);
}

void displayInfo() {
    drawRectARGB32(0,0,MAX_DISPLAY_WIDTH,MAX_DISPLAY_HEIGHT,BLACK,1);
    const char* info[10] = {
        "EEET2490",
        " ",
        "Snake Game",
        " ",
        "Developed by group 15 :",
        "    Duong Minh Nam s3878202",
        "    Huynh Dac Tan Dat s3777091",
        "    Lai Phuc Anh s3878693",
        "    Phan Anh"
    };
    const int FONT_SCALE = 3;
    int x = 30;
    int y = 30;
    for (int i = 0; i < 10; i++) {
        drawString(info[i], x, y, WHITE, FONT_SCALE);
        y += FONT_HEIGHT * FONT_SCALE;
    }
    drawString("Press [b] to go back!", x, frame_height + 4, WHITE, FONT_SCALE - 1);
}


void wait_msec(unsigned int n) {
    register unsigned long f, t, r, expiredTime;
    // Get the current counter frequency (Hz)
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // Read the current counter value
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // Calculate expire value for counter
    expiredTime = t + ( (f/1000)*n )/1000;
    do {
        asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
    } while(r < expiredTime);
}