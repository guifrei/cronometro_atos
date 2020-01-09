//mencoder mf://f*.png -mf fps=1 -ovc copy -oac copy -o output.avi
//ffmpeg -i output.avi -c:v libx264 -profile:v baseline -level 3.0 -pix_fmt yuv420p working.mp4

//Extrai quadro a quadro
//ffmpeg -i ~/WORSHIP\ COUNTDOWN\ 5\ MIN\ \(CONTAGEM\ REGRESSIVA\ -\ CULTO\ JOVEM\)-iTa_cDVN1xw.webm -r 24 image-%04d.bmp
//ffmpeg -i VID_20200108_210308994.mp4 -r 24 leticia_%03d.bmp

/* Bring in gd library functions */
#include "gd.h"
/* Bring in standard I/O so we can output the PNG to a file */
#include <stdio.h>
#include <math.h>

int w = 20;
int npts = 500;

void drawColor(gdImagePtr bmpMainFrame, int color, int shift) {

	for (int j = shift * w; j < (shift + 1) * w; j++) {
		double x1 = 650.0 - j;
		double x2 = 1300.0 + j;
		double y1 = -640.0;
		double y2 = -550.0;

		double xr = (x1 + x2) / 2.0;
		double yr = (y1 + y2) / 2.0;

		double r2 = (x1 - xr) * (x1 - xr) + (y1 - yr) * (y1 - yr);
		double r = sqrt(r2);
		double theta2 = atan((y2 - yr) / (x2 - xr));
		double theta1 = theta2 + M_PI;
		for (int i = 0; i <= npts; i++) {
			double theta = (theta2 - theta1) / (double)npts * (double) i + theta1;
			double x = xr + r * cos(theta);
			double y = yr + r * sin(theta);
			gdImageSetPixel(bmpMainFrame, x, -y, color);
		}
	}
}

int main() {

	gdImagePtr bmpMainFrame = gdImageCreateTrueColor(1920, 1080);

	FILE* bmpFrame = fopen("leticia_080.bmp", "rb");
	gdImagePtr frame = gdImageCreateFromBmp(bmpFrame);

	gdImageCopy(bmpMainFrame, frame, 0, 0, 0, 0, 1920, 1080);

	int black = gdImageColorAllocate(bmpMainFrame, 0, 0, 0);
	int white = gdImageColorAllocate(bmpMainFrame, 255, 255, 255);
	gdImageFilledEllipse(bmpMainFrame, 1300, 550, 15, 15, black);
	gdImageFilledEllipse(bmpMainFrame, 650, 640, 15, 15, white);

	int red = gdImageColorAllocate(bmpMainFrame, 255, 0, 0);
	drawColor(bmpMainFrame, red, 0);
	int orange = gdImageColorAllocate(bmpMainFrame, 255, 165, 0);
	drawColor(bmpMainFrame, orange, 1);
	int yellow = gdImageColorAllocate(bmpMainFrame, 255, 255, 0);
	drawColor(bmpMainFrame, yellow, 2);
	int green = gdImageColorAllocate(bmpMainFrame, 0, 255, 0);
	drawColor(bmpMainFrame, green, 3);
	int blue = gdImageColorAllocate(bmpMainFrame, 0, 0, 255);
	drawColor(bmpMainFrame, blue, 4);
	int indigo = gdImageColorAllocate(bmpMainFrame, 75, 0, 130);
	drawColor(bmpMainFrame, indigo, 5);
	int violet = gdImageColorAllocate(bmpMainFrame, 79, 47, 79);
	drawColor(bmpMainFrame, violet, 6);

	FILE* mainFrame = fopen("teste.bmp", "w");
	gdImageBmp(bmpMainFrame, mainFrame, 0);

	gdImageDestroy(bmpMainFrame);
	fclose(mainFrame);

	gdImageDestroy(frame);
	fclose(bmpFrame);

	return 0;

	printf("\n");
	system("mencoder mf://f*.png -mf fps=1 -ovc copy -oac copy -o output.avi");
	system("ffmpeg -y -i output.avi -c:v libx264 -profile:v baseline -level 3.0 -pix_fmt yuv420p working.mp4");
}
