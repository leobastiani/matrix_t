#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "matrix.h"

/**
 * create matrix
 * @param  matrix a simple &matrix_t
 * @param  width  
 * @param  height 
 * @return        true if success
 */
bool matrixCreate(matrix_t *matrix, unsigned width, unsigned height) {
	// apenas alocando...
	matrix->elems = (matrix_elem_t **) malloc(height*sizeof(matrix_elem_t *));
	if(matrix->elems == NULL) {
		return false;
	}
	int i;
	for(i=0; i<height; i++) {
		matrix->elems[i] = (matrix_elem_t *) calloc(width,sizeof(matrix_elem_t));
		if(matrix->elems[i] == NULL) {
			return false;
		}
	}
	matrix->width = width;
	matrix->height = height;
	return true;
}

/**
 * return true if elem in lin x col exists
 * @param  matrix simple &matrix_t
 * @param  lin    
 * @param  col    
 * @return        true or false
 */
bool matrixElemExists(matrix_t *matrix, int lin, int col) {
	if(	(lin >= matrix->height  || lin < 0) ||
		(col >= matrix->width || col < 0) ) { // caso a linha e coluna estejam fora do 
		return false;
	}
	return true;
}

/**
 * set one elem in matrix
 * @param  matrix simple &matrix_t initialized
 * @param  lin    
 * @param  col    
 * @param  val    value of elem
 * @return        true if success
 */
bool matrixSet(matrix_t *matrix, int lin, int col, matrix_elem_t val) {
	if(!matrixElemExists(matrix, lin, col)) {
		return false;
	}
	matrix->elems[lin][col] = val;
	return true;
}

/**
 * return the elem at lin x col
 * @param  matrix simple &matrix_t
 * @param  lin    
 * @param  col    
 * @return        value of elem in double
 */
double matrixGet(matrix_t *matrix, int lin, int col) {
	if(!matrixElemExists(matrix, lin, col)) {
		return 0;
	}
	return matrix->elems[lin][col];
}

/**
 * calculates the inverse of matrix
 * @param  matrix simple &matrix_t initialized
 * @return        true if success
 */
bool matrixInverse(matrix_t *matrix) {
	if(matrix->width != matrix->height) { // aceita apenas matrizes quadradas
		return false;
	}
	matrix_t result; // cria uma matriz 2N x N, o objetivo é transformar os primeiros N x N em uma identidade
	int size = matrix->width;
	matrixCreate(&result, size * 2, size);
	int i, j;
	for(i=0; i<size; i++) { // copiando os elementos da matrix para result
		for(j=0; j<size; j++) {
			result.elems[i][j] = matrix->elems[i][j];
		}
	}
	for(i=0; i<size; i++) { // copiando os elementos da matrix para result
		result.elems[i][i+size] = 1;
	}

	if(matrixResolveLinearsys(&result) == false) { // utiliza a função do t2 para encontrar a matriz inversa
		matrixFree(&result);
		return false;
	}

	for(i=0; i<size; i++) { // copiando os elementos da matrix para result
		for(j=0; j<size; j++) {
			matrix->elems[i][j] = result.elems[i][j+size];
		}
	}

	matrixFree(&result);
	return true;
}

/**
 * multiply A x B
 * @param  A    matrix_t A initialized
 * @param  B    matrix_t B initialized
 * @param  dest matrix_t dest not initialized
 * @return      true if success
 */
bool matrixMul(matrix_t *A, matrix_t *B, matrix_t *dest) {
	if(A->height != B->width) { // se não for possível multiplicar as matrizes
		return false;
	}
	int size = A->height;
	matrixCreate(dest, size, size); // cria a matriz dest
	int i, j;
	for(i=0; i<size; i++) {
		for(j=0; j<size; j++) {
			// apenas encontrando o valor dest[i][j]
			matrix_elem_t dest_val = 0;
			int k;
			for(k=0; k<A->width; k++) {
				matrix_elem_t elem_a = A->elems[i][k];
				matrix_elem_t elem_b = B->elems[k][j];
				dest_val += elem_a * elem_b;
			}
			dest->elems[i][j] = dest_val;
		}
	}
	return true;
}

/**
 * verify if a line exists in matrix
 * @param  matrix simple &matrix_t initialized
 * @param  line   
 * @return        true if success
 */
bool matrixVerifyLine(matrix_t *matrix, int line) {
	if(line < 0 || line >= matrix->width) {
		return false;
	}
	return true;
}

/**
 * multiply a line by a factor
 * @param  matrix simple &matrix_t initialized
 * @param  line   
 * @param  factor 
 * @return        true if success
 */
