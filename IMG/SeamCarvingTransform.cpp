#include "SeamCarvingTransform.h"

void SeamCarvingTransform(char * inputFile,
						  char * outputFile,
						  int	changeX,
						  int changeY) {
	
	Magick::Image myImageHandler(inputFile);
	myImageHandler.write("temp.png");

	if (changeX > 0) {
		ImageProcessor myImageProcessor;
		myImageProcessor.addCols( changeX );
	} else if (changeX < 0) {
		ImageProcessor myImageProcessor;
		myImageProcessor.removeCols( changeX*-1 );
	}

	if (changeY != 0) {
		Magick::Image img("temp.png");
		img.rotate(90);
		img.write("temp.png");
	}
	
	if (changeY > 0) {
		ImageProcessor myImageProcessor;
		myImageProcessor.addCols( changeY );
	} else if (changeY < 0) {
		ImageProcessor myImageProcessor;
		myImageProcessor.removeCols( changeY*-1 );
	}
	
	if (changeY != 0) {
		Magick::Image img("temp.png");
		img.rotate(-90);
		img.write("temp.png");
	}

	Magick::Image outputImageHandler("temp.png");
	outputImageHandler.write(outputFile);
	remove("temp.png");
	
}