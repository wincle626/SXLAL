/*
 *	This is C++ implementation Ali's admm
 *	Author: Yun Wu
 *	Created by: 2019-07-18
 *	Copyright @ Yun Wu
 *
 */

#include "admm.hpp"
FILE_IO fileioobj;

void ADMM_EXAMPLE_DOUBLE(double b[DIM_SIZE][DIM_SIZE],
						 	 double eigDtD[DIM_SIZE][DIM_SIZE],
						 	 double Init[DIM_SIZE][DIM_SIZE],
							 double S[DIM_SIZE][DIM_SIZE],
							 double xout[DIM_SIZE][DIM_SIZE]){

	Float_Point_Algebra fptalgebraobj;
	FFT_FFTW3 fftfftw3obj;

	// initiate variables
	double x[DIM_SIZE][DIM_SIZE];
	double r[DIM_SIZE][DIM_SIZE];
	double vx[DIM_SIZE][DIM_SIZE];
	double vy[DIM_SIZE][DIM_SIZE];
	double y2[DIM_SIZE][DIM_SIZE];
	double y3x[DIM_SIZE][DIM_SIZE];
	double y3y[DIM_SIZE][DIM_SIZE];
	double rhs1[DIM_SIZE][DIM_SIZE];
	double rhs2[DIM_SIZE][DIM_SIZE];
	double xold[DIM_SIZE][DIM_SIZE];

	double Sb[DIM_SIZE][DIM_SIZE];
	double rhor[DIM_SIZE][DIM_SIZE];
	double Sbrhor[DIM_SIZE][DIM_SIZE];
	double S2[DIM_SIZE][DIM_SIZE];
	double rhoS2[DIM_SIZE][DIM_SIZE];
	double rhoS2inv[DIM_SIZE][DIM_SIZE];
	double rhovx[DIM_SIZE][DIM_SIZE];
	double rhovy[DIM_SIZE][DIM_SIZE];
	double y3xrhovx[DIM_SIZE][DIM_SIZE];
	double y3yrhovy[DIM_SIZE][DIM_SIZE];
	double Dty3rhov[DIM_SIZE][DIM_SIZE];
	double rhox[DIM_SIZE][DIM_SIZE];
	double rhoxDty3rhov[DIM_SIZE][DIM_SIZE];
	double rhoeigDtD[DIM_SIZE][DIM_SIZE];
	double rhorhoeigDtD[DIM_SIZE][DIM_SIZE];
	Complex<double> y2rhoxDty3rhovcmplx[DIM_SIZE][DIM_SIZE];
	Complex<double> fft2rhs[DIM_SIZE][DIM_SIZE];
	Complex<double> fft2rhsrhorhoeigDtD[DIM_SIZE][DIM_SIZE];
	Complex<double> ifft2fft2rhsrhorhoeigDtD[DIM_SIZE][DIM_SIZE];
	double Dx[DIM_SIZE][DIM_SIZE];
	double Dy[DIM_SIZE][DIM_SIZE];
	double y3xrho2[DIM_SIZE][DIM_SIZE];
	double y3yrho2[DIM_SIZE][DIM_SIZE];
	double Dxy3xrho2[DIM_SIZE][DIM_SIZE];
	double Dyy3yrho2[DIM_SIZE][DIM_SIZE];
	double absDxy3xrho2[DIM_SIZE][DIM_SIZE];
	double absDyy3yrho2[DIM_SIZE][DIM_SIZE];
	double absDxy3xrho2betarho2[DIM_SIZE][DIM_SIZE];
	double absDyy3yrho2betarho2[DIM_SIZE][DIM_SIZE];
	double maxabsDxy3xrho2betarho2[DIM_SIZE][DIM_SIZE];
	double maxabsDyy3yrho2betarho2[DIM_SIZE][DIM_SIZE];
	double signDx[DIM_SIZE][DIM_SIZE];
	double signDy[DIM_SIZE][DIM_SIZE];
	double xr[DIM_SIZE][DIM_SIZE];
	double xrrho2[DIM_SIZE][DIM_SIZE];
	double dxvx[DIM_SIZE][DIM_SIZE];
	double dyvy[DIM_SIZE][DIM_SIZE];
	double rho2dxvx[DIM_SIZE][DIM_SIZE];
	double rho2dyvy[DIM_SIZE][DIM_SIZE];

	fptalgebraobj.MAT_EQ<double, DIM_SIZE, DIM_SIZE>(
			Init, x);// x = Init;
	fptalgebraobj.ZEROS_MAT<double, DIM_SIZE, DIM_SIZE>(
			r);// r  = zeros(rows,cols)
	Dd<double, DIM_SIZE, DIM_SIZE>(
			r,vx, vy);// v = D(r);
	fptalgebraobj.ZEROS_MAT<double, DIM_SIZE, DIM_SIZE>(
			y2);// y2 = zeros(size(r));
	fptalgebraobj.ZEROS_MAT<double, DIM_SIZE, DIM_SIZE>(
			y3x);
	fptalgebraobj.ZEROS_MAT<double, DIM_SIZE, DIM_SIZE>(
			y3y);// y3 = zeros(size(v));

	// iteration
	double betadivrho2 = BETA / RHO2;
	fptalgebraobj.MAT_DOTMUL<double, DIM_SIZE, DIM_SIZE>(
			S, b, Sb); // S.*b
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
			Sb, "S.*b.csv", 5);
#endif
	fptalgebraobj.MAT_DOTSQUARE<double, DIM_SIZE, DIM_SIZE>(
			S,S2); // S.^2
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
			S2, "S.^2.csv", 5);
#endif
	fptalgebraobj.MAT_SCALAR_DOTADD<double, double, DIM_SIZE, DIM_SIZE>(
			RHO, S2, rhoS2); // rho+ (S.^2)
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
			rhoS2, "rho+S.^2.csv", 5);
#endif
	fptalgebraobj.MAT_DOTINV<double, DIM_SIZE, DIM_SIZE>(
			rhoS2,rhoS2inv); // 1./(rho+ (S.^2))
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
			rhoS2inv, "1./(rho+S.^2).csv", 5);
#endif

	fptalgebraobj.MAT_SCALAR_DOTMUL<double, double, DIM_SIZE, DIM_SIZE>(
			RHO, eigDtD, rhoeigDtD); // rho.*eigDtD
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
			rhoeigDtD, "rhoeigDtD.csv", 5);
#endif
	fptalgebraobj.MAT_SCALAR_DOTADD<double, double, DIM_SIZE, DIM_SIZE>(
			RHO, rhoeigDtD, rhorhoeigDtD); // rho+rho.*eigDtD
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
			rhorhoeigDtD, "rhorhoeigDtD.csv", 5);
