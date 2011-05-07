//
//  SeamCarvingTransform.cpp
//  resizing
//
//  Created by enwine on 4/18/11.
//  Copyright 2011 Universitat Pompeu Fabra. All rights reserved.
//

#include "SeamCarvingTransform.h"

void SeamCarvingTransform(char * inputFile,
						  char * outputFile,
						  unsigned long outputWidth,
						  unsigned long outputHeight) {
	
	std::cout << "This process may last several minutes ..." << std::endl;
	
	Magick::Image myImageHandler(inputFile);
	std::cout << "Evaluating image data ..." << std::endl;
	ImageProcessor myImageProcessor(&myImageHandler);
	
	std::cout << "Transforming ... \n";
	// Columns transformation
	std::cout << "  - Columns ...";
	if (myImageHandler.columns() > outputWidth) {
		myImageProcessor.removeCols( myImageHandler.columns() - outputWidth );
	} else if (myImageHandler.columns() < outputWidth) {
		myImageProcessor.addCols( outputWidth - myImageHandler.columns() );
	}
	std::cout << " transformed." << std::endl;
	
	// Rows transformation
	std::cout << "  - Rows ...";
	if (myImageHandler.rows() > outputHeight) {
		myImageProcessor.removeRows( myImageHandler.rows() - outputHeight );
	} else if (myImageHandler.rows() < outputHeight) {
		myImageProcessor.addRows( outputHeight - myImageHandler.rows() );
	}
	std::cout << " transformed." << std::endl;
	
	// Pending! An interlaced implementation when performing operation in both 
	// axis (x, y).
	
	std::cout << "Printing image ..." << std::endl;
	myImageHandler.write(outputFile);
	
	std::cout << "Process finished." << std::endl;
	
}