#pragma once

class FACAL : public CWnd
{
	DECLARE_DYNAMIC(FACAL)

public:

	FACAL();
	virtual ~FACAL();

public:

	KD_PHOTOMETRIC		photometric;
	CScrollBar			scrollBar, tscrollBar;

	unsigned char *		srcData;
	unsigned char *		dstData;

	int					samplePerPixel, width, height, srcStep, dibStep, bitsAllocated, bitsStored;
	int					pixelRepresentation, gamma;

	double				windowWidth, windowCenter, windowWidthTemp, windowCenterTemp;
	double				d_mtx[256][256][15];
	double				vm[6][15];
	double				x1,x2,x3;
	double				rfa[256*256];

	double				MG(int gamma);
	double				Determinant(double *psrc,int sizeM);
	double				CUBICEQ(double a, double b, double c);
	double				Cofactor(double *psrc, int sizeM, double *pdst);
	double				invmat(double *psrc, int sizeM, double *pdst);
	
	void				eigen();

protected:

	DECLARE_MESSAGE_MAP()

public:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


