//mencoder mf://f*.png -mf fps=1 -ovc copy -oac copy -o output.avi
//ffmpeg -i output.avi -c:v libx264 -profile:v baseline -level 3.0 -pix_fmt yuv420p working.mp4

//Extrai quadro a quadro
//ffmpeg -i ~/WORSHIP\ COUNTDOWN\ 5\ MIN\ \(CONTAGEM\ REGRESSIVA\ -\ CULTO\ JOVEM\)-iTa_cDVN1xw.webm -r 24 image-%04d.bmp
//ffmpeg -i VID_20200108_210308994.mp4 -r 24 leticia_%03d.bmp

/* Bring in gd library functions */
#undef __STRICT_ANSI__
#include <math.h>
#include "gd.h"
/* Bring in standard I/O so we can output the PNG to a file */
#include <stdio.h>

int w = 20;
int npts = 2000;

void drawColor(gdImagePtr bmpMainFrame, int color, int shift, double ang) {

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
		double theta1 = theta2 + ang;
		for (int i = 0; i <= npts; i++) {
			double theta = (theta2 - theta1) / (double) npts * (double) i + theta1;
			double x = xr + r * cos(theta);
			double y = yr + r * sin(theta);
			gdImageSetPixel(bmpMainFrame, x, -y, color);
		}
	}
}

int main() {

	char bmpInputFileName[16];
	char bmpOutputFileName[17];

	int startidx = 24;
	int stopidx = 57;
	int endidx = 118;
	for (int idx = startidx; idx <= endidx; idx++) {
		printf("%i\n", idx);
		if (idx < 10) {
			sprintf(bmpInputFileName, "leticia_00%d.jpg", idx);
			sprintf(bmpOutputFileName, "letician_00%d.jpg", idx);
		} else if (idx < 100) {
			sprintf(bmpInputFileName, "leticia_0%d.jpg", idx);
			sprintf(bmpOutputFileName, "letician_0%d.jpg", idx);
		} else {
			sprintf(bmpInputFileName, "leticia_%d.jpg", idx);
			sprintf(bmpOutputFileName, "letician_%d.jpg", idx);
		}
		bmpInputFileName[15] = '\0';
		bmpOutputFileName[16] = '\0';

		gdImagePtr bmpMainFrame = gdImageCreateTrueColor(1920, 1080);

		int colors[7];
		colors[0] = gdImageColorAllocate(bmpMainFrame, 255, 0, 0); //red
		colors[1] = gdImageColorAllocate(bmpMainFrame, 255, 165, 0); //orange
		colors[2] = gdImageColorAllocate(bmpMainFrame, 255, 255, 0); //yellow
		colors[3] = gdImageColorAllocate(bmpMainFrame, 0, 255, 0); //green
		colors[4] = gdImageColorAllocate(bmpMainFrame, 0, 0, 255); //blue
		colors[5] = gdImageColorAllocate(bmpMainFrame, 75, 0, 130); //indigo
		colors[6] = gdImageColorAllocate(bmpMainFrame, 79, 47, 79); //violet

		FILE* bmpFrame = fopen(bmpInputFileName, "rb");
		gdImagePtr frame = gdImageCreateFromJpeg(bmpFrame);

		gdImageCopy(bmpMainFrame, frame, 0, 0, 0, 0, 1920, 1080);

		double ang;

		if (idx <= stopidx)
			ang = ((double) idx - (double) startidx) / ((double) stopidx - (double) startidx) * M_PI;
		else
			ang = M_PI;

		for (int i = 0; i < 7; i++)
			drawColor(bmpMainFrame, colors[i], i, ang);

		FILE* mainFrame = fopen(bmpOutputFileName, "w");
		gdImageJpeg(bmpMainFrame, mainFrame, 100);

		gdImageDestroy(bmpMainFrame);
		fclose(mainFrame);

		gdImageDestroy(frame);
		fclose(bmpFrame);
	}

	return 0;

	printf("\n");
	system("mencoder mf://f*.png -mf fps=1 -ovc copy -oac copy -o output.avi");
	system("ffmpeg -y -i output.avi -c:v libx264 -profile:v baseline -level 3.0 -pix_fmt yuv420p working.mp4");
}
