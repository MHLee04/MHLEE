#pragma once

class DTIclass : public CWnd
{
	DECLARE_DYNAMIC(DTIclass)

public:

	DTIclass();
	virtual ~DTIclass();

public:

	KD_PHOTOMETRIC		photometric;

	unsigned char *		srcData;
	unsigned char *		dstData;

	int					samplePerPixel, width, height, srcStep, dibStep, bitsAllocated, bitsStored;
	int					pixelRepresentation, gamma;

	double				windowWidth, windowCenter, windowWidthTemp, windowCenterTemp;

	void				OpenDTIFile(CString		* path, int gamma);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


