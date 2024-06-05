#ifndef FDR_H_
#define FDR_H_

#define UINT16 unsigned short

/*
 * rank	: one dimensional rank array with length n
 * FDR	: return results
 * workspace: temporary array of length n as workspace
 */
void convert_rank_to_FDR(const UINT16 rank[], const UINT16 n, double FDR[], UINT16 workspace[]);

// a convenient wrapper for convert_rank_to_FDR_2d, return FDR array
double *wrapper_convert_rank_to_FDR(const UINT16 rank[], const UINT16 nrow, const UINT16 ncol);


#endif /* FDR_H_ */
