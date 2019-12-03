
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#define ROW 16 // matrix row number
#define COL 16 // matrix column number
#define DIAG (COL<ROW ? COL : ROW) // diagonal matrix size

#define BOX_CONST 10 // Box constraint on the variable

template<class T, int M>
void VEC_EQ( T V1[M], T V2[M] ){
#pragma HLS INLINE off
	for( int i=0; i<M; i++ ){
#pragma HLS PIPELINE II=2 rewind
		V2[i] = V1[i];
	}
}

template<class T, int M>
void VEC_SUB( T V1[M], T V2[M], T V3[M] ){
#pragma HLS INLINE off
	for( int i=0; i<M; i++ ){
#pragma HLS PIPELINE II=2 rewind
		V3[i] = V1[i] - V2[i];
	}
}

template<class T, int M>
void VEC_ADD( T V1[M], T V2[M], T V3[M] ){
#pragma HLS INLINE off
	for( int i=0; i<M; i++ ){
#pragma HLS PIPELINE II=2 rewind
		V3[i] = V1[i] + V2[i];
	}
}

template<class T, int M, int N>
void MAT_VEC_MUL(T A[M][N],
									  T B[N],
									  T C[M]){
#pragma HLS INLINE off
	for ( int i=0; i<M; i++ ){
//#pragma HLS PIPELINE II=2 rewind
		C[i] = 0;
		for ( int j=0; j<N; j++ ){
			C[i] += A[i][j] * B[j];
		}
	}
}

template<class T, int M>
void VEC_SCALAR_MUL( T V1[M], T S, T V3[M] ){
#pragma HLS INLINE off
	for( int i=0; i<M; i++ ){
#pragma HLS PIPELINE II=2 rewind
		V3[i] = V1[i] * S;
	}
}

template<class T, int M>
void ONES_VEC( T V[M] ){
#pragma HLS INLINE off
	for( int i=0; i<M; i++ ){
#pragma HLS PIPELINE II=2 rewind
		V[i] = 1;
	}
}

template<class T, int M>
void VEC_SCALAR_MIN( T V1[M], T S, T V3[M] ){
#pragma HLS INLINE off
	for( int i=0; i<M; i++ ){
#pragma HLS PIPELINE II=2 rewind
		V3[i] = V1[i] < S ? V1[i] : S;
	}
}

template<class T, int M>
void VEC_SCALAR_MAX( T V1[M], T S, T V3[M] ){
#pragma HLS INLINE off
	for( int i=0; i<M; i++ ){
#pragma HLS PIPELINE II=2 rewind
		V3[i] = V1[i] > S ? V1[i] : S;
	}
}

void kernel(double x_k_vec_in[DIAG], double x_k_vec_out[DIAG]);
