//mencoder mf://f*.png -mf fps=1 -ovc copy -oac copy -o output.avi
//ffmpeg -i output.avi -c:v libx264 -profile:v baseline -level 3.0 -pix_fmt yuv420p working.mp4

/* Bring in gd library functions */
#include "gd.h"

/* Bring in standard I/O so we can output the PNG to a file */
#include <stdio.h>
#include <math.h>

double sq = sqrt(2.0) / 2.0 * 10.0;

int codes[] = { 119, 18, 93, 91, 58, 107, 111, 82, 127, 123 };

int tmax = 5 * 60;

void drawHorizontal(double xref, double yref, gdImagePtr im) {
	int white = gdImageColorAllocate(im, 255, 255, 255);
	int black = gdImageColorAllocate(im, 0, 0, 0);

	gdPointPtr points = (gdPointPtr) calloc(6, sizeof(gdPoint));
	points[0].x = xref;
	points[0].y = 5 + yref;

	points[1].x = 5 + xref;
	points[1].y = yref;

	points[2].x = 95 + xref;
	points[2].y = yref;

	points[3].x = 100 + xref;
	points[3].y = 5 + yref;

	points[4].x = 95 + xref;
	points[4].y = 10 + yref;

	points[5].x = 5 + xref;
	points[5].y = 10 + yref;

	gdImageFilledPolygon(im, points, 6, black);
	free(points);
}

void drawVertical(double xref, double yref, gdImagePtr im) {
	int white = gdImageColorAllocate(im, 255, 255, 255);
	int black = gdImageColorAllocate(im, 0, 0, 0);

	gdPointPtr points = (gdPointPtr) calloc(6, sizeof(gdPoint));
	points[0].x = xref;
	points[0].y = 5 + yref;

	points[1].x = 5 + xref;
	points[1].y = yref;

	points[2].x = 10 + xref;
	points[2].y = 5 + yref;

	points[3].x = 10 + xref;
	points[3].y = 75 + yref;

	points[4].x = 5 + xref;
	points[4].y = 80 + yref;

	points[5].x = xref;
	points[5].y = 75 + yref;

	gdImageFilledPolygon(im, points, 6, black);
	free(points);
}

void drawNumber(double shift, gdImagePtr im, int number) {
	int code = codes[number];
	int a = (code & 0x0040) >> 6;
	int b = (code & 0x0020) >> 5;
	int c = (code & 0x0010) >> 4;
	int d = (code & 0x0008) >> 3;
	int e = (code & 0x0004) >> 2;
	int f = (code & 0x0002) >> 1;
	int g = code & 0x0001;
	if (a)
		drawHorizontal(shift + 120, 120, im);
	if (b)
		drawVertical(shift + 115, 125, im);
	if (c)
		drawVertical(shift + 215, 125, im);
	if (d)
		drawHorizontal(shift + 120, 200, im);
	if (e)
		drawVertical(shift + 115, 205, im);
	if (f)
		drawVertical(shift + 215, 205, im);
	if (g)
		drawHorizontal(shift + 120, 280, im);
}

void drawTime(int time) {
	/* Declare the image */
	gdImagePtr im;
	/* Declare output files */
	FILE *pngout;
	/* Declare color indexes */
	int black;
	int white;

	/* Allocate the image: 64 pixels across by 64 pixels tall */
	im = gdImageCreate(720, 486);

	/* Allocate the color black (red, green and blue all minimum).
	 Since this is the first color in a new image, it will
	 be the background color. */
	white = gdImageColorAllocate(im, 255, 255, 255);
	black = gdImageColorAllocate(im, 0, 0, 0);

	double shift;

	int min = time / 60;
	int sec = time % 60;
	int dmin = min / 10;
	int umin = min % 10;
	int dsec = sec / 10;
	int usec = sec % 10;

	shift = 0;
	drawNumber(shift, im, dmin);

	shift = 120;
	drawNumber(shift, im, umin);

	shift = 260;
	drawNumber(shift, im, dsec);

	shift = 380;
	drawNumber(shift, im, usec);

	gdImageFilledEllipse(im, 360, 160, 15, 15, black);
	gdImageFilledEllipse(im, 360, 240, 15, 15, black);

	FILE* png = fopen("logo atos.png", "rb");
	gdImagePtr logo = gdImageCreateFromPng(png);
	gdImageCopy(im, logo, 300, 300, 0, 0, 400, 165);

	fclose(png);

	/* Open a file for writing. "wb" means "write binary", important
	 under MSDOS, harmless under Unix. */
	char filename[9];
	filename[8] = 0;
	int i = tmax - time;
	if (i < 10) {
		sprintf(filename, "f00%d.png", i);
	} else if (i < 100) {
		sprintf(filename, "f0%d.png", i);
	} else {
		sprintf(filename, "f%d.png", i);
	}
	pngout = fopen(filename, "wb");

	/* Output the image to the disk file in PNG format. */
	gdImagePng(im, pngout);

	/* Close the files. */
	fclose(pngout);

	/* Destroy the image in memory. */
	gdImageDestroy(im);
}

int main() {
	for (int i = 0; i <= tmax; i++)
		if (i % 10 == 0)
			printf("=");
	printf("\n");
	for (int i = 0; i <= tmax; i++) {
		drawTime(tmax - i);
		if (i % 10 == 0)
			printf(".");
	}
	printf("\n");
	system("mencoder mf://f*.png -mf fps=1 -ovc copy -oac copy -o output.avi");
	system("ffmpeg -y -i output.avi -c:v libx264 -profile:v baseline -level 3.0 -pix_fmt yuv420p working.mp4");
}