#endif

	// start iteration
	for(int itr=0;itr<MAX_ITR;itr++){

		// std::cout << itr << "th iteration ... ... " << std::endl;

		// save last state
		fptalgebraobj.MAT_EQ<double, DIM_SIZE, DIM_SIZE>(
				x, xold);// xold = x;
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				xold, "xold.csv", 5);
#endif

		// solving the x-subproblem
		fptalgebraobj.MAT_SCALAR_DOTMUL<double, double, DIM_SIZE, DIM_SIZE>(
				r, RHO, rhor); // rho.*r
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				rhor, "rhor.csv", 5);
#endif
		fptalgebraobj.MAT_ADD<double, DIM_SIZE, DIM_SIZE>(
				Sb, rhor, Sbrhor); // S.*b  + rho.*r
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				Sbrhor, "Sbrhor.csv", 5);
#endif
		fptalgebraobj.MAT_SUB<double, DIM_SIZE, DIM_SIZE>(
				Sbrhor, y2, rhs1);// rhs = S.*b  +rho.*r - (y2)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				rhs1, "rhs1.csv", 5);
#endif
		fptalgebraobj.MAT_DOTMUL<double, DIM_SIZE, DIM_SIZE>(
				rhoS2inv, rhs1, x); // x = (1./(rho+ (S.^2))).*rhs
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				x, "x.csv", 5);
#endif

		// subproblem r
		fptalgebraobj.MAT_SCALAR_DOTMUL<double, double, DIM_SIZE, DIM_SIZE>(
				RHO, vx, rhovx);
		fptalgebraobj.MAT_SCALAR_DOTMUL<double, double, DIM_SIZE, DIM_SIZE>(
				RHO, vy, rhovy); // rho.*v
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				rhovx, "rhovx.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				rhovy, "rhovy.csv", 5);
#endif
		fptalgebraobj.MAT_SUB<double, DIM_SIZE, DIM_SIZE>(
				y3x, rhovx, y3xrhovx);
		fptalgebraobj.MAT_SUB<double, DIM_SIZE, DIM_SIZE>(
				y3y, rhovy, y3yrhovy); // y3- rho.*v
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				y3xrhovx, "y3xrhovx.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				y3yrhovy, "y3yrhovy.csv", 5);
#endif
		Dtd<double, DIM_SIZE, DIM_SIZE>(
				y3xrhovx, y3yrhovy, Dty3rhov);// Dt(y3- rho.*v)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				Dty3rhov, "Dty3rhov.csv", 5);
#endif
		fptalgebraobj.MAT_SCALAR_DOTMUL<double, double, DIM_SIZE, DIM_SIZE>(
				RHO, x, rhox); // rho.*x
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				rhox, "rhox.csv", 5);
#endif
		fptalgebraobj.MAT_SUB<double, DIM_SIZE, DIM_SIZE>(
				rhox, Dty3rhov, rhoxDty3rhov); // rho.*x  -Dt(y3- rho.*v)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				rhoxDty3rhov, "rhoxDty3rhov.csv", 5);
#endif
		fptalgebraobj.MAT_ADD<double, DIM_SIZE, DIM_SIZE>(
				y2, rhoxDty3rhov,
				rhs2); // rhs = y2 + rho.*x - Dt(y3 - rho.*v);
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				rhs2, "rhs2.csv", 5);
#endif
		fptalgebraobj.MAT_REAL2COMPLEX<double, DIM_SIZE, DIM_SIZE>(
				rhs2, y2rhoxDty3rhovcmplx);
		fftfftw3obj.fftw3_cfft_2d
				<double, DIM_SIZE, DIM_SIZE>(
				y2rhoxDty3rhovcmplx,
				fft2rhs); // fft2(rhs)
		fptalgebraobj.MAT_COMPLEX_DOTDIV_REAL
				<double, DIM_SIZE, DIM_SIZE>(
				fft2rhs,
				rhorhoeigDtD,
				fft2rhsrhorhoeigDtD);// fft2(rhs)./(rho+rho.*eigDtD)
		fftfftw3obj.fftw3_cifft_2d
				<double, DIM_SIZE, DIM_SIZE>(
				fft2rhsrhorhoeigDtD,
				ifft2fft2rhsrhorhoeigDtD);// ifft2( fft2(rhs)./(rho+rho.*eigDtD))
		fptalgebraobj.MAT_COMPLEX_GETREAL<double, DIM_SIZE, DIM_SIZE>(
				ifft2fft2rhsrhorhoeigDtD,
				r);// r = real( ifft2( fft2(rhs)./(rho+rho.*eigDtD)) );
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				r, "r.csv", 5);
#endif

		// subproblem v
		Dd<double, DIM_SIZE, DIM_SIZE>(r,Dx, Dy);// tempv=D(r);
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				Dx, "Dx.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				Dy, "Dy.csv", 5);
#endif
		fptalgebraobj.MAT_SCALAR_DOTDIV<double, double, DIM_SIZE, DIM_SIZE>(
				y3x, RHO2, y3xrho2);
		fptalgebraobj.MAT_SCALAR_DOTDIV<double, double, DIM_SIZE, DIM_SIZE>(
				y3y, RHO2, y3yrho2);// y3./rho2
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				y3xrho2, "y3xrho2.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				y3yrho2, "y3yrho2.csv", 5);
#endif
		fptalgebraobj.MAT_ADD<double, DIM_SIZE, DIM_SIZE>(
				Dx, y3xrho2, Dxy3xrho2);
		fptalgebraobj.MAT_ADD<double, DIM_SIZE, DIM_SIZE>(
				Dy, y3yrho2, Dyy3yrho2);// tempv+(y3./rho2)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				Dxy3xrho2, "Dxy3xrho2.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				Dyy3yrho2, "Dyy3yrho2.csv", 5);
#endif
		fptalgebraobj.MAT_ABS<double, DIM_SIZE, DIM_SIZE>(
				Dxy3xrho2, absDxy3xrho2);
		fptalgebraobj.MAT_ABS<double, DIM_SIZE, DIM_SIZE>(
				Dyy3yrho2, absDyy3yrho2);// abs(tempv1)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				absDxy3xrho2, "absDxy3xrho2.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				absDyy3yrho2, "absDyy3yrho2.csv", 5);
#endif
		fptalgebraobj.MAT_SCALAR_DOTSUB<double, double, DIM_SIZE, DIM_SIZE>(
				absDxy3xrho2, betadivrho2,
				absDxy3xrho2betarho2);
		fptalgebraobj.MAT_SCALAR_DOTSUB<double, double, DIM_SIZE, DIM_SIZE>(
				absDyy3yrho2, betadivrho2,
				absDyy3yrho2betarho2);// abs(tempv1)-(beta./rho2)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				absDxy3xrho2betarho2, "absDxy3xrho2betarho2.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				absDyy3yrho2betarho2, "absDyy3yrho2betarho2.csv", 5);
#endif
		fptalgebraobj.MAT_MAXCMP<double, double, DIM_SIZE, DIM_SIZE>(
				absDxy3xrho2betarho2, 0,
				maxabsDxy3xrho2betarho2);
		fptalgebraobj.MAT_MAXCMP<double, double, DIM_SIZE, DIM_SIZE>(
				absDyy3yrho2betarho2, 0,
				maxabsDyy3yrho2betarho2);// max(abs(tempv1)-(beta./rho2),0)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				maxabsDxy3xrho2betarho2, "maxabsDxy3xrho2betarho2.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				maxabsDyy3yrho2betarho2, "maxabsDyy3yrho2betarho2.csv", 5);
