#include "ImageProcessor.h"
#include <Magick++.h>

#define COLOR_RATIO (255.0/65535.0) 

using namespace std; 
using namespace Magick;

ImageProcessor::ImageProcessor() {
	imageHandler = new Image("temp.png");
	pixelValues = NULL;
	pixelEnergies = NULL;
	pixelAccumuledEnergies = NULL;
	processImage();
}

ImageProcessor::~ImageProcessor() {
	clean();
}

void ImageProcessor::addCols(unsigned long amount) {
	Geometry geom(cols+1, rows);
	Image im(geom,"white");
	
	int * path = optimalPath();
	
	int colAdded = 0;
	for (int i=0; i<rows; i++){
		colAdded = 0;
		for (int j=0; j<cols; j++){
			Color col = imageHandler->pixelColor(j, i);
			im.pixelColor(j+colAdded, i, col);
			if (j == path[i]) {
				colAdded++;
				Color col = imageHandler->pixelColor(j, i);
				im.pixelColor(j+colAdded, i, col);
			}
		}
	}
	
	imageHandler = &im;
	processImage();
	
	if (amount > 1) {
		addCols(amount-1);
	} else {
		im.write("temp.png");
	}	
}

void ImageProcessor::removeCols(unsigned long amount) {		
	Geometry geom(cols-1, rows);
	Image im(geom,"white");
	
	int * path = optimalPath();
	
	int colRemoved = 0;
	for (int i=0; i<rows; i++){
		colRemoved = 0;
		for (int j=0; j<cols; j++){
			if (j == path[i]) {
				colRemoved++;
			} else {
				Color col = imageHandler->pixelColor(j, i);
				im.pixelColor(j-colRemoved, i, col);
			}
		}
	}
	
	imageHandler = &im;
	processImage();
	
	if (amount > 1) {
		removeCols(amount-1);
	} else {
		im.write("temp.png");
	}
}

Image ImageProcessor::getImage() {
	return *imageHandler;
}
void ImageProcessor::clean() {
	delete [] pixelValues;
	delete [] pixelEnergies;
	delete [] pixelAccumuledEnergies;
}
void ImageProcessor::processImage() {	
	// First, check if the memory was previously allocated, if so, clean.
	if (pixelValues != NULL) {
		clean();
	}
	
	// Get our sizes, and keep it in handy to use it as many times as we need.
	ImageProcessor::rows = imageHandler->rows();
	ImageProcessor::cols = imageHandler->columns();
	
	pixelValues = new float [rows*cols];
	pixelEnergies = new float [rows*cols];
	pixelAccumuledEnergies = new float [rows*cols];	
	gatherPixelValues();
	gatherPixelEnergies();
	gatherPixelAccumuledEnergies();	
}
void ImageProcessor::gatherPixelValues() {
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			*(pixelValues+i*cols+j) = 0;
			*(pixelValues+i*cols+j) += (float)COLOR_RATIO*(imageHandler->pixelColor(j,i)).redQuantum();
			*(pixelValues+i*cols+j) += (float)COLOR_RATIO*(imageHandler->pixelColor(j,i)).greenQuantum();
			*(pixelValues+i*cols+j) += (float)COLOR_RATIO*(imageHandler->pixelColor(j,i)).blueQuantum();
			*(pixelValues+i*cols+j) /= 3.0;
		}
	}
}
void ImageProcessor::gatherPixelEnergies() {
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			*(pixelEnergies+i*cols+j) = 0;
			*(pixelEnergies+i*cols+j) += fabs( (getPixelValueAt(i+1,j) - getPixelValueAt(i-1,j)) / 2.0 );
			*(pixelEnergies+i*cols+j) += fabs( (getPixelValueAt(i,j+1) - getPixelValueAt(i,j-1)) / 2.0 );
		}
	}
}
void ImageProcessor::gatherPixelAccumuledEnergies() {
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			*(pixelAccumuledEnergies+i*cols+j) = 0;
			*(pixelAccumuledEnergies+i*cols+j) += getPixelEnergyAt(i,j);
			*(pixelAccumuledEnergies+i*cols+j) += min(getPixelAccumuledEnergyAt(i-1,j), min(getPixelAccumuledEnergyAt(i-1,j+1), getPixelAccumuledEnergyAt(i-1,j-1)));
		}
	}
}
float ImageProcessor::getPixelValueAt(int i, int j) {
	if (i < 0 || j < 0 || j >= cols || i >= rows ) {
		return 0.0;
	}
	return (*(pixelValues+i*cols+j));
}
float ImageProcessor::getPixelEnergyAt(int i, int j) {
	if (i < 0 || j < 0 || j >= cols || i >= rows ) {
		return 0.0;
	}
	return (*(pixelEnergies+i*cols+j));
}
float ImageProcessor::getPixelAccumuledEnergyAt(int i, int j) {
	if (j < 0 || j >= cols) {
		return 99999; //we return a value over 255, which is the max energy, and then this energy won't be selected.
	}
	if (i < 0) {
		return 0;
	}
	return (*(pixelAccumuledEnergies+i*cols+j));
}
int * ImageProcessor::optimalPath() {
	int * path = new int[rows];
	path[rows-1] = 0;
	for(int i=0; i < cols; i++) {
		if(getPixelAccumuledEnergyAt((int)rows-1,i) < getPixelAccumuledEnergyAt((int)rows-1,path[rows-1])) {
			path[rows-1] = i;
		}
	}
	for(int i=(int)rows-2; i >= 0; i--) {
		if(getPixelAccumuledEnergyAt(i,path[i+1]-1) < getPixelAccumuledEnergyAt(i,path[i+1])) {
			if(getPixelAccumuledEnergyAt(i,path[i+1]-1) < getPixelAccumuledEnergyAt(i,path[i+1]+1)) {
				path[i] = path[i+1]-1;
			} else {
				path[i] = path[i+1]+1;
			}
		} else if(getPixelAccumuledEnergyAt(i,path[i+1]) < getPixelAccumuledEnergyAt(i,path[i+1]+1)) {
			path[i] = path[i+1];
		} else {
			path[i] = path[i+1]+1;
		}
	}
	return path;
}
