typedef double matrix_elem_t;
typedef struct {
	matrix_elem_t **elems;
	int width;
	int height;
} matrix_t;

/**
 * create matrix
 * @param  matrix a simple &matrix_t
 * @param  width  
 * @param  height 
 * @return        true if success
 */
bool matrixCreate(matrix_t *matrix, int width, int height);

/**
 * set one elem in matrix
 * @param  matrix simple &matrix_t initialized
 * @param  lin    
 * @param  col    
 * @param  val    value of elem
 * @return        true if success
 */
bool matrixSet(matrix_t *matrix, int lin, int col, matrix_elem_t val);

/**
 * calculates the inverse of matrix
 * @param  matrix simple &matrix_t initialized
 * @return        true if success
 */
bool matrixInverse(matrix_t *matrix);

/**
 * multiply A x B
 * @param  A    matrix_t A initialized
 * @param  B    matrix_t B initialized
 * @param  dest matrix_t dest not initialized
 * @return      true if success
 */
bool matrixMul(matrix_t *A, matrix_t *B, matrix_t *dest);

/**
 * verify if a line exists in matrix
 * @param  matrix simple &matrix_t initialized
 * @param  line   
 * @return        true if success
 */
bool matrixVerifyLine(matrix_t *matrix, int line);

/**
 * multiply a line by a factor
 * @param  matrix simple &matrix_t initialized
 * @param  line   
 * @param  factor 
 * @return        true if success
 */
bool matrixMulLine(matrix_t *matrix, int line, double factor);

/**
 * add line src * factor to line dest
 * @param  matrix simple &matrix_t initialized
 * @param  src    
 * @param  dest   
 * @param  factor 
 * @return        true if success
 */
bool matrixAddLine(matrix_t *matrix, int src, int dest, double factor);

/**
 * change lines in matrix_t
 * @param  matrix [description]
 * @param  line1  [description]
 * @param  line2  [description]
 * @return        [description]
 */
bool matrixChangeLine(matrix_t *matrix, int line1, int line2);

/**
 * resolve a linear system like
 *   1  0  0  ?
 *   0  1  0  ?
 *   0  0  1  ?
 * @param  matrix simple &matrix_t initialized
 * @return        true if success
 */
bool matrixResolveLinearsys(matrix_t *matrix);

/**
 * free matrix_t
 * @param matrix simple &matrix_t
 */
void matrixFree(matrix_t *matrix);