#endif
		fptalgebraobj.MAT_SIGN<double, DIM_SIZE, DIM_SIZE>(
				Dx, signDx);
		fptalgebraobj.MAT_SIGN<double, DIM_SIZE, DIM_SIZE>(
				Dy, signDy); // sign(tempv1)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				signDx, "signDx.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				signDy, "signDy.csv", 5);
#endif
		fptalgebraobj.MAT_DOTMUL<double, DIM_SIZE, DIM_SIZE>(
				signDx, maxabsDxy3xrho2betarho2,
				vx);
		fptalgebraobj.MAT_DOTMUL<double, DIM_SIZE, DIM_SIZE>(
				signDy, maxabsDyy3yrho2betarho2,
				vy);//v=sign(tempv1).*max(abs(tempv1)-(beta./rho2),0);
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				vx, "vx.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				vy, "vy.csv", 5);
#endif


		// update multipliers
		fptalgebraobj.MAT_SUB<double, DIM_SIZE, DIM_SIZE>(
				x, r, xr);// (x - r)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				xr, "xr.csv", 5);
#endif
		fptalgebraobj.MAT_SCALAR_DOTMUL<double, double, DIM_SIZE, DIM_SIZE>(
				xr, RHO2, xrrho2);// rho2*(x - r)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				xrrho2, "xrrho2.csv", 5);
#endif
		fptalgebraobj.MAT_ADD<double, DIM_SIZE, DIM_SIZE>(
				y2, xrrho2, y2);// y2 = y2 + rho2*(x - r);
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				y2, "y2.csv", 5);
#endif
		fptalgebraobj.MAT_SUB<double, DIM_SIZE, DIM_SIZE>(
				Dx, vx, dxvx);
		fptalgebraobj.MAT_SUB<double, DIM_SIZE, DIM_SIZE>(
				Dy, vy, dyvy);// tempv-v
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				dxvx, "dxvx.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				dyvy, "dyvy.csv", 5);
#endif
		fptalgebraobj.MAT_SCALAR_DOTMUL<double, double, DIM_SIZE, DIM_SIZE>(
				RHO2, dxvx, rho2dxvx);
		fptalgebraobj.MAT_SCALAR_DOTMUL<double, double, DIM_SIZE, DIM_SIZE>(
				RHO2, dyvy, rho2dyvy);// rho2*(tempv-v)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				rho2dxvx, "rho2dxvx.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				rho2dyvy, "rho2dyvy.csv", 5);
#endif
		fptalgebraobj.MAT_ADD<double, DIM_SIZE, DIM_SIZE>(
				y3x, rho2dxvx, y3x);
		fptalgebraobj.MAT_ADD<double, DIM_SIZE, DIM_SIZE>(
				y3y, rho2dyvy, y3y);// y3 = y3 + rho2*(tempv-v);
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				y3x, "y3x.csv", 5);
		fileioobj.writematrix_c<double, DIM_SIZE, DIM_SIZE>(
				y3y, "y3y.csv", 5);
#endif

	}
	// save result
	fptalgebraobj.MAT_EQ<double, DIM_SIZE, DIM_SIZE>(x, xout);// xout=x;

}

/*
// this is not right, since there is no speedup, this function is terminated
void ADMM_EXAMPLE_EIGEN_DOUBLE(Eigen::MatrixXd b, Eigen::MatrixXd eigDtD,
		Eigen::MatrixXd Init, Eigen::MatrixXd S, Eigen::MatrixXd &xout){

	FFT_FFTW3 fftfftw3obj;

	Eigen::MatrixXd x(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd r(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd vx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd vy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd y2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd y3x(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd y3y(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rhs1(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rhs2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd xold(DIM_SIZE, DIM_SIZE);

	Eigen::MatrixXd Sb(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rhor(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd Sbrhor(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd S2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rhoS2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rhoS2inv(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rhovx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rhovy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd y3xrhovx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd y3yrhovy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd Dty3rhov(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rhox(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rhoxDty3rhov(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rhoeigDtD(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rhorhoeigDtD(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd Dx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd Dy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd y3xrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd y3yrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd Dxy3xrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd Dyy3yrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd absDxy3xrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd absDyy3yrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd absDxy3xrho2betarho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd absDyy3yrho2betarho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd maxabsDxy3xrho2betarho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd maxabsDyy3yrho2betarho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd signDx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd signDy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd xr(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd xrrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd dxvx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd dyvy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rho2dxvx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd rho2dyvy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXcd y2rhoxDty3rhovcmplx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXcd fft2rhs(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXcd fft2rhsrhorhoeigDtD(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXcd ifft2fft2rhsrhorhoeigDtD(DIM_SIZE, DIM_SIZE);

	x = Init;
	r = Eigen::MatrixXd::Zero(DIM_SIZE, DIM_SIZE);
	y2 = Eigen::MatrixXd::Zero(DIM_SIZE, DIM_SIZE);
	y3x = Eigen::MatrixXd::Zero(DIM_SIZE, DIM_SIZE);
	y3y = Eigen::MatrixXd::Zero(DIM_SIZE, DIM_SIZE);
	D_eigen<Eigen::MatrixXd, DIM_SIZE, DIM_SIZE>( r,vx, vy );// v = D(r);

	// interation
	double betadivrho2 = BETA / RHO2;
	Sb = S.cwiseProduct(b);// S.*b
	S2 = S.cwiseAbs2();// S.^2
	rhoS2 = (S2.array() + RHO).matrix();
	rhoS2inv = rhoS2.cwiseInverse();
	rhoeigDtD = RHO * eigDtD;
	rhorhoeigDtD = (rhoeigDtD.array() + RHO).matrix();


	// start iteration
	for(int itr=0;itr<MAX_ITR;itr++){

		// std::cout << itr << "th iteration ... ... " << std::endl;

		// save last state
		xold = x;

		// solving the x-subproblem
		rhor = r * RHO;// rho.*r
		Sbrhor = Sb + rhor; // S.*b  + rho.*r
		rhs1 = Sbrhor + y2; // rhs = S.*b  +rho.*r - (y2)
		x = rhoS2inv.cwiseProduct(rhs1) ; // x = (1./(rho+ (S.^2))).*rhs

		// subproblem r
		rhovx = vx * RHO; rhovy = vy * RHO;// rho.*v
		y3xrhovx = y3x - rhovx;y3yrhovy = y3y - rhovy;// y3- rho.*v
		Dt_eigne<Eigen::MatrixXd, DIM_SIZE, DIM_SIZE>(
				y3xrhovx, y3yrhovy, Dty3rhov);// Dt(y3- rho.*v)
		rhox = RHO * x;// rho.*x
		rhoxDty3rhov = Dty3rhov - rhox;// rho.*x  -Dt(y3- rho.*v)
		rhs2 = y2 + rhoxDty3rhov; // rhs = y2 + rho.*x - Dt(y3 - rho.*v);
		for(int i=0;i<DIM_SIZE;i++){
			for(int j=0;j<DIM_SIZE;j++){
				y2rhoxDty3rhovcmplx(i,j).real(rhs2(i,j));
				y2rhoxDty3rhovcmplx(i,j).imag(0);
			}
		}
		fftfftw3obj.fftw3_cfft_2d
				<Eigen::MatrixXcd, DIM_SIZE, DIM_SIZE>(
				y2rhoxDty3rhovcmplx,
				fft2rhs); // fft2(rhs)
		fft2rhsrhorhoeigDtD = fft2rhs.cwiseProduct(rhorhoeigDtD);// fft2(rhs)./(rho+rho.*eigDtD)
		fftfftw3obj.fftw3_cifft_2d
				<Eigen::MatrixXcd, DIM_SIZE, DIM_SIZE>(
				fft2rhsrhorhoeigDtD,
				ifft2fft2rhsrhorhoeigDtD);// ifft2( fft2(rhs)./(rho+rho.*eigDtD))
		r = ifft2fft2rhsrhorhoeigDtD.real();// r = real( ifft2( fft2(rhs)./(rho+rho.*eigDtD)) );

		// subproblem v
		D_eigen<Eigen::MatrixXd, DIM_SIZE, DIM_SIZE>(r,Dx, Dy);// tempv=D(r);
		y3xrho2 = y3x * RHO;y3yrho2 = y3y * RHO;// y3./rho2
		Dxy3xrho2 = Dx + y3xrho2;Dyy3yrho2 = Dx + y3yrho2;// tempv+(y3./rho2)
		absDxy3xrho2 = Dxy3xrho2.cwiseAbs();
		absDyy3yrho2 = Dyy3yrho2.cwiseAbs();// abs(tempv1)
		absDxy3xrho2betarho2 = (absDxy3xrho2.array() - betadivrho2).matrix();
		absDyy3yrho2betarho2 = (absDyy3yrho2.array() - betadivrho2).matrix();// abs(tempv1)-(beta./rho2)
		maxabsDxy3xrho2betarho2 = absDxy3xrho2betarho2.cwiseMax(0);
		maxabsDyy3yrho2betarho2 = absDyy3yrho2betarho2.cwiseMax(0);// max(abs(tempv1)-(beta./rho2),0)
		signDx = Dx.cwiseSign();
		signDy = Dy.cwiseSign();// sign(tempv1)
		vx = signDx * maxabsDxy3xrho2betarho2;
		vy = signDy * maxabsDyy3yrho2betarho2;//v=sign(tempv1).*max(abs(tempv1)-(beta./rho2),0);


		// update multipliers
		xr = x - r;// (x - r)
		xrrho2 = xr * RHO2;// rho2*(x - r)
		y2 = y2 - xrrho2; // y2 = y2 + rho2*(x - r);
		dxvx = Dx - vx; dyvy = Dy - vy;// tempv-v
		rho2dxvx = RHO2 * dxvx;rho2dyvy = RHO2 * dyvy;// rho2*(tempv-v)
		y3x = y3x + rho2dxvx; y3y = y3y + rho2dyvy;// y3 = y3 + rho2*(tempv-v);

	}


}*/

void ADMM_DOUBLE(){

	// read data
	std::string bname = "./input/b.csv";
	std::string eigDtDname = "./input/eigDtD.csv";
	std::string Initname = "./input/Init.csv";
	std::string Sname = "./input/S.csv";
	Eigen::MatrixXd b(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd eigDtD(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd Init(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXd S(DIM_SIZE, DIM_SIZE);
	Eigen_Algebra Eigen_Algebra_obj;
	if(checkfileexist(bname)){
		b = readmatrix_double(bname);
		std::cout << "read b matrix" << std::endl << std::endl;
	}else{
		Eigen_Algebra_obj.RND_MAT<Eigen::MatrixXd, DIM_SIZE, DIM_SIZE>( b );
		writematrix_double(b, bname, 5);
		std::cout << "created the b matrix "
				  << bname
				  << std::endl;
	}
	if(checkfileexist(eigDtDname)){
		eigDtD = readmatrix_double(eigDtDname);
		std::cout << "read eigDtD matrix" << std::endl << std::endl;
	}else{
		Eigen_Algebra_obj.RND_MAT<Eigen::MatrixXd, DIM_SIZE, DIM_SIZE>( eigDtD );
		writematrix_double(eigDtD, eigDtDname, 5);
		std::cout << "created the eigDtD matrix "
				  << eigDtDname
				  << std::endl;
	}
	if(checkfileexist(Initname)){
		Init = readmatrix_double(Initname);
		std::cout << "read Init matrix" << std::endl << std::endl;
	}else{
		Eigen_Algebra_obj.RND_MAT<Eigen::MatrixXd, DIM_SIZE, DIM_SIZE>( Init );
		writematrix_double(Init, Initname, 5);
		std::cout << "created the Init matrix "
				  << Initname
				  << std::endl;
	}
	if(checkfileexist(Sname)){
		S = readmatrix_double(Sname);
		std::cout << "read S matrix" << std::endl << std::endl;
	}else{
		Eigen_Algebra_obj.RND_MAT<Eigen::MatrixXd, DIM_SIZE, DIM_SIZE>( S );
		writematrix_double(S, Sname, 5);
		std::cout << "created the S matrix "
				  << Sname
				  << std::endl;
	}

	// prepare data
	double b_c[DIM_SIZE][DIM_SIZE];
	Eigen::MatrixXd bT = b.transpose();
	memcpy(b_c,bT.data(),sizeof(double)*DIM_SIZE*DIM_SIZE);
	double eigDtD_c[DIM_SIZE][DIM_SIZE];
	Eigen::MatrixXd eigDtDT = eigDtD.transpose();
	memcpy(eigDtD_c,eigDtDT.data(),sizeof(double)*DIM_SIZE*DIM_SIZE);
	double Init_c[DIM_SIZE][DIM_SIZE];
	Eigen::MatrixXd InitT = Init.transpose();
	memcpy(Init_c,InitT.data(),sizeof(double)*DIM_SIZE*DIM_SIZE);
	double S_c[DIM_SIZE][DIM_SIZE];
	Eigen::MatrixXd ST = S.transpose();
	memcpy(S_c,ST.data(),sizeof(double)*DIM_SIZE*DIM_SIZE);
	std::cout << "Prepare Data DONE!" << std::endl;

	// Record start time
	auto start = std::chrono::high_resolution_clock::now();

	double xout[DIM_SIZE][DIM_SIZE];
	ADMM_EXAMPLE_DOUBLE(b_c, eigDtD_c, Init_c, S_c, xout);
#if defined(DEBUG_RESULT)
	fileioobj.writematrix_c<double,DIM_SIZE,DIM_SIZE>( xout, "xoutd.csv", 5 );
#endif
//	Eigen::MatrixXd xout( DIM_SIZE, DIM_SIZE );
//	ADMM_ALI_EXAMPLE_EIGEN_DOUBLE( b, eigDtD, Init, S, xout );
//#if defined(DEBUG_RESULT)
//	writematrix_double( xout, "xout.csv", 5 );
//#endif

	// Record end time
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time: " << elapsed.count() << " s\n";

}



void ADMM_EXAMPLE_FLOAT(float b[DIM_SIZE][DIM_SIZE],
						 	 float eigDtD[DIM_SIZE][DIM_SIZE],
						 	 float Init[DIM_SIZE][DIM_SIZE],
							 float S[DIM_SIZE][DIM_SIZE],
							 float xout[DIM_SIZE][DIM_SIZE]){

	Float_Point_Algebra fptalgebraobj;
	FFT_FFTW3 fftfftw3obj;

	// initiate variables
	float x[DIM_SIZE][DIM_SIZE];
	float r[DIM_SIZE][DIM_SIZE];
	float vx[DIM_SIZE][DIM_SIZE];
	float vy[DIM_SIZE][DIM_SIZE];
	float y2[DIM_SIZE][DIM_SIZE];
	float y3x[DIM_SIZE][DIM_SIZE];
	float y3y[DIM_SIZE][DIM_SIZE];
	float rhs1[DIM_SIZE][DIM_SIZE];
	float rhs2[DIM_SIZE][DIM_SIZE];
	float xold[DIM_SIZE][DIM_SIZE];

	float Sb[DIM_SIZE][DIM_SIZE];
	float rhor[DIM_SIZE][DIM_SIZE];
	float Sbrhor[DIM_SIZE][DIM_SIZE];
	float S2[DIM_SIZE][DIM_SIZE];
	float rhoS2[DIM_SIZE][DIM_SIZE];
	float rhoS2inv[DIM_SIZE][DIM_SIZE];
	float rhovx[DIM_SIZE][DIM_SIZE];
	float rhovy[DIM_SIZE][DIM_SIZE];
	float y3xrhovx[DIM_SIZE][DIM_SIZE];
	float y3yrhovy[DIM_SIZE][DIM_SIZE];
	float Dty3rhov[DIM_SIZE][DIM_SIZE];
	float rhox[DIM_SIZE][DIM_SIZE];
	float rhoxDty3rhov[DIM_SIZE][DIM_SIZE];
	float rhoeigDtD[DIM_SIZE][DIM_SIZE];
	float rhorhoeigDtD[DIM_SIZE][DIM_SIZE];
	Complex<float> y2rhoxDty3rhovcmplx[DIM_SIZE][DIM_SIZE];
	Complex<float> fft2rhs[DIM_SIZE][DIM_SIZE];
	Complex<float> fft2rhsrhorhoeigDtD[DIM_SIZE][DIM_SIZE];
	Complex<float> ifft2fft2rhsrhorhoeigDtD[DIM_SIZE][DIM_SIZE];
	float Dx[DIM_SIZE][DIM_SIZE];
	float Dy[DIM_SIZE][DIM_SIZE];
	float y3xrho2[DIM_SIZE][DIM_SIZE];
	float y3yrho2[DIM_SIZE][DIM_SIZE];
	float Dxy3xrho2[DIM_SIZE][DIM_SIZE];
	float Dyy3yrho2[DIM_SIZE][DIM_SIZE];
	float absDxy3xrho2[DIM_SIZE][DIM_SIZE];
	float absDyy3yrho2[DIM_SIZE][DIM_SIZE];
	float absDxy3xrho2betarho2[DIM_SIZE][DIM_SIZE];
	float absDyy3yrho2betarho2[DIM_SIZE][DIM_SIZE];
	float maxabsDxy3xrho2betarho2[DIM_SIZE][DIM_SIZE];
	float maxabsDyy3yrho2betarho2[DIM_SIZE][DIM_SIZE];
	float signDx[DIM_SIZE][DIM_SIZE];
	float signDy[DIM_SIZE][DIM_SIZE];
	float xr[DIM_SIZE][DIM_SIZE];
	float xrrho2[DIM_SIZE][DIM_SIZE];
	float dxvx[DIM_SIZE][DIM_SIZE];
	float dyvy[DIM_SIZE][DIM_SIZE];
	float rho2dxvx[DIM_SIZE][DIM_SIZE];
	float rho2dyvy[DIM_SIZE][DIM_SIZE];

	fptalgebraobj.MAT_EQ<float, DIM_SIZE, DIM_SIZE>(
			Init, x);// x = Init;
	fptalgebraobj.ZEROS_MAT<float, DIM_SIZE, DIM_SIZE>(
			r);// r  = zeros(rows,cols)
	Df<float, DIM_SIZE, DIM_SIZE>(
			r,vx, vy);// v = D(r);
	fptalgebraobj.ZEROS_MAT<float, DIM_SIZE, DIM_SIZE>(
			y2);// y2 = zeros(size(r));
	fptalgebraobj.ZEROS_MAT<float, DIM_SIZE, DIM_SIZE>(
			y3x);
	fptalgebraobj.ZEROS_MAT<float, DIM_SIZE, DIM_SIZE>(
			y3y);// y3 = zeros(size(v));

	// iteration
	float betadivrho2 = BETA / RHO2;
	fptalgebraobj.MAT_DOTMUL<float, DIM_SIZE, DIM_SIZE>(
			S, b, Sb); // S.*b
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
			Sb, "S.*b.csv", 5);
#endif
	fptalgebraobj.MAT_DOTSQUARE<float, DIM_SIZE, DIM_SIZE>(
			S,S2); // S.^2
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
			S2, "S.^2.csv", 5);
#endif
	fptalgebraobj.MAT_SCALAR_DOTADD<float, float, DIM_SIZE, DIM_SIZE>(
			RHO, S2, rhoS2); // rho+ (S.^2)
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
			rhoS2, "rho+S.^2.csv", 5);
#endif
	fptalgebraobj.MAT_DOTINV<float, DIM_SIZE, DIM_SIZE>(
			rhoS2,rhoS2inv); // 1./(rho+ (S.^2))
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
			rhoS2inv, "1./(rho+S.^2).csv", 5);
