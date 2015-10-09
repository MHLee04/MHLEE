#include "stdafx.h"
#include "UI_EXAMPLE.h"
#include "FACAL.h"
#include "MainFrm.h"
#include "FADIB.h"
#include "FILEPROC.h"
#include "ToolWnd.h"
#include "math.h"
#include "DTIclass.h"
#include "DTIDIB.h"
#include "ViewWnd.h"
#include <complex>
IMPLEMENT_DYNAMIC(FACAL, CWnd)



	FACAL::FACAL()
{
	dstData     = NULL;
	srcData     = NULL;
}

FACAL::~FACAL()
{
	if(dstData)
		delete[] dstData;
	if(srcData)
		delete[] srcData;
}

BEGIN_MESSAGE_MAP(FACAL, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int FACAL::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

double FACAL::MG(int gamma)
{
	CMainFrame * pFrame  = (CMainFrame *) AfxGetApp()->m_pMainWnd;

	double				mMG[15][6];
	double				tMG[6][15];
	double				temMG[6*6];
	double				invMG[6*6];
	double				src_mtx[256][256];
	int					bval = 1000;
	memset(temMG,0,36*8);
	memset(vm,0,6*15*8);

	for(int j = 0; j <15; j++)
	{
		tMG[0][j]=mMG[j][0] = pFrame->toolWnd->BV[0][j] * pFrame->toolWnd->BV[0][j];

		tMG[1][j]=mMG[j][1] =	2 * pFrame->toolWnd->BV[0][j] * pFrame->toolWnd->BV[1][j];

		tMG[2][j]=mMG[j][2] = 2 * pFrame->toolWnd->BV[0][j] * pFrame->toolWnd->BV[2][j];

		tMG[3][j]=mMG[j][3] =	pFrame->toolWnd->BV[1][j] * pFrame->toolWnd->BV[1][j];

		tMG[4][j]=mMG[j][4] =	2 * pFrame->toolWnd->BV[1][j] * pFrame->toolWnd->BV[2][j];

		tMG[5][j]=mMG[j][5] =	pFrame->toolWnd->BV[2][j] * pFrame->toolWnd->BV[2][j];

	}

	for (int i = 0; i<6; i++)
	{
		for (int k = 0; k<6; k++)
		{
			for(int j = 0; j<15; j++)
			{
				temMG[i+6*k] += tMG[i][j] * mMG[j][k];
			}
		}
	}

	invmat(temMG, 6, invMG);

	for (int i = 0; i<6; i++)
	{
		for (int k = 0; k<6; k++)
		{
			for(int j = 0; j<15; j++)
			{
				vm[i][j] += invMG[i*6+k] * tMG[k][j];
			}
		}
	}

	pFrame->fileopen->OpenDTIFile(pFrame->fileproc->DTI, gamma);

	short * src = (short *) pFrame->fileopen->dstData;

	short * temfa = NULL; 

	for(int i = 0;i < pFrame->fileopen->height;i++)
	{
		for(int j = 0;j < pFrame->fileopen->width;j++)
		{
			src_mtx[i][j] = src[i * pFrame->fileopen->width + j];
		}
	}

	for(int k = 0; k < 15; k++)
	{
		pFrame->fileopen->OpenDTIFile(pFrame->fileproc->DTI, gamma + 58 * (k+1));

		temfa = (short *) pFrame->fileopen->dstData;

		for(int i = 0;i < pFrame->fileopen->height;i++)
		{
			for(int j = 0;j < pFrame->fileopen->width;j++)
			{
				if((temfa[i * pFrame->fileopen->width + j] != 0) && (src_mtx[i][j] != 0))
				{
					d_mtx[i][j][k] = log(src_mtx[i][j]/(temfa[i * pFrame->fileopen->width + j]));
				}
				else
				{
					d_mtx[i][j][k] = 0;
				}
			}
		}
	}
	return **vm, *** d_mtx;

}

double FACAL::Determinant(double *psrc,int sizeM)
{
	double det = 0;

	if (sizeM == 1)
	{
		det = psrc[0];
	} 
	else if (sizeM == 2)
	{
		det = psrc[0] * psrc[3] - psrc[1] * psrc[2];
	}
	else
	{
		for(int i=0; i<sizeM; i++)
		{
			double * m = new double [ (sizeM-1)*(sizeM-1) ];
			memset(m,0,(sizeM-1)*(sizeM-1)*8);

			int k=0;

			for(int i2=1; i2<sizeM; i2++)
			{
				for(int j2=0; j2<sizeM; j2++)
				{
					if(j2==i)
						continue;
					m[k] = psrc[sizeM*i2+j2];
					k += 1;
				}
			}

			det += pow(-1.0,i)*psrc[i]*Determinant(m,sizeM-1);

			if(m)
				delete[] m;
		}
	}

	return det;
}

double FACAL::Cofactor(double *psrc, int sizeM, double *pdst)
{
	double * c = new double [(sizeM - 1) * (sizeM - 1)];

	memset(c, 0, (sizeM - 1) * (sizeM - 1) * 8);

	for(int i = 0; i < sizeM; i++)
	{
		for(int j = 0; j < sizeM; j++)
		{
			int k = 0;

			for(int i2 = 0; i2 < sizeM; i2++)
			{
				for(int j2 = 0; j2 < sizeM; j2++)
				{
					if(i2 == i)
						continue;
					if(j2 == j)
						continue;
					c[k] = psrc[i2 * sizeM + j2];
					k++;
				}
			}

			pdst[i * sizeM + j] = pow(-1.0, i + j) * Determinant(c, sizeM - 1);
		}
	}

	return *pdst;
}

double FACAL::invmat(double *psrc, int sizeM, double *pdst)
{
	double		D = Determinant(psrc,sizeM);
	double *	c = new double [ sizeM*sizeM ];

	memset(c,0, sizeM * sizeM * 8);

	Cofactor(psrc, sizeM, c);

	for(int i = 0; i < sizeM; i++)
	{
		for(int j = 0; j < sizeM; j++)
		{
			pdst[i + sizeM * j] = c[j + sizeM * i] / D;
		}
	}

	if(c)
		delete[] c;

	return *pdst;
}

void FACAL::eigen()
{
	FACAL m;

	double a, b, c, d, e, f;

	double coef1, coef2, coef3, mean;

	double eigen_temp[256][256][6];

	memset(eigen_temp, 0, 6 * 255 * 255 * 8);

	memset(rfa, 0, 256 * 256 * 8);

	for(int i = 0; i < 256;i++)
	{
		for(int j = 0; j < 256;j++)
		{
			for(int k = 0; k < 6;k++)
			{
				for(int h = 0; h < 15;h++)
				{
					eigen_temp[i][j][k]	+= vm[k][h]*d_mtx[i][j][h];						
				}
			}
		}
	}

	for(int i = 0; i < 256;i++)
	{
		for(int j = 0; j < 256;j++)
		{
			a		=eigen_temp[i][j][0];
			b		= eigen_temp[i][j][1];
			c		= eigen_temp[i][j][2];
			d		= eigen_temp[i][j][3];
			e		=eigen_temp[i][j][4];
			f		= eigen_temp[i][j][5];

			coef1	= -1. * (a + d + f);
			coef2	= a * d + a * f + f * d - e * e - b * b - c * c;
			coef3	= -2. * b * e * c + d * c * c -1. * a * f * d + a * e * e + b * b * f;

			CUBICEQ(coef1, coef2, coef3);

			if(x3 >x2 && x3>x1)
			{
				if (x3 == 0)
					continue;
				if (x3 < 0)
					continue;
				if(x2 < 0)
					x2 = 0;
				if (x1 < 0)
					x1 = 0;
			}
			if(x2 >x3 && x2>x1)
			{
				if (x2 == 0)
					continue;
				if (x2 < 0)
					continue;
				if(x1 < 0)
					x1 = 0;
				if (x3 < 0)
					x3 = 0;
			}
			if(x1 >x2 && x1>x3)
			{
				if (x1 == 0)
					continue;
				if (x1 < 0)
					continue;
				if(x3 < 0)
					x3 = 0;
				if (x2 < 0)
					x2 = 0;
			}

			if(x1 == 0 && x2 ==0 && x1 ==0)
			{
				continue;
			}
			mean = (x1 + x2 + x3) / 3;

			rfa[i * 256 + j] = sqrt(1.5 * ((x1 - mean) * (x1 - mean) + (x2 - mean) * (x2 - mean) 
				+ (x3 - mean) * (x3 - mean)) / (x1 * x1 + x2 * x2 + x3 * x3));
		}
	}
}

double FACAL::CUBICEQ(double a, double b, double c)
{
	int    temp;
	double f, g, h, i, j, k, L, M, N,p,R,S,T, U;

	// x^3 + ax^2 + bx + c = 0

	f			= ( 3. * b - a * a ) / 3.0;

	g			= ( 2. * a * a * a - 9. * a * b + 27. * c) / 27.0;
	h			= (g * g / 4.) + (f * f * f / 27.);


	if ( h < 0 )
	{
		i = sqrt((g * g/4.) - h);

		j = pow(i,1./3.);
		k = acos(-1.* (g /(2.*i)));
		L = j*(-1.);
		M = cos(k / 3.);
		N = sqrt(3.)* sin(k/3.);
		p = a / 3. * (-1.);
		x1 = 2 * j * cos( k / 3.)- (a / 3);
		x2 = L * (M + N) + p;
		x3 = L * (M - N) + p;
	}
	else if (h > 0 )
	{
		R = -1.* (g/2.) + sqrt(h);
		S = pow(R,1./3.);
		T = -1.*(g/2.) - sqrt(h);
		U = pow(T, 1./3.);
		x1 = S+U - a/3.;
		
		x2 = -1.*(S+U)/2. -b/3.;
		//x2.imag = (S-U)*sqrt(3)/2;
		x3 = -1.*(S+U)/2. -b/3.;
		//x3.imag = -1.*(S-U)*sqrt(3.)/2.;

	}
	else
	{
		x1 = x2 = x3 = pow(c,1./3.);
	}
	return 0;
}