bool matrixMulLine(matrix_t *matrix, int line, double factor) {
	int width = matrix->width;
	if(!matrixVerifyLine(matrix, line)) {
		return false;
	}
	int i;
	for(i=0; i<width; i++) {
		matrix->elems[line][i] *= factor;
	}
	return true;
}

/**
 * add line src * factor to line dest
 * @param  matrix simple &matrix_t initialized
 * @param  src    
 * @param  dest   
 * @param  factor 
 * @return        true if success
 */
bool matrixAddLine(matrix_t *matrix, int src, int dest, double factor) {
	if(!matrixVerifyLine(matrix, src) || matrixVerifyLine(matrix, dest)) {
		return false;
	}
	int width = matrix->width;
	int i;
	for(i=0;i<width;i++) {
		matrix->elems[dest][i] += matrix->elems[src][i] * factor;
	}
}

/**
 * change lines in matrix_t
 * @param  matrix [description]
 * @param  line1  [description]
 * @param  line2  [description]
 * @return        [description]
 */
bool matrixChangeLine(matrix_t *matrix, int line1, int line2) {
	if(line1 == line2) {
		return true; // nao faz sentido trocar a linha por ela msma
	}
	if(!matrixVerifyLine(matrix, line1) || !matrixVerifyLine(matrix, line2)) {
		return false;
	}

	matrix_elem_t *temp;
	temp = matrix->elems[line1];
	matrix->elems[line1] = matrix->elems[line2];
	matrix->elems[line2] = temp;
	return true;
}

/**
 * resolve a linear system like
 *   1  0  0  ?
 *   0  1  0  ?
 *   0  0  1  ?
 * @param  matrix simple &matrix_t initialized
 * @return        true if success
 */
bool matrixResolveLinearsys(matrix_t *matrix) {
	int size = matrix->height;
	int i, j;
	for(i=0;i<size;i++) {
		if(matrix->elems[i][i] == 0) { // tenta trocar a linha por outra
			for(j=i+1;j<size;j++) { // começa a procurar por linhas que podem trocar
				if(matrix->elems[j][i] != 0) {
					matrixChangeLine(matrix, i, j);
				}
			}
		}
		if(matrix->elems[i][i] == 0) {
			return false; // se o primeiro elemento for zero, nao pode 1 / 0
		}
		double factor = 1 / (double) matrix->elems[i][i];
		matrixMulLine(matrix, i, factor);
		for(j=0;j<size;j++) {
			if(i == j) { // nao executa para a mesma linha
				continue;
			}
			double factor = -1 * matrix->elems[j][i] / (double) matrix->elems[i][i];
			matrixAddLine(matrix, i, j, factor);
		}
	}
	return true;
}

/**
 * aux func, only used in matrixDet. get the matrix of the rest cuting the last line and the col = factor
 * @param matrix_src  source matrix
 * @param matrix_dest destiny matrix, not initialized
 * @param factor      col to cut
 */
void _matrixDet(matrix_t *matrix_src, matrix_t *matrix_dest, int factor) {
	int width = matrix_src->width;
	matrixCreate(matrix_dest, width-1, width-1);
	int m = 0; // represents line of matrix dest
	int i; // representes line of matrix src
	int j = 0; // represents col
	for(i=0; i<width; i++) {
		if(i == factor) { // jump the line
			continue;
		}

		for(j=0; j<width-1; j++) {
			matrixSet(matrix_dest, m, j, matrixGet(matrix_src, i, j));
		}
		m++;
	}
}

/**
 * calculates the determinant of a matrix
 * @param  matrix a simple &matrix_t
 * @return        
 */
double matrixDet(matrix_t *matrix) {
	// cut a line and a col, get the rest of the matrix and get the det
	if(matrix->width != matrix->height) { // matrix need to has same value of width and height
		return 0;
	}
	int width = matrix->width;
	if(width <= 0) {
		return 0;
	} else if(width == 1) {
		return matrix->elems[0][0];
	}
	// calculates the det in matrix with 2 or more lines
	int i;
	double result = 0;
	for(i=0; i<width; i++) { // i variant width
		matrix_t rest_matrix;
		_matrixDet(matrix, &rest_matrix, i);
		if((i+width+1) % 2 == 1) {
			result -= matrixDet(&rest_matrix);
		} else {
			result += matrixDet(&rest_matrix);
		}
	}
	return result;
}

/**
 * free matrix_t
 * @param matrix simple &matrix_t
 */
void matrixFree(matrix_t *matrix) {
	int i;
	int height = matrix->height;
	for(i=0; i<height; i++) {
		free(matrix->elems[i]);
	}
	free(matrix->elems);
}