//
//  SeamCarvingTransform.h
//  resizing
//
//  Created by enwine on 4/18/11.
//  Copyright 2011 Universitat Pompeu Fabra. All rights reserved.
//

#include <Magick++.h>
#include <iostream>
#include "ImageProcessor.h"

void SeamCarvingTransform(char * inputFile,
						  char * outputFile,
						  unsigned long outputWidth,
						  unsigned long outputHeight);