#endif

	fptalgebraobj.MAT_SCALAR_DOTMUL<float, float, DIM_SIZE, DIM_SIZE>(
			RHO, eigDtD, rhoeigDtD); // rho.*eigDtD
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
			rhoeigDtD, "rhoeigDtD.csv", 5);
#endif
	fptalgebraobj.MAT_SCALAR_DOTADD<float, float, DIM_SIZE, DIM_SIZE>(
			RHO, rhoeigDtD, rhorhoeigDtD); // rho+rho.*eigDtD
#if defined(DEBUG_ITER)
	fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
			rhorhoeigDtD, "rhorhoeigDtD.csv", 5);
#endif

	// start iteration
	for(int itr=0;itr<MAX_ITR;itr++){

		// std::cout << itr << "th iteration ... ... " << std::endl;

		// save last state
		fptalgebraobj.MAT_EQ<float, DIM_SIZE, DIM_SIZE>(
				x, xold);// xold = x;
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				xold, "xold.csv", 5);
#endif

		// solving the x-subproblem
		fptalgebraobj.MAT_SCALAR_DOTMUL<float, float, DIM_SIZE, DIM_SIZE>(
				r, RHO, rhor); // rho.*r
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				rhor, "rhor.csv", 5);
#endif
		fptalgebraobj.MAT_ADD<float, DIM_SIZE, DIM_SIZE>(
				Sb, rhor, Sbrhor); // S.*b  + rho.*r
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				Sbrhor, "Sbrhor.csv", 5);
#endif
		fptalgebraobj.MAT_SUB<float, DIM_SIZE, DIM_SIZE>(
				Sbrhor, y2, rhs1);// rhs = S.*b  +rho.*r - (y2)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				rhs1, "rhs1.csv", 5);
#endif
		fptalgebraobj.MAT_DOTMUL<float, DIM_SIZE, DIM_SIZE>(
				rhoS2inv, rhs1, x); // x = (1./(rho+ (S.^2))).*rhs
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				x, "x.csv", 5);
#endif

		// subproblem r
		fptalgebraobj.MAT_SCALAR_DOTMUL<float, float, DIM_SIZE, DIM_SIZE>(
				RHO, vx, rhovx);
		fptalgebraobj.MAT_SCALAR_DOTMUL<float, float, DIM_SIZE, DIM_SIZE>(
				RHO, vy, rhovy); // rho.*v
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				rhovx, "rhovx.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				rhovy, "rhovy.csv", 5);
#endif
		fptalgebraobj.MAT_SUB<float, DIM_SIZE, DIM_SIZE>(
				y3x, rhovx, y3xrhovx);
		fptalgebraobj.MAT_SUB<float, DIM_SIZE, DIM_SIZE>(
				y3y, rhovy, y3yrhovy); // y3- rho.*v
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				y3xrhovx, "y3xrhovx.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				y3yrhovy, "y3yrhovy.csv", 5);
#endif
		Dtf<float, DIM_SIZE, DIM_SIZE>(
				y3xrhovx, y3yrhovy, Dty3rhov);// Dt(y3- rho.*v)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				Dty3rhov, "Dty3rhov.csv", 5);
#endif
		fptalgebraobj.MAT_SCALAR_DOTMUL<float, float, DIM_SIZE, DIM_SIZE>(
				RHO, x, rhox); // rho.*x
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				rhox, "rhox.csv", 5);
#endif
		fptalgebraobj.MAT_SUB<float, DIM_SIZE, DIM_SIZE>(
				rhox, Dty3rhov, rhoxDty3rhov); // rho.*x  -Dt(y3- rho.*v)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				rhoxDty3rhov, "rhoxDty3rhov.csv", 5);
#endif
		fptalgebraobj.MAT_ADD<float, DIM_SIZE, DIM_SIZE>(
				y2, rhoxDty3rhov,
				rhs2); // rhs = y2 + rho.*x - Dt(y3 - rho.*v);
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				rhs2, "rhs2.csv", 5);
#endif
		fptalgebraobj.MAT_REAL2COMPLEX<float, DIM_SIZE, DIM_SIZE>(
				rhs2, y2rhoxDty3rhovcmplx);
		fftfftw3obj.fftw3_cfft_2d
				<float, DIM_SIZE, DIM_SIZE>(
				y2rhoxDty3rhovcmplx,
				fft2rhs); // fft2(rhs)
		fptalgebraobj.MAT_COMPLEX_DOTDIV_REAL
				<float, DIM_SIZE, DIM_SIZE>(
				fft2rhs,
				rhorhoeigDtD,
				fft2rhsrhorhoeigDtD);// fft2(rhs)./(rho+rho.*eigDtD)
		fftfftw3obj.fftw3_cifft_2d
				<float, DIM_SIZE, DIM_SIZE>(
				fft2rhsrhorhoeigDtD,
				ifft2fft2rhsrhorhoeigDtD);// ifft2( fft2(rhs)./(rho+rho.*eigDtD))
		fptalgebraobj.MAT_COMPLEX_GETREAL<float, DIM_SIZE, DIM_SIZE>(
				ifft2fft2rhsrhorhoeigDtD,
				r);// r = real( ifft2( fft2(rhs)./(rho+rho.*eigDtD)) );
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				r, "r.csv", 5);
#endif

		// subproblem v
		Df<float, DIM_SIZE, DIM_SIZE>(r,Dx, Dy);// tempv=D(r);
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				Dx, "Dx.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				Dy, "Dy.csv", 5);
#endif
		fptalgebraobj.MAT_SCALAR_DOTDIV<float, float, DIM_SIZE, DIM_SIZE>(
				y3x, RHO2, y3xrho2);
		fptalgebraobj.MAT_SCALAR_DOTDIV<float, float, DIM_SIZE, DIM_SIZE>(
				y3y, RHO2, y3yrho2);// y3./rho2
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				y3xrho2, "y3xrho2.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				y3yrho2, "y3yrho2.csv", 5);
#endif
		fptalgebraobj.MAT_ADD<float, DIM_SIZE, DIM_SIZE>(
				Dx, y3xrho2, Dxy3xrho2);
		fptalgebraobj.MAT_ADD<float, DIM_SIZE, DIM_SIZE>(
				Dy, y3yrho2, Dyy3yrho2);// tempv+(y3./rho2)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				Dxy3xrho2, "Dxy3xrho2.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				Dyy3yrho2, "Dyy3yrho2.csv", 5);
#endif
		fptalgebraobj.MAT_ABS<float, DIM_SIZE, DIM_SIZE>(
				Dxy3xrho2, absDxy3xrho2);
		fptalgebraobj.MAT_ABS<float, DIM_SIZE, DIM_SIZE>(
				Dyy3yrho2, absDyy3yrho2);// abs(tempv1)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				absDxy3xrho2, "absDxy3xrho2.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				absDyy3yrho2, "absDyy3yrho2.csv", 5);
#endif
		fptalgebraobj.MAT_SCALAR_DOTSUB<float, float, DIM_SIZE, DIM_SIZE>(
				absDxy3xrho2, betadivrho2,
				absDxy3xrho2betarho2);
		fptalgebraobj.MAT_SCALAR_DOTSUB<float, float, DIM_SIZE, DIM_SIZE>(
				absDyy3yrho2, betadivrho2,
				absDyy3yrho2betarho2);// abs(tempv1)-(beta./rho2)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				absDxy3xrho2betarho2, "absDxy3xrho2betarho2.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				absDyy3yrho2betarho2, "absDyy3yrho2betarho2.csv", 5);
#endif
		fptalgebraobj.MAT_MAXCMP<float, float, DIM_SIZE, DIM_SIZE>(
				absDxy3xrho2betarho2, 0,
				maxabsDxy3xrho2betarho2);
		fptalgebraobj.MAT_MAXCMP<float, float, DIM_SIZE, DIM_SIZE>(
				absDyy3yrho2betarho2, 0,
				maxabsDyy3yrho2betarho2);// max(abs(tempv1)-(beta./rho2),0)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				maxabsDxy3xrho2betarho2, "maxabsDxy3xrho2betarho2.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				maxabsDyy3yrho2betarho2, "maxabsDyy3yrho2betarho2.csv", 5);
#endif
		fptalgebraobj.MAT_SIGN<float, DIM_SIZE, DIM_SIZE>(
				Dx, signDx);
		fptalgebraobj.MAT_SIGN<float, DIM_SIZE, DIM_SIZE>(
				Dy, signDy); // sign(tempv1)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				signDx, "signDx.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				signDy, "signDy.csv", 5);
#endif
		fptalgebraobj.MAT_DOTMUL<float, DIM_SIZE, DIM_SIZE>(
				signDx, maxabsDxy3xrho2betarho2,
				vx);
		fptalgebraobj.MAT_DOTMUL<float, DIM_SIZE, DIM_SIZE>(
				signDy, maxabsDyy3yrho2betarho2,
				vy);//v=sign(tempv1).*max(abs(tempv1)-(beta./rho2),0);
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				vx, "vx.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				vy, "vy.csv", 5);
#endif


		// update multipliers
		fptalgebraobj.MAT_SUB<float, DIM_SIZE, DIM_SIZE>(
				x, r, xr);// (x - r)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				xr, "xr.csv", 5);
#endif
		fptalgebraobj.MAT_SCALAR_DOTMUL<float, float, DIM_SIZE, DIM_SIZE>(
				xr, RHO2, xrrho2);// rho2*(x - r)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				xrrho2, "xrrho2.csv", 5);
#endif
		fptalgebraobj.MAT_ADD<float, DIM_SIZE, DIM_SIZE>(
				y2, xrrho2, y2);// y2 = y2 + rho2*(x - r);
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				y2, "y2.csv", 5);
#endif
		fptalgebraobj.MAT_SUB<float, DIM_SIZE, DIM_SIZE>(
				Dx, vx, dxvx);
		fptalgebraobj.MAT_SUB<float, DIM_SIZE, DIM_SIZE>(
				Dy, vy, dyvy);// tempv-v
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				dxvx, "dxvx.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				dyvy, "dyvy.csv", 5);
#endif
		fptalgebraobj.MAT_SCALAR_DOTMUL<float, float, DIM_SIZE, DIM_SIZE>(
				RHO2, dxvx, rho2dxvx);
		fptalgebraobj.MAT_SCALAR_DOTMUL<float, float, DIM_SIZE, DIM_SIZE>(
				RHO2, dyvy, rho2dyvy);// rho2*(tempv-v)
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				rho2dxvx, "rho2dxvx.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				rho2dyvy, "rho2dyvy.csv", 5);
#endif
		fptalgebraobj.MAT_ADD<float, DIM_SIZE, DIM_SIZE>(
				y3x, rho2dxvx, y3x);
		fptalgebraobj.MAT_ADD<float, DIM_SIZE, DIM_SIZE>(
				y3y, rho2dyvy, y3y);// y3 = y3 + rho2*(tempv-v);
#if defined(DEBUG_ITER)
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				y3x, "y3x.csv", 5);
		fileioobj.writematrix_c<float, DIM_SIZE, DIM_SIZE>(
				y3y, "y3y.csv", 5);
#endif

	}
	// save result
	fptalgebraobj.MAT_EQ<float, DIM_SIZE, DIM_SIZE>(x, xout);// xout=x;

}

/*
// this is not right, since there is no speedup, this function is terminated
void ADMM_EXAMPLE_EIGEN_FLOAT(Eigen::MatrixXf b, Eigen::MatrixXf eigDtD,
		Eigen::MatrixXf Init, Eigen::MatrixXf S, Eigen::MatrixXf &xout){

	FFT_FFTW3 fftfftw3obj;

	Eigen::MatrixXf x(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf r(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf vx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf vy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf y2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf y3x(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf y3y(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rhs1(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rhs2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf xold(DIM_SIZE, DIM_SIZE);

	Eigen::MatrixXf Sb(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rhor(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf Sbrhor(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf S2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rhoS2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rhoS2inv(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rhovx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rhovy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf y3xrhovx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf y3yrhovy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf Dty3rhov(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rhox(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rhoxDty3rhov(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rhoeigDtD(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rhorhoeigDtD(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf Dx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf Dy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf y3xrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf y3yrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf Dxy3xrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf Dyy3yrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf absDxy3xrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf absDyy3yrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf absDxy3xrho2betarho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf absDyy3yrho2betarho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf maxabsDxy3xrho2betarho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf maxabsDyy3yrho2betarho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf signDx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf signDy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf xr(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf xrrho2(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf dxvx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf dyvy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rho2dxvx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf rho2dyvy(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXcd y2rhoxDty3rhovcmplx(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXcd fft2rhs(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXcd fft2rhsrhorhoeigDtD(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXcd ifft2fft2rhsrhorhoeigDtD(DIM_SIZE, DIM_SIZE);

	x = Init;
	r = Eigen::MatrixXf::Zero(DIM_SIZE, DIM_SIZE);
	y2 = Eigen::MatrixXf::Zero(DIM_SIZE, DIM_SIZE);
	y3x = Eigen::MatrixXf::Zero(DIM_SIZE, DIM_SIZE);
	y3y = Eigen::MatrixXf::Zero(DIM_SIZE, DIM_SIZE);
	D_eigen<Eigen::MatrixXf, DIM_SIZE, DIM_SIZE>( r,vx, vy );// v = D(r);

	// interation
	float betadivrho2 = BETA / RHO2;
	Sb = S.cwiseProduct(b);// S.*b
	S2 = S.cwiseAbs2();// S.^2
	rhoS2 = (S2.array() + RHO).matrix();
	rhoS2inv = rhoS2.cwiseInverse();
	rhoeigDtD = RHO * eigDtD;
	rhorhoeigDtD = (rhoeigDtD.array() + RHO).matrix();


	// start iteration
	for(int itr=0;itr<MAX_ITR;itr++){

		// std::cout << itr << "th iteration ... ... " << std::endl;

		// save last state
		xold = x;

		// solving the x-subproblem
		rhor = r * RHO;// rho.*r
		Sbrhor = Sb + rhor; // S.*b  + rho.*r
		rhs1 = Sbrhor + y2; // rhs = S.*b  +rho.*r - (y2)
		x = rhoS2inv.cwiseProduct(rhs1) ; // x = (1./(rho+ (S.^2))).*rhs

		// subproblem r
		rhovx = vx * RHO; rhovy = vy * RHO;// rho.*v
		y3xrhovx = y3x - rhovx;y3yrhovy = y3y - rhovy;// y3- rho.*v
		Dt_eigne<Eigen::MatrixXf, DIM_SIZE, DIM_SIZE>(
				y3xrhovx, y3yrhovy, Dty3rhov);// Dt(y3- rho.*v)
		rhox = RHO * x;// rho.*x
		rhoxDty3rhov = Dty3rhov - rhox;// rho.*x  -Dt(y3- rho.*v)
		rhs2 = y2 + rhoxDty3rhov; // rhs = y2 + rho.*x - Dt(y3 - rho.*v);
		for(int i=0;i<DIM_SIZE;i++){
			for(int j=0;j<DIM_SIZE;j++){
				y2rhoxDty3rhovcmplx(i,j).real(rhs2(i,j));
				y2rhoxDty3rhovcmplx(i,j).imag(0);
			}
		}
		fftfftw3obj.fftw3_cfft_2d
				<Eigen::MatrixXcd, DIM_SIZE, DIM_SIZE>(
				y2rhoxDty3rhovcmplx,
				fft2rhs); // fft2(rhs)
		fft2rhsrhorhoeigDtD = fft2rhs.cwiseProduct(rhorhoeigDtD);// fft2(rhs)./(rho+rho.*eigDtD)
		fftfftw3obj.fftw3_cifft_2d
				<Eigen::MatrixXcd, DIM_SIZE, DIM_SIZE>(
				fft2rhsrhorhoeigDtD,
				ifft2fft2rhsrhorhoeigDtD);// ifft2( fft2(rhs)./(rho+rho.*eigDtD))
		r = ifft2fft2rhsrhorhoeigDtD.real();// r = real( ifft2( fft2(rhs)./(rho+rho.*eigDtD)) );

		// subproblem v
		D_eigen<Eigen::MatrixXf, DIM_SIZE, DIM_SIZE>(r,Dx, Dy);// tempv=D(r);
		y3xrho2 = y3x * RHO;y3yrho2 = y3y * RHO;// y3./rho2
		Dxy3xrho2 = Dx + y3xrho2;Dyy3yrho2 = Dx + y3yrho2;// tempv+(y3./rho2)
		absDxy3xrho2 = Dxy3xrho2.cwiseAbs();
		absDyy3yrho2 = Dyy3yrho2.cwiseAbs();// abs(tempv1)
		absDxy3xrho2betarho2 = (absDxy3xrho2.array() - betadivrho2).matrix();
		absDyy3yrho2betarho2 = (absDyy3yrho2.array() - betadivrho2).matrix();// abs(tempv1)-(beta./rho2)
		maxabsDxy3xrho2betarho2 = absDxy3xrho2betarho2.cwiseMax(0);
		maxabsDyy3yrho2betarho2 = absDyy3yrho2betarho2.cwiseMax(0);// max(abs(tempv1)-(beta./rho2),0)
		signDx = Dx.cwiseSign();
		signDy = Dy.cwiseSign();// sign(tempv1)
		vx = signDx * maxabsDxy3xrho2betarho2;
		vy = signDy * maxabsDyy3yrho2betarho2;//v=sign(tempv1).*max(abs(tempv1)-(beta./rho2),0);


		// update multipliers
		xr = x - r;// (x - r)
		xrrho2 = xr * RHO2;// rho2*(x - r)
		y2 = y2 - xrrho2; // y2 = y2 + rho2*(x - r);
		dxvx = Dx - vx; dyvy = Dy - vy;// tempv-v
		rho2dxvx = RHO2 * dxvx;rho2dyvy = RHO2 * dyvy;// rho2*(tempv-v)
		y3x = y3x + rho2dxvx; y3y = y3y + rho2dyvy;// y3 = y3 + rho2*(tempv-v);

	}


}*/

