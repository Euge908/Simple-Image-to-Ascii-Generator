#include "include/FreeImage.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//NOTE: This code assumes that the bit depth is 24 bits

//function prototypes
long map(long x, long in_min, long in_max, long out_min, long out_max);
void simplifyImage(uint8_t* inputColor, int m);

char density[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\" ^ `\'."; //fine-ness could be improved by adding more characters



int main() {

	//too lazy to manually code the functs in opening images
	FIBITMAP* img = FreeImage_Load(FIF_JPEG, "assets/minions.jpg");
	int img_width = 200, img_height = FreeImage_GetHeight(img) * ((float) img_width/ FreeImage_GetWidth(img));
	img = FreeImage_Rescale(img, img_width, img_height);

	

	FreeImage_FlipVertical(img);
	uint8_t* img_data = FreeImage_GetBits(img);

	printf("width: %d, height: %d\n\n", FreeImage_GetWidth(img), FreeImage_GetHeight(img));

	for (int i = 0; i < img_height; i++) {
		for (int j = 0; j < img_width; j++) {
			uint8_t r = img_data[3 * img_width * i + 3 * j + 2];
			uint8_t g = img_data[3 * img_width * i + 3 * j + 1];
			uint8_t b = img_data[3 * img_width * i + 3 * j + 0];


			
			simplifyImage(&r, 10);
			simplifyImage(&g, 10);
			simplifyImage(&b, 10);
			

			uint32_t ave = (r + g + b) / 3;

			uint32_t density_len = sizeof(density) / sizeof(char);
			uint32_t charIndex = map(ave, 0, 255, density_len, 0);
			//printf("%d ", (r<< 16) | (g << 8) | b);
			printf("%c", density[charIndex]);
		}

		printf("\n");

	}



	return 0;
}

//from arduino map function reference 
long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void simplifyImage(uint8_t* inputColor, int m) {
	
	//this approach will simplify 0-255 into m levels
	//will yield a total of m^3 rgb color values

	*inputColor = 255 * map(*inputColor, 0, 255, 0, m)/ m;

}
