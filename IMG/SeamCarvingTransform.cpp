#include "SeamCarvingTransform.h"

void SeamCarvingTransform(char * inputFile,
						  char * outputFile,
						  unsigned long outputWidth,
						  unsigned long outputHeight) {
	
	Magick::Image * myImageHandler;
	myImageHandler = (Magick::Image*) malloc(sizeof(Magick::Image));
	*myImageHandler = Magick::Image(inputFile);
	
	if (myImageHandler->columns() > outputWidth) {
		ImageProcessor myImageProcessor(myImageHandler);
		myImageProcessor.removeCols( myImageHandler->columns() - outputWidth );
		myImageHandler = myImageProcessor.getImage();
	} else if (myImageHandler->columns() < outputWidth) {
		ImageProcessor myImageProcessor(myImageHandler);
		myImageProcessor.removeCols( outputWidth - myImageHandler->columns() );
		myImageHandler = myImageProcessor.getImage();
	}

	if (myImageHandler->rows() > outputHeight) {
		myImageHandler->rotate(90);
		ImageProcessor myImageProcessor(myImageHandler);
		myImageProcessor.removeCols( myImageHandler->rows() - outputHeight );
		myImageHandler = myImageProcessor.getImage();
		myImageHandler->rotate(-90);
	} else if (myImageHandler->rows() < outputHeight) {
		myImageHandler->rotate(90);
		ImageProcessor myImageProcessor(myImageHandler);
		myImageProcessor.removeCols( outputHeight - myImageHandler->rows() );
		myImageHandler = myImageProcessor.getImage();
		myImageHandler->rotate(-90);
	}
	
	myImageHandler->write(outputFile);
	delete myImageHandler;
	
}