#include <Magick++.h>
#include <iostream>
#include "ImageProcessor.h"

void SeamCarvingTransform(char * inputFile,
						  char * outputFile,
						  unsigned long outputWidth,
						  unsigned long outputHeight);