#include "FDR.h"
#include <stdlib.h>

void convert_rank_to_FDR(const UINT16 rank[], const UINT16 n, double FDR[], UINT16 workspace[])
{
	UINT16 i, j, pivot, cnt=0;

	double q;

	for(i=0;i<n;i++)
	{
		pivot = rank[i];

		// elements ranking higher beyond and include current position
		for(cnt=0, j=0; j<=i; j++)
		{
			if(rank[j] <= pivot)
			{
				workspace[cnt] = j;
				cnt++;
			}
		}

		q = (double)((i+1)*pivot)/(n*cnt);
		if(q>1) q=1;

		FDR[i] = q;

		// pass again to make monotonic
		for(j=0; j < cnt-1; j++)
		{
			pivot = workspace[j];

			if(FDR[pivot] > q) FDR[pivot] = q;
		}
	}
}


double *wrapper_convert_rank_to_FDR(const UINT16 rank[], const UINT16 nrow, const UINT16 ncol)
{
	UINT16 i, workspace[ncol];
	double *FDR = (double*)malloc(nrow * ncol * sizeof(double));

	for(i=0;i<nrow;i++) convert_rank_to_FDR(rank+i*ncol, ncol, FDR + i*ncol, workspace);

	return FDR;
}
