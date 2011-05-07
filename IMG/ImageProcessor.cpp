#include "ImageProcessor.h"
#include <Magick++.h>

#define COLOR_RATIO (255.0/65535.0) 

using namespace std; 
using namespace Magick;

ImageProcessor::ImageProcessor(Image * imageInput) {
	imageHandler = imageInput;
	pixelValues = NULL;
	pixelEnergies = NULL;
	pixelAccumuledVerticalEnergies = NULL;
	pixelAccumuledHorizontalEnergies = NULL; // en proceso de implementacion
	processImage();
}

ImageProcessor::~ImageProcessor() {
	clean();
}

void ImageProcessor::addCols(unsigned long amount) {
	// empty by now
}

void ImageProcessor::addRows(unsigned long amount) {
	// empty by now
}

void ImageProcessor::removeCols(unsigned long amount) {
		
	Geometry geom(cols-1,rows);
	Image im(geom,"white");
	
	/** Bucle para eliminar columna **/
	
	int * verticalPath = verticalPathLesserAE();				//en verticalPath guardamos el camino de pixeles para eliminar
	
	int ivp = rows-1;											//ivp es el indice para recorrer el array del camino, es decir verticalPath
	for (int i=0; i<rows; i++){									//i sera el indice para las filas
		int colRemoved=0;										//colRemoved indica si ya hemos eliminado la columna en la fila en la que estamos
		for (int j=0; j<cols; j++) {							//j sera el indice para las columnas
			int fil = verticalPath[ivp]/cols;					//fil sera la fila en la que se encuentra el pixel ivp del camino, que hay que eliminar
			int col = verticalPath[ivp]-fil*cols;				//col sera la columna en la que se encuentra el pixel ivp del camino, que hay que eliminar
			if (col == j && fil == i) {							//si se cumple, significa que estamos en el pixel que hay que eliminar
				--ivp;											//reducimos el indice del camino, para que busque el siguiente pixel para eliminar
				++colRemoved;									//incrementamos colRemoved para que valga 1
			}else {												//si no se cumple, no estamos en el pixel
				Color col = imageHandler->pixelColor(j, i);		//col sera el Color del pixel que hay en la posicion (j,i)
				im.pixelColor(j-colRemoved, i, col);			//guardamos el color en la posicion (j-colRemoved,i); si el pixel de la fila ya fue eliminado colRemoved valdra 1
			}

		}
	}
	
	imageHandler = &im;
	processImage();
	
	if (amount > 1) {
		removeCols(amount-1);
	} else {
		im.write("TEST.jpg");
	}
}
void ImageProcessor::removeRows(unsigned long amount) {

	Geometry geom(cols-1,rows);
	Image im(geom,"white");
	
	/** Bucle para eliminar columna **/
	
	int * horizontalPath = horizontalPathLesserAE();			//en verticalPath guardamos el camino de pixeles para eliminar
	
	int ihp = cols-1;											//ivp es el indice para recorrer el array del camino, es decir verticalPath
	for (int i=0; i<cols; i++){									//i sera el indice para las filas
		if(i == cols-1)
		printf("\n%d ", i);
		int rowRemoved=0;										//colRemoved indica si ya hemos eliminado la columna en la fila en la que estamos
		for (int j=0; j<rows; j++) {							//j sera el indice para las columnas
			//printf("%d ", j);
			int fil = horizontalPath[ihp]/cols;					//fil sera la fila en la que se encuentra el pixel ivp del camino, que hay que eliminar
			int col = horizontalPath[ihp]-fil*cols;				//col sera la columna en la que se encuentra el pixel ivp del camino, que hay que eliminar
			if (col == i && fil == j) {							//si se cumple, significa que estamos en el pixel que hay que eliminar
				--ihp;											//reducimos el indice del camino, para que busque el siguiente pixel para eliminar
				++rowRemoved;									//incrementamos colRemoved para que valga 1
			}else {												//si no se cumple, no estamos en el pixel
				Color col = imageHandler->pixelColor(3, 3);		//col sera el Color del pixel que hay en la posicion (j,i)
				im.pixelColor(i, j-rowRemoved , col);			//guardamos el color en la posicion (j-colRemoved,i); si el pixel de la fila ya fue eliminado colRemoved valdra 1
			}
			
		}
	}
	
	imageHandler = &im;
	processImage();
	
	if (amount > 1) {
		removeCols(amount-1);
	} else {
		im.write("TEST.jpg");
	}
	
	
}

void ImageProcessor::clean() {
	delete [] pixelValues;
	delete [] pixelEnergies;
	delete [] pixelAccumuledVerticalEnergies;
	delete [] pixelAccumuledHorizontalEnergies; // en proceso de implementacion
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
	pixelAccumuledVerticalEnergies = new float [rows*cols];
	pixelAccumuledHorizontalEnergies = new float [rows*cols];
	
	gatherPixelValues();
	gatherPixelEnergies();
	gatherPixelAccumuledVerticalEnergies();
	gatherPixelAccumuledHorizontalEnergies(); // en proceso de implementacion
	
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
//#ifdef  DEBUG
	float max = 0;
//#endif
	
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			*(pixelEnergies+i*cols+j) = 0;
			*(pixelEnergies+i*cols+j) += fabs( (getPixelValueAt(i+1,j) - getPixelValueAt(i-1,j)) / 2.0 );
			*(pixelEnergies+i*cols+j) += fabs( (getPixelValueAt(i,j+1) - getPixelValueAt(i,j-1)) / 2.0 );
//#ifdef  DEBUG
			if (*(pixelEnergies+i*cols+j) > max) {
				max = *(pixelEnergies+i*cols+j);
			}
//#endif
		}
	}
	
//#ifdef  DEBUG
	Geometry geom(cols,rows);
	Image im(geom,"white");
	float r = 65535.0/max;
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			Color col((int)r*(*(pixelEnergies+i*cols+j)),(int)r*(*(pixelEnergies+i*cols+j)),(int)r*(*(pixelEnergies+i*cols+j)));
			im.pixelColor(j, i,	col);
		}
	}
	im.write("energy.jpg");
//#endif
}

void ImageProcessor::gatherPixelAccumuledVerticalEnergies() {
//#ifdef  DEBUG
	float max = 0;
//#endif
	
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			*(pixelAccumuledVerticalEnergies+i*cols+j) = 0;
			*(pixelAccumuledVerticalEnergies+i*cols+j) += getPixelEnergyAt(i,j);
			*(pixelAccumuledVerticalEnergies+i*cols+j) += min(getPixelAccumuledVerticalEnergyAt(i-1,j), min(getPixelAccumuledVerticalEnergyAt(i-1,j+1), getPixelAccumuledVerticalEnergyAt(i-1,j-1)));
//#ifdef  DEBUG
			if (*(pixelAccumuledVerticalEnergies+i*cols+j) > max) {
				max = *(pixelAccumuledVerticalEnergies+i*cols+j);
			}
//#endif
		}
	}
	
//#ifdef  DEBUG
	Geometry geom(cols,rows);
	Image im(geom,"white");
	float r = 65535.0/max;
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			Color col((int)r*(*(pixelAccumuledVerticalEnergies+i*cols+j)),(int)r*(*(pixelAccumuledVerticalEnergies+i*cols+j)),(int)r*(*(pixelAccumuledVerticalEnergies+i*cols+j)));
			im.pixelColor(j, i,	col);
		}
	}
	im.write("accumuled_energy.jpg");
//#endif
}

void ImageProcessor::gatherPixelAccumuledHorizontalEnergies() {
#ifdef  DEBUG
	float max = 0;
#endif
	
	for(int j = 0; j < cols; j++) {
		for(int i = 0; i < rows; i++) {
			*(pixelAccumuledHorizontalEnergies+i*cols+j) = 0;
			*(pixelAccumuledHorizontalEnergies+i*cols+j) += getPixelEnergyAt(i,j);
			*(pixelAccumuledHorizontalEnergies+i*cols+j) += min(getPixelAccumuledHorizontalEnergyAt(i,j-1), min(getPixelAccumuledHorizontalEnergyAt(i+1,j-1), getPixelAccumuledHorizontalEnergyAt(i-1,j-1)));
#ifdef  DEBUG
			if (*(pixelAccumuledHorizontalEnergies+i*cols+j) > max) {
				max = *(pixelAccumuledHorizontalEnergies+i*cols+j);
			}
#endif
		}
	}
	
#ifdef  DEBUG
	Geometry geom(cols,rows);
	Image im(geom,"white");
	float r = 65535.0/max;
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			Color col((int)r*(*(pixelAccumuledHorizontalEnergies+i*cols+j)),(int)r*(*(pixelAccumuledHorizontalEnergies+i*cols+j)),(int)r*(*(pixelAccumuledHorizontalEnergies+i*cols+j)));
			im.pixelColor(j, i,	col);
		}
	}
	im.write("accumuled_energy_h.jpg");
#endif
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

float ImageProcessor::getPixelAccumuledVerticalEnergyAt(int i, int j) {
	if (j < 0 || j >= cols) {
		return 999999; //we return a value over 255, which is the max energy, and then this energy won't be selected.
	}
	if (i < 0) {
		return 0;
	}
	return (*(pixelAccumuledVerticalEnergies+i*cols+j));
}

float ImageProcessor::getPixelAccumuledHorizontalEnergyAt(int i, int j) {
	if (i < 0 || i >= cols) {
		return 999999; //we return a value over 255, which is the max energy, and then this energy won't be selected.
	}
	if (j < 0) {
		return 0;
	}
	float val = (*(pixelAccumuledHorizontalEnergies+i*cols+j));
	return val;
}

