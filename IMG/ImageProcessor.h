#include <Magick++.h>
#include <math.h>
#include <iostream>


int minValueIndex(float *array, int sizeArray);

class ImageProcessor {
	
public:
	ImageProcessor(Magick::Image *);
	~ImageProcessor();
	void addCols(unsigned long);
	void addRows(unsigned long);
	void removeCols(unsigned long);
	void removeRows(unsigned long);
	
private:	
	void clean();
	void processImage();
	void gatherPixelValues();
	void gatherPixelEnergies();
	void gatherPixelAccumuledVerticalEnergies();
	void gatherPixelAccumuledHorizontalEnergies();
	
	float getPixelValueAt(int,int);
	float getPixelEnergyAt(int,int);
	float getPixelAccumuledVerticalEnergyAt(int,int);
	float getPixelAccumuledHorizontalEnergyAt(int,int);
	
	// Necesario para array de pixeles a eliminar
	float * getRowRangeAE(int,int,int);
	float * getColRangeAE(int,int,int);
	int indexLesserValueArray(float*,int);
	int * verticalPathLesserAE();
	int * horizontalPathLesserAE();
	
	
	
	// Vars
	unsigned long rows; // Type defined as unsigned long, as far as it will lose
	unsigned long cols; // precision if not (from Magick::Image definition).
	float * pixelValues;
	float * pixelEnergies;
	float * pixelAccumuledVerticalEnergies;
	float * pixelAccumuledHorizontalEnergies;
	Magick::Image * imageHandler;
};
