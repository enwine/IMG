#include <Magick++.h>
#include <math.h>
#include <iostream>

class ImageProcessor {
	
public:
	ImageProcessor(Magick::Image *);
	~ImageProcessor();
	void addCols(unsigned long);
	void removeCols(unsigned long);
	void removeRows(unsigned long);
	Magick::Image* getImage();
	
private:	
	void clean();
	void processImage();
	void gatherPixelValues();
	void gatherPixelEnergies();
	void gatherPixelAccumuledEnergies();
	
	float getPixelValueAt(int,int);
	float getPixelEnergyAt(int,int);
	float getPixelAccumuledEnergyAt(int,int);
	
	int * optimalPath();
	
	
	
	// Vars
	unsigned long rows; // Type defined as unsigned long, as far as it will lose
	unsigned long cols; // precision if not (from Magick::Image definition).
	float * pixelValues;
	float * pixelEnergies;
	float * pixelAccumuledEnergies;
	Magick::Image * imageHandler;
};
