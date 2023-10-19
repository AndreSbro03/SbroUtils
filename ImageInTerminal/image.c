#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#define ASCII " .:-=+*#%@"
#define DIMH 40
#define DIMW 100

#define ASCII_LEN (sizeof(ASCII) - 1)/sizeof(char)

float lerp(float a, float b, float f) 
{
    return (a * (1.0 - f)) + (b * f);
}

int main(int argc, char *argv[]){

    char * nomefile = argv[1];
    int width, height, channels;

    unsigned char *img = stbi_load(nomefile, &width , &height, &channels, 0);

     if(img == NULL){
        printf("Error loading the file\n");
        exit(1);
    }

    size_t img_size = width * height * channels;
    int gray_channels = channels == 4 ? 2 : 1;
    size_t gray_img_size = width * height * gray_channels;
    unsigned char *gray_img = malloc(gray_img_size);
    
    if(gray_img == NULL){
        printf("Error creating gray imgage");
        exit(1);
    }

    int i = 0;
    int dh_or = height / DIMH;
    int dw_or = width / DIMW;
    int dh = dh_or;
    int dw = dw_or;

    for(unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels){
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
        if(channels == 4){
            *(pg + 1) = *(p + 3);
        }
       

        if(dh == 0){
            if(dw == 0){
                char ch = ASCII[(int) lerp(0, ASCII_LEN, *pg/256.0)];
                printf("%c", ch);

                //printf("%d -> %d\n", i,  width * channels);
                dw = dw_or;
            }
            if(i % width * channels == 0){
                printf("\n");
                dh = dh_or;
            }

            dw = dw - 1;
                    
        }     
        if(i % width * channels == 0){
                dh = dh - 1;
            }

        i++;

    }

    printf("Load an image with a whith of %dpx, a height of %dpx, and %d channels\n", width , height, channels);

    stbi_write_jpg("inter_bw.jpg", width, height, gray_channels, gray_img, 100);
    stbi_image_free(img);
    free(gray_img);

    return 0;
}