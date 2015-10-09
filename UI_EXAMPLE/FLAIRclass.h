#pragma once

class FLAIRclass : public CWnd
{
	DECLARE_DYNAMIC(FLAIRclass)

public:

	FLAIRclass();
	virtual ~FLAIRclass();

public:

	KD_PHOTOMETRIC		photometric;

	unsigned char *		srcData;
	unsigned char *		dstData;

	int					samplePerPixel, width, height, srcStep, dibStep, bitsAllocated, bitsStored;
	int					pixelRepresentation, gamma;

	double				windowWidth, windowCenter, windowWidthTemp, windowCenterTemp;

	void				OpenFLAIRFile(CString	* path, int gamma);

protected:

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


