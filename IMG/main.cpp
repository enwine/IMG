//
//  main.cpp
//  resizing
//
//  Created by enwine on 4/17/11.
//  Copyright 2011 Universitat Pompeu Fabra. All rights reserved.
//

#include <iostream>
#include "ImageProcessor.h"
#include <Magick++.h>


int main (int argc, char * argv[]) {
	
	Magick::Image img("paisajes.png");
	ImageProcessor im(&img);
	
//	im.removeRows(1); //esta en proceso de implementacion
	
	im.removeCols(100);
	
//	SeamCarvingTransform("capt2.jpg", "out.jpg", 1000, 1024);
//	SeamCarvingTransform(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
	
    return 0;
}

