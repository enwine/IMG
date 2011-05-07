//
//  main.cpp
//  resizing
//
//  Created by enwine on 4/17/11.
//  Copyright 2011 Universitat Pompeu Fabra. All rights reserved.
//

#include <iostream>
#include "SeamCarvingTransform.h"
#include <Magick++.h>


int main (int argc, char * argv[]) {
	
	SeamCarvingTransform(*(argv+1), *(argv+2), atoi(*(argv+3)), atoi(*(argv+4)));

	return 0;

}