float* ImageProcessor::getRowRangeAE(int row, int initRange, int endRange){	//metodo que retorna un array de floats, de la fila row, entre las posiciones initRange y endRange
	float* rowRange;														//rowRange contendra los valores entre initRange y endRange de la fila row
	int range = endRange-initRange+1;										//range contiene la cantidad de elementos que contendra rowRange
	rowRange = new float [range];
	int point;																//point sera el indice para recorrer el array rowRange
	for (point=0; point < range; ++point) {
		rowRange[point] = getPixelAccumuledVerticalEnergyAt(row,initRange+point);	//obtenemos el valor del array de la energia acumulada, en la posicion (row,initRange+point)
	}
	return rowRange;														//retornamos rowRange
};

int ImageProcessor::indexLesserValueArray(float *array, int sizeArray){ //metodo que retorna el indice de la posicion del array con menor valor
	int min=0;															//min contendra el indice de la posicion con menor valor
	for (int i=0; i<sizeArray; ++i) {									//recorremos todo el array
		if (array[i] < array[min]) min = i;								//comparamos si el valor de la posicion actual es menor que la que tenemos como minimo hasta ahora,
	}																	//si se cumple sustituimos el indice por el actual
	return min;															//retornamos el indice
};

int* ImageProcessor::verticalPathLesserAE(){												//metodo que retorna un array, con los indices de la matriz de la imagen de energia acumulada, que formaran el camino de pixeles para eliminar.
	int* path = new int [rows];																//path sera el array de enteros, en el que se guardaran los indices.
	path[0] = indexLesserValueArray(getRowRangeAE(rows-1, 0, cols-1), cols)+(rows-1)*cols;	//buscamos el pixel de menor valor en la ultima fila, y lo guardamos en la primera posicion de path.
	int min;
	for (int i=1; i<rows; ++i) {
		min = indexLesserValueArray(getRowRangeAE(rows-i-1, (path[i-1]-cols-1)-((rows-i-1)*cols), (path[i-1]-cols+1)-((rows-i-1)*cols)), 3);	//como se define en el algoritmo, el camino se generara segun el minimo de los 3 pixeles de la fila superior a la actual.
		if (min == 0)																															//por lo tanto buscamos buscamos el pixel de menor valor, entre esas 3 posiciones de la fila superior
			path[i] = (path[i-1]-cols)-1;																										//obtendremos el indice del que tenga menor valor, y lo guardaremos en path como siguiente posicion del camino.
		if (min == 1)
			path[i] = (path[i-1]-cols);
		if (min == 2)
			path[i] = (path[i-1]-cols)+1;
	}
	return path;																			// retornamos path
};

float* ImageProcessor::getColRangeAE(int col, int initRange, int endRange){	//metodo que retorna un array de floats, de la columna col, entre las posiciones initRange y endRange
	float* colRange;														//colRange contendra los valores entre initRange y endRange de la columna col
	int range = endRange-initRange+1;										//range contiene la cantidad de elementos que contendra colRange
	colRange = new float [range];
	int point;																//point sera el indice para recorrer el array colRange
	for (point=initRange; point <= endRange; point++) {
		colRange[point] = getPixelAccumuledHorizontalEnergyAt(point,col);	//obtenemos el valor del array de la energia acumulada, en la posicion (initRange+point,col)
	}
	return colRange;														//retornamos colRange
};

int* ImageProcessor::horizontalPathLesserAE(){												//metodo que retorna un array, con los indices de la matriz de la imagen de energia acumulada, que formaran el camino de pixeles para eliminar.
	int* path = new int [cols];																//path sera el array de enteros, en el que se guardaran los indices.
	path[0] = indexLesserValueArray(getColRangeAE(cols-1, 0, rows-1), rows)*cols+(cols-1);	//buscamos el pixel de menor valor en la ultima fila, y lo guardamos en la primera posicion de path.
	printf("a%d\n",path[0]);
	int min;
	for (int i=1; i<cols; ++i) {
		min = indexLesserValueArray(getColRangeAE(cols-i-1, (path[i-1]-cols-1)/cols,(path[i-1]+cols-1)/cols), 3);	//como se define en el algoritmo, el camino se generara segun el minimo de los 3 pixeles de la columna anterior a la actual.
		if (min == 0)																								//por lo tanto buscamos buscamos el pixel de menor valor, entre esas 3 posiciones de la columna anterior
			path[i] = (path[i-1]-1)-cols;																			//obtendremos el indice del que tenga menor valor, y lo guardaremos en path como siguiente posicion del camino.
		if (min == 1)
			path[i] = (path[i-1]-1);
		if (min == 2)
			path[i] = (path[i-1]-1)+cols;
		printf("%d\n",path[i]);
	}
	return path;																			// retornamos path
};