void ADMM_FLOAT(){

	// read data
	std::string bname = "./input/b.csv";
	std::string eigDtDname = "./input/eigDtD.csv";
	std::string Initname = "./input/Init.csv";
	std::string Sname = "./input/S.csv";
	Eigen::MatrixXf b(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf eigDtD(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf Init(DIM_SIZE, DIM_SIZE);
	Eigen::MatrixXf S(DIM_SIZE, DIM_SIZE);
	Eigen_Algebra Eigen_Algebra_obj;
	if(checkfileexist(bname)){
		b = readmatrix_float(bname);
		std::cout << "read b matrix" << std::endl << std::endl;
	}else{
		Eigen_Algebra_obj.RND_MAT<Eigen::MatrixXf, DIM_SIZE, DIM_SIZE>( b );
		writematrix_float(b, bname, 5);
		std::cout << "created the b matrix "
				  << bname
				  << std::endl;
	}
	if(checkfileexist(eigDtDname)){
		eigDtD = readmatrix_float(eigDtDname);
		std::cout << "read eigDtD matrix" << std::endl << std::endl;
	}else{
		Eigen_Algebra_obj.RND_MAT<Eigen::MatrixXf, DIM_SIZE, DIM_SIZE>( eigDtD );
		writematrix_float(eigDtD, eigDtDname, 5);
		std::cout << "created the eigDtD matrix "
				  << eigDtDname
				  << std::endl;
	}
	if(checkfileexist(Initname)){
		Init = readmatrix_float(Initname);
		std::cout << "read Init matrix" << std::endl << std::endl;
	}else{
		Eigen_Algebra_obj.RND_MAT<Eigen::MatrixXf, DIM_SIZE, DIM_SIZE>( Init );
		writematrix_float(Init, Initname, 5);
		std::cout << "created the Init matrix "
				  << Initname
				  << std::endl;
	}
	if(checkfileexist(Sname)){
		S = readmatrix_float(Sname);
		std::cout << "read S matrix" << std::endl << std::endl;
	}else{
		Eigen_Algebra_obj.RND_MAT<Eigen::MatrixXf, DIM_SIZE, DIM_SIZE>( S );
		writematrix_float(S, Sname, 5);
		std::cout << "created the S matrix "
				  << Sname
				  << std::endl;
	}

	// prepare data
	float b_c[DIM_SIZE][DIM_SIZE];
	Eigen::MatrixXf bT = b.transpose();
	memcpy(b_c,bT.data(),sizeof(float)*DIM_SIZE*DIM_SIZE);
	float eigDtD_c[DIM_SIZE][DIM_SIZE];
	Eigen::MatrixXf eigDtDT = eigDtD.transpose();
	memcpy(eigDtD_c,eigDtDT.data(),sizeof(float)*DIM_SIZE*DIM_SIZE);
	float Init_c[DIM_SIZE][DIM_SIZE];
	Eigen::MatrixXf InitT = Init.transpose();
	memcpy(Init_c,InitT.data(),sizeof(float)*DIM_SIZE*DIM_SIZE);
	float S_c[DIM_SIZE][DIM_SIZE];
	Eigen::MatrixXf ST = S.transpose();
	memcpy(S_c,ST.data(),sizeof(float)*DIM_SIZE*DIM_SIZE);
	std::cout << "Prepare Data DONE!" << std::endl;

	// Record start time
	auto start = std::chrono::high_resolution_clock::now();

	float xout[DIM_SIZE][DIM_SIZE];
	ADMM_EXAMPLE_FLOAT(b_c, eigDtD_c, Init_c, S_c, xout);
#if defined(DEBUG_RESULT)
	fileioobj.writematrix_c<float,DIM_SIZE,DIM_SIZE>( xout, "xoutf.csv", 5 );
#endif
//	Eigen::MatrixXf xout( DIM_SIZE, DIM_SIZE );
//	ADMM_ALI_EXAMPLE_EIGEN_float( b, eigDtD, Init, S, xout );
//#if defined(DEBUG_RESULT)
//	writematrix_float( xout, "xout.csv", 5 );
//#endif

	// Record end time
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = finish - start;
	std::cout << "Elapsed time: " << elapsed.count() << " s\n";

}

int main(int argc, char** argv){
	ADMM_DOUBLE();
	ADMM_FLOAT();
	return 0;
}
