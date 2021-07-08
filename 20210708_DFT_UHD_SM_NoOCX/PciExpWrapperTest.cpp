// PciExpWrapper.cpp: interface for the CPciExpWrapper class.
//
//////////////////////////////////////////////////////////////////////
int	CPciExpWrapper::DFT3_UHDPuzzle(int nMode, unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight, int nRotate) {
	int		i, j, k, l;

	int		nBmpSrcOff, nBmpDestOff;

	nBmpSrcOff = (nWidth * 3);		//	150303 cks 수정
	nBmpDestOff = (nWidth * 3 + 3) & 0xfffffffc;	// 150303 cks 수정

	unsigned char *bufTmp;
	bufTmp = (unsigned char *)malloc(nWidth*nHeight * 3);

	switch (nMode) {
	case 0:
		if (nRotate == 0) {
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 16; j = j + 1) {
					for (k = 0; k < 4; k = k + 1) {
						for (l = 0; l < 12; l = l + 1)
							bufOut[(nHeight - 1 - i)*nWidth * 3 + j * 3 * 4 * 4 + k * 12 + l] = bufIn[i*nWidth * 3 + j * 3 * 4 + 960 * 3 * k + l];
					}
				}
			}
		}
		else {
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 16; j = j + 1) {
					for (k = 0; k < 4; k = k + 1) {
						for (l = 0; l < 4; l = l + 1) {
							bufOut[(i)*nWidth * 3 + j * 3 * 4 * 4 + k * 12 + 4 * l + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - 960 * 3 * k - 4 * l + 0];
							bufOut[(i)*nWidth * 3 + j * 3 * 4 * 4 + k * 12 + 4 * l + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - 960 * 3 * k - 4 * l + 1];
							bufOut[(i)*nWidth * 3 + j * 3 * 4 * 4 + k * 12 + 4 * l + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - 960 * 3 * k - 4 * l + 2];
						}
					}
				}
			}

		}
		break;
	case 1:		//	150224 - cks 2 Division 대응
		if (nRotate == 0) {
			for (i = 0; i < nHeight; i++) {
				for (j = 0; j < nWidth / 16; j++) {
					for (k = 0; k < 12; k++) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 0 * nWidth * 3 / 2 + 24 * j + k] = bufIn[i*nWidth * 3 + nWidth * 0 * 3 / 4 + j * 12 + k];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 0 * nWidth * 3 / 2 + 24 * j + 12 + k] = bufIn[i*nWidth * 3 + nWidth * 1 * 3 / 4 + j * 12 + k];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 1 * nWidth * 3 / 2 + 24 * j + k] = bufIn[i*nWidth * 3 + nWidth * 2 * 3 / 4 + j * 12 + k];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 1 * nWidth * 3 / 2 + 24 * j + 12 + k] = bufIn[i*nWidth * 3 + nWidth * 3 * 3 / 4 + j * 12 + k];
					}
				}
			}
		}
		else {
			for (i = 0; i < nHeight; i++) {
				for (j = 0; j < nWidth / 16; j++) {
					for (k = 0; k < 4; k++) {
						bufOut[(i)*nWidth * 3 + 0 * nWidth * 3 / 2 + 24 * j + 3 * k + 0] = bufIn[(i + 1)*nWidth * 3 - nWidth * 0 * 3 / 4 - j * 12 - 3 * k + 0 - 3];
						bufOut[(i)*nWidth * 3 + 0 * nWidth * 3 / 2 + 24 * j + 12 + 3 * k + 0] = bufIn[(i + 1)*nWidth * 3 - nWidth * 1 * 3 / 4 - j * 12 - 3 * k + 0 - 3];
						bufOut[(i)*nWidth * 3 + 1 * nWidth * 3 / 2 + 24 * j + 3 * k + 0] = bufIn[(i + 1)*nWidth * 3 - nWidth * 2 * 3 / 4 - j * 12 - 3 * k + 0 - 3];
						bufOut[(i)*nWidth * 3 + 1 * nWidth * 3 / 2 + 24 * j + 12 + 3 * k + 0] = bufIn[(i + 1)*nWidth * 3 - nWidth * 3 * 3 / 4 - j * 12 - 3 * k + 0 - 3];
						bufOut[(i)*nWidth * 3 + 0 * nWidth * 3 / 2 + 24 * j + 3 * k + 1] = bufIn[(i + 1)*nWidth * 3 - nWidth * 0 * 3 / 4 - j * 12 - 3 * k + 1 - 3];
						bufOut[(i)*nWidth * 3 + 0 * nWidth * 3 / 2 + 24 * j + 12 + 3 * k + 1] = bufIn[(i + 1)*nWidth * 3 - nWidth * 1 * 3 / 4 - j * 12 - 3 * k + 1 - 3];
						bufOut[(i)*nWidth * 3 + 1 * nWidth * 3 / 2 + 24 * j + 3 * k + 1] = bufIn[(i + 1)*nWidth * 3 - nWidth * 2 * 3 / 4 - j * 12 - 3 * k + 1 - 3];
						bufOut[(i)*nWidth * 3 + 1 * nWidth * 3 / 2 + 24 * j + 12 + 3 * k + 1] = bufIn[(i + 1)*nWidth * 3 - nWidth * 3 * 3 / 4 - j * 12 - 3 * k + 1 - 3];
						bufOut[(i)*nWidth * 3 + 0 * nWidth * 3 / 2 + 24 * j + 3 * k + 2] = bufIn[(i + 1)*nWidth * 3 - nWidth * 0 * 3 / 4 - j * 12 - 3 * k + 2 - 3];
						bufOut[(i)*nWidth * 3 + 0 * nWidth * 3 / 2 + 24 * j + 12 + 3 * k + 2] = bufIn[(i + 1)*nWidth * 3 - nWidth * 1 * 3 / 4 - j * 12 - 3 * k + 2 - 3];
						bufOut[(i)*nWidth * 3 + 1 * nWidth * 3 / 2 + 24 * j + 3 * k + 2] = bufIn[(i + 1)*nWidth * 3 - nWidth * 2 * 3 / 4 - j * 12 - 3 * k + 2 - 3];
						bufOut[(i)*nWidth * 3 + 1 * nWidth * 3 / 2 + 24 * j + 12 + 3 * k + 2] = bufIn[(i + 1)*nWidth * 3 - nWidth * 3 * 3 / 4 - j * 12 - 3 * k + 2 - 3];

					}
				}
			}

		}
		break;
	case 2:			//	150224 - cks 4 division 대응
		if (nRotate == 0) {
			//// Y17&18&19
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 8; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 0]; //CH0_BLUE
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 1]; //CH0_GREEN
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 2]; //CH0_RED
					}
					for (k = 0; k < 2; k = k + 1) {
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 0];
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 1];
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 2];
					}
				}
				for (j = nWidth / 8; j < nWidth / 4; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 0];
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 1];
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 2];
					}
					for (k = 0; k < 2; k = k + 1) {
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 2 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 0];
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 2 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 1];
						bufTmp[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 2 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 2];
					}
				}
			}
			//20.09.14 신규 60핀
			for (i = 0; i < nHeight / 2; i = i + 1) {
				for (j = 0; j < ((nWidth * 3 / 4) * 3 / 18); j = j + 1) {
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 0] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 3];		//B
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 1] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 0];		//G
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 2] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 4];		//R
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 3] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 1];		//B
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 4] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 5];		//G
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 5] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 2];		//R
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 6] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 11];	//B
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 7] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 8];		//G
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 8] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 9];		//R
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 9] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 6];		//B
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 10] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 10];	//G
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 11] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 7];		//R
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 12] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 16];	//B
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 13] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 13];	//G
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 14] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 17];	//R
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 15] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 14];	//B
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 16] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 15];	//G
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 17] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 12];	//R

					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 0] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 4];		//B
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 1] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 1];		//G
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 2] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 5];		//R
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 3] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 2];		//B
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 4] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 3];		//G
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 5] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 0];		//R
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 6] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 9];		//B
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 7] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 6];		//G
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 8] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 10];	//R
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 9] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 7];		//B
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 10] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 11];	//G
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 11] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 8];		//R
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 12] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 17];	//B
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 13] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 14];	//G
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 14] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 15];	//R
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 15] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 12];	//B
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 16] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 16];	//G
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 17] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 13];	//R
				}
			}

			/*
			//20.09.14 신규 60핀
			for (i = 0; i < nHeight / 2; i = i + 1) {
				for (j = 0; j < ((nWidth * 3 / 4) * 3 / 18); j = j + 1) {
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 0] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 1];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 1] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 0];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 2] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 2];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 3] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 3];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 4] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 5];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 5] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 4];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 6] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 6];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 7] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 8];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 8] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 7];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 9] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 11];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 10] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 10];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 11] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 9];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 12] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 14];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 13] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 13];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 14] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 12];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 15] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 16];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 16] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 15];
					bufOut[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 17] = bufTmp[(nHeight - i * 2 - 1) * nWidth * 3 + j * 18 + 17];

					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 0] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 2]; //B
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 1] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 1]; //G
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 2] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 0]; //R
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 3] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 4];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 4] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 3];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 5] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 5];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 6] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 7];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 7] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 6];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 8] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 8];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 9] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 9];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 10] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 11];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 11] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 10];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 12] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 12];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 13] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 14];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 14] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 13];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 15] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 17];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 16] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 16];
					bufOut[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 17] = bufTmp[(nHeight - i * 2 - 2) * nWidth * 3 + j * 18 + 15];
				}
			}
			*/

			/*
			//// Y17&18&19 Red Blue Swap Mode 추가
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 8; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 2]; //CH0_Red
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 1]; //CH0_GREEN
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 0]; //CH0_BLUE
					}
					for (k = 0; k < 2; k = k + 1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 2];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 0];
					}
				}
				for (j = nWidth / 8; j < nWidth / 4; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 2];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 0];
					}
					for (k = 0; k < 2; k = k + 1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 2 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 2];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 2 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 2 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + j * 3 * 4 + (k + 2) * 3 + 0];
					}
				}
			}
			*/
			/*
			//Y18_Model_4
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 16; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						if (j < 180) {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 0];	//CH0_BLUE
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 1];	//CH0_GREEN
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + j * 3 * 4 + k * 3 + 2]; //CH0_RED
						}
						else {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + (j + 240 - 180) * 3 * 4 + k * 3 + 0];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + (j + 240 - 180) * 3 * 4 + k * 3 + 1];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + (j + 240 - 180) * 3 * 4 + k * 3 + 2];
						}
					}
				}

				for (j = 0; j < nWidth / 16; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						if (j < 120) {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * (j + 240) + k * 3 + 0] = bufIn[i*nWidth * 3 + (j + 240 + 60) * 3 * 4 + k * 3 + 0];	//CH1_BLUE
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * (j + 240) + k * 3 + 1] = bufIn[i*nWidth * 3 + (j + 240 + 60) * 3 * 4 + k * 3 + 1];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * (j + 240) + k * 3 + 2] = bufIn[i*nWidth * 3 + (j + 240 + 60) * 3 * 4 + k * 3 + 2];
						}
						else {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * (j + 240) + k * 3 + 0] = bufIn[i*nWidth * 3 + (j - 120) * 3 * 4 + (k + 2) * 3 + 0];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * (j + 240) + k * 3 + 1] = bufIn[i*nWidth * 3 + (j - 120) * 3 * 4 + (k + 2) * 3 + 1];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + 6 * (j + 240) + k * 3 + 2] = bufIn[i*nWidth * 3 + (j - 120) * 3 * 4 + (k + 2) * 3 + 2];
						}
					}
				}

				for (j = 0; j < nWidth / 16; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						if (j < 60) {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + (j + 120) * 3 * 4 + (k + 2) * 3 + 0];	//CH2_BLUE
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + (j + 120) * 3 * 4 + (k + 2) * 3 + 1];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + (j + 120) * 3 * 4 + (k + 2) * 3 + 2];
						}
						else {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + (j + 240 - 60) * 3 * 4 + (k + 2) * 3 + 0];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + (j + 240 - 60) * 3 * 4 + (k + 2) * 3 + 1];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + (j + 240 - 60) * 3 * 4 + (k + 2) * 3 + 2];
						}
					}
				}

				for (j = 0; j < nWidth / 16; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						if (j < 180) {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 0] = bufIn[i*nWidth * 3 + (j + 480) * 3 * 4 + k * 3 + 0];	//CH3_BLUE
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 1] = bufIn[i*nWidth * 3 + (j + 480) * 3 * 4 + k * 3 + 1];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 2] = bufIn[i*nWidth * 3 + (j + 480) * 3 * 4 + k * 3 + 2];
						}
						else {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 0] = bufIn[i*nWidth * 3 + (j + 480 + 60) * 3 * 4 + k * 3 + 0];	//CH3_BLUE
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 1] = bufIn[i*nWidth * 3 + (j + 480 + 60) * 3 * 4 + k * 3 + 1];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 2] = bufIn[i*nWidth * 3 + (j + 480 + 60) * 3 * 4 + k * 3 + 2];
						}

					}
				}

				for (j = nWidth / 8; j < nWidth * 3 / 16; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						if (j < 600) {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + (j + 240 + 60) * 3 * 4 + k * 3 + 0];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + (j + 240 + 60) * 3 * 4 + k * 3 + 1];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + (j + 240 + 60) * 3 * 4 + k * 3 + 2];
						}
						else {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + (j - 120) * 3 * 4 + (k + 2) * 3 + 0];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + (j - 120) * 3 * 4 + (k + 2) * 3 + 1];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + (j - 120) * 3 * 4 + (k + 2) * 3 + 2];
						}
					}
				}

				for (j = nWidth / 8; j < nWidth * 3 / 16; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						if (j < 540) {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 0] = bufIn[i*nWidth * 3 + (j + 120) * 3 * 4 + (k + 2) * 3 + 0];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 1] = bufIn[i*nWidth * 3 + (j + 120) * 3 * 4 + (k + 2) * 3 + 1];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 2] = bufIn[i*nWidth * 3 + (j + 120) * 3 * 4 + (k + 2) * 3 + 2];
						}
						else {
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 0] = bufIn[i*nWidth * 3 + (j + 240 - 60) * 3 * 4 + (k + 2) * 3 + 0];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 1] = bufIn[i*nWidth * 3 + (j + 240 - 60) * 3 * 4 + (k + 2) * 3 + 1];
							bufOut[(nHeight - 1 - i)*nWidth * 3 + nWidth * 3 / 4 + 6 * (j + 240) + k * 3 + 2] = bufIn[i*nWidth * 3 + (j + 240 - 60) * 3 * 4 + (k + 2) * 3 + 2];
						}
					}
				}
			}
			*/
		}
		/*
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth; j = j + 1)
				{
					bufOut[(nHeight - 1 - i) * nWidth * 3 + (nWidth - 1 - j) * 3 + 0] = bufIn[i * nWidth * 3 + j * 3 + 0];
					bufOut[(nHeight - 1 - i) * nWidth * 3 + (nWidth - 1 - j) * 3 + 1] = bufIn[i * nWidth * 3 + j * 3 + 1];
					bufOut[(nHeight - 1 - i) * nWidth * 3 + (nWidth - 1 - j) * 3 + 2] = bufIn[i * nWidth * 3 + j * 3 + 2];
				}
			}
		}
		*/

		/*
		for (i = 0; i < nHeight; i = i + 1) {
			memcpy(bufOut + (2159 - i) * 3840 * 3, bufIn + i * 3840 * 3, 3840 * 3);
		}
		}
		*/

		else {		//	4division rotated
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 8; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						bufOut[(i)*nWidth * 3 + 6 * j + k * 3 + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - k * 3 + 0];
						bufOut[(i)*nWidth * 3 + 6 * j + k * 3 + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - k * 3 + 1];
						bufOut[(i)*nWidth * 3 + 6 * j + k * 3 + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - k * 3 + 2];
					}
					for (k = 0; k < 2; k = k + 1) {
						bufOut[(i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - (k + 2) * 3 + 0];
						bufOut[(i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - (k + 2) * 3 + 1];
						bufOut[(i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - (k + 2) * 3 + 2];
					}
				}
				for (j = nWidth / 8; j < nWidth / 4; j = j + 1) {
					for (k = 0; k < 2; k = k + 1) {
						bufOut[(i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - k * 3 + 0];
						bufOut[(i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - k * 3 + 1];
						bufOut[(i)*nWidth * 3 + nWidth * 3 / 4 + 6 * j + k * 3 + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - k * 3 + 2];
					}
					for (k = 0; k < 2; k = k + 1) {
						bufOut[(i)*nWidth * 3 + nWidth * 3 / 2 + 6 * j + k * 3 + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - (k + 2) * 3 + 0];
						bufOut[(i)*nWidth * 3 + nWidth * 3 / 2 + 6 * j + k * 3 + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - (k + 2) * 3 + 1];
						bufOut[(i)*nWidth * 3 + nWidth * 3 / 2 + 6 * j + k * 3 + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 3 * 4 - (k + 2) * 3 + 2];
					}
				}
			}
		}
		break;
	case 3:		//	8 Division
		if (nRotate == 0) {
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 8; j = j + 1) {
					for (k = 0; k < 4; k = k + 1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + k * 3 + 0] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 0];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + k * 3 + 1] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + k * 3 + 2] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 2];
					}
					for (k = 0; k < 4; k = k + 1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 0] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 0];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 1] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 2] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 2];
					}
				}
			}
		}
		else {		//	8 division rotated
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 8; j = j + 1) {
					for (k = 0; k < 4; k = k + 1) {
						bufOut[(i)*nWidth * 3 + 24 * j + k * 3 + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - k * 3 + 0];
						bufOut[(i)*nWidth * 3 + 24 * j + k * 3 + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - k * 3 + 1];
						bufOut[(i)*nWidth * 3 + 24 * j + k * 3 + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - k * 3 + 2];
					}
					for (k = 0; k < 4; k = k + 1) {
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - (nWidth / 2) * 3 - k * 3 + 0];
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - (nWidth / 2) * 3 - k * 3 + 1];
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - (nWidth / 2) * 3 - k * 3 + 2];
					}
				}
			}

		}
		break;
	case 5:
		if (nRotate == 0) {
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 8; j = j + 1) {
					//19년 이전
					for (k = 0; k < 2; k = k + 1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 0) * 3 + 0] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 0];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 0) * 3 + 1] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 0) * 3 + 2] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 2];
					}

					for (k = 0; k < 2; k = k + 1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 0] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 0];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 1] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 2] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 2];
					}

					for (k = 2; k < 4; k = k + 1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 0] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 0];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 1] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 2] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 2];
					}

					for (k = 2; k < 4; k = k + 1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 0] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 0];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 1] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 2] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 2];
					}
					//19년 신규
					/*
					for (k=0; k < 2; k=k+1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 0) * 3 + 0] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 0];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 0) * 3 + 1] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 0) * 3 + 2] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 2];
					}

					for (k=0; k < 2; k=k+1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 0] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 0];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 1] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 2] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 2];
					}

					for (k=2; k < 4; k=k+1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 0] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 0];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 1] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 2] = bufIn[i*nWidth * 3 + j * 12 + (nWidth / 2) * 3 + k * 3 + 2];
					}

					for (k=2; k < 4; k=k+1) {
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 0] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 0];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 1] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 1];
						bufOut[(nHeight - 1 - i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 2] = bufIn[i*nWidth * 3 + j * 12 + k * 3 + 2];
					}*/
				}
			}
		}
		else {		// case5 analog rotated
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 8; j = j + 1) {

					for (k = 0; k < 2; k = k + 1) {
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 0) * 3 + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - k * 3 + 0];
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 0) * 3 + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - k * 3 + 1];
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 0) * 3 + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - k * 3 + 2];
					}

					for (k = 0; k < 2; k = k + 1) {
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - (nWidth / 2) * 3 - k * 3 + 0];
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - (nWidth / 2) * 3 - k * 3 + 1];
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - (nWidth / 2) * 3 - k * 3 + 2];
					}

					for (k = 2; k < 4; k = k + 1) {
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - k * 3 + 0];
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - k * 3 + 1];
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 2) * 3 + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - k * 3 + 2];
					}

					for (k = 2; k < 4; k = k + 1) {
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - (nWidth / 2) * 3 - k * 3 + 0];
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - (nWidth / 2) * 3 - k * 3 + 1];
						bufOut[(i)*nWidth * 3 + 24 * j + (k + 4) * 3 + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - j * 12 - (nWidth / 2) * 3 - k * 3 + 2];
					}

				}
			}
		}
		break;
	case 6:	//	150303 cks 원래 8번팩인데 안씀.
		if (nRotate == 0) {
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 2; j = j + 1) {
					bufOut[(nHeight - 1 - i)*nWidth * 3 / 2 + 3 * j + 0] = bufIn[i*nWidth * 3 + 6 * j + 3 + 719 * 3 * 2];
					bufOut[(nHeight - 1 - i)*nWidth * 3 / 2 + 3 * j + 1] = bufIn[i*nWidth * 3 + 6 * j + 4 + 719 * 3 * 2];
					bufOut[(nHeight - 1 - i)*nWidth * 3 / 2 + 3 * j + 2] = bufIn[i*nWidth * 3 + 6 * j + 5 + 719 * 3 * 2];

				}
			}
		}
		else {
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth / 2; j = j + 1) {
					bufOut[(i)*nWidth * 3 / 2 + 3 * j + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - 6 * j + 3 - 719 * 3 * 2];
					bufOut[(i)*nWidth * 3 / 2 + 3 * j + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - 6 * j + 4 - 719 * 3 * 2];
					bufOut[(i)*nWidth * 3 / 2 + 3 * j + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - 6 * j + 5 - 719 * 3 * 2];

				}
			}
		}
		break;
	case 7:		//	150303 CKS Pack#5 puzzle
		if (nRotate == 0) {
			for (i = 0; i < nHeight; i = i + 1) {
				memcpy(bufOut + (nHeight - 1 - i)*nBmpDestOff, bufIn + i * nBmpSrcOff, nBmpSrcOff);
			}
		}
		else {
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth; j++) {
					bufOut[(i)*nBmpDestOff + j * 3 + 0] = bufIn[(i + 1)*nBmpSrcOff - 3 - j * 3 * 0];
					bufOut[(i)*nBmpDestOff + j * 3 + 1] = bufIn[(i + 1)*nBmpSrcOff - 3 - j * 3 * 1];
					bufOut[(i)*nBmpDestOff + j * 3 + 2] = bufIn[(i + 1)*nBmpSrcOff - 3 - j * 3 * 2];
				}
			}
		}
		break;
	case 9:		//	170821 LKM Pack#5 HD Rev puzzle
		if (nRotate == 0) {
			for (i = 0; i < nHeight; i = i + 1) {
				memcpy(bufOut + (nHeight - 1 - i)*nBmpDestOff, bufIn + i * nBmpSrcOff, nBmpSrcOff);
			}
		}
		else {
			nBmpDestOff = (1366 * 3 + 3 & 0xffffffc);
			for (i = 0; i < nHeight - 132; i = i + 1) {
				for (j = 0; j < nWidth - (234); j++) {
					bufOut[(i)*(nBmpDestOff)+j * 3 + 0] = bufIn[(i + 0)*nBmpSrcOff + 1366 * 3 - 3 - j * 3 + 0];
					bufOut[(i)*(nBmpDestOff)+j * 3 + 1] = bufIn[(i + 0)*nBmpSrcOff + 1366 * 3 - 3 - j * 3 + 1];
					bufOut[(i)*(nBmpDestOff)+j * 3 + 2] = bufIn[(i + 0)*nBmpSrcOff + 1366 * 3 - 3 - j * 3 + 2];

					//bufOut[(i+132)*nBmpDestOff + j * 3 + 0] = bufIn[(i + 0)*nBmpSrcOff + 1366 * 3 - 3 - j * 3 + 0];
					//bufOut[(i+132)*nBmpDestOff + j * 3 + 1] = bufIn[(i + 0)*nBmpSrcOff + 1366 * 3 - 3 - j * 3 + 1];
					//bufOut[(i+132)*nBmpDestOff + j * 3 + 2] = bufIn[(i + 0)*nBmpSrcOff + 1366 * 3 - 3 - j * 3 + 2];
				}
				//memcpy(bufOut + (nHeight - 1 - i)*nBmpDestOff, bufIn + (nHeight - 1 - i)*nBmpSrcOff, nBmpSrcOff);
			}
		}
		break;
	case 10:		//	191008 LKM Pack#5 Rev puzzle
		if (nRotate == 0) {
			for (i = 0; i < nHeight; i = i + 1) {
				memcpy(bufOut + (nHeight - 1 - i)*nBmpDestOff, bufIn + i * nBmpSrcOff, nBmpSrcOff);
			}
		}
		else {
			for (i = 0; i < nHeight; i = i + 1) {
				for (j = 0; j < nWidth; j++) {
					bufOut[(i)*nBmpDestOff + j * 3 + 0] = bufIn[(i + 1)*nBmpSrcOff - 3 - j * 3 * 0];
					bufOut[(i)*nBmpDestOff + j * 3 + 1] = bufIn[(i + 1)*nBmpSrcOff - 3 - j * 3 * 1];
					bufOut[(i)*nBmpDestOff + j * 3 + 2] = bufIn[(i + 1)*nBmpSrcOff - 3 - j * 3 * 2];
				}
			}
		}
		break;
	default:
		if (nRotate == 0) {
			for (i = 0; i < nHeight; i = i + 1) {
				memcpy(bufOut + (nHeight - 1 - i)*nWidth, bufIn + i * nWidth, nWidth);
			}
		}
		else {
			for (i = 0; i < nHeight; i++) {
				for (j = 0; j < nWidth; j++) {
					bufOut[(i)*nWidth * 3 + 3 * j + 0] = bufIn[(i + 1)*nWidth * 3 - 3 - 3 * j + 0];
					bufOut[(i)*nWidth * 3 + 3 * j + 1] = bufIn[(i + 1)*nWidth * 3 - 3 - 3 * j + 1];
					bufOut[(i)*nWidth * 3 + 3 * j + 2] = bufIn[(i + 1)*nWidth * 3 - 3 - 3 * j + 2];

				}
			}
		}

		break;
	}
	free(bufTmp);
	return 0;
}

//
//#if !defined(AFX_PCIEXPWRAPPER_H__114BF46B_9176_44E0_983E_BDC6F7BCD4C6__INCLUDED_)
//#define AFX_PCIEXPWRAPPER_H__114BF46B_9176_44E0_983E_BDC6F7BCD4C6__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#ifdef _DLL_COMPILE
//#define DLL_API	__declspec(dllexport)
//#endif
//
//#include "stdio.h"
//
//#define		FPGA_FW_FILE_SIZE		6692572
//
//typedef struct _ISD_DEV_INFO
//{
//	UINT		nDevDrvVer;
//	UINT		nTotalPCICardNumber;
//	UCHAR		cBusNumber;
//	UCHAR		cFuncNumber;
//	UCHAR		cDevNumber;
//} ISD_DEV_INFO;
//
////typedef struct _I2C_CMD_LIST
////{
////	unsigned char		cSlaveID;
////	unsigned short int	snAddr;
////	unsigned char		cWriteVal;
////} I2C_CMD_LIST;
//// 20140204 16M 수정
//typedef struct _I2C_CMD_LIST
//{
//	unsigned char		cSlaveID ;
//	unsigned short int	snAddr ;
//	unsigned int		cWriteVal ;
//	unsigned int		nI2cMode ;	
//} I2C_CMD_LIST;
//
//
//typedef struct
//{
//	int			wr_date_year ;
//	int			wr_date_Day ;
//	int			wr_code ;
//	int			reser1 ;
//	float		cur_offset_ch0 ;
//	float		cur_offset_ch1 ;
//	float		cur_offset_ch2 ;
//	float		cur_offset_ch3 ;
//	char		phase_tap ;
//	char		reser2[7] ;
//} tagGRABBER_PARAM ;
//
//typedef struct
//{
//	int		firstTry[256] ;
//	int		secondTry[256] ;
//	int		thirdTry[256] ;
//} tagI2C_RESULT ;
//
//typedef enum
//{
//	FW_VESION5 = 0,
//	FW_VESION6,
//	FW_VESION7,
//	FW_VESION8
//} tagFW_VESION ;
//
////#define			PACK_TYPE_UHDFULL		0
////#define			PACK_TYPE_UHDNORMAL		1
////#define			PACK_TYPE_1_60HZ		2
////#define			PACK_TYPE_1_120HZ		3
////#define			PACK_TYPE_10			4
////#define			PACK_TYPE_8				5
////#define			PACK_TYPE_5				6
////#define			PACK_TYPE_UHDEPI		7
////#define			PACK_TYPE_UHDEPI60		8
////#define			PACK_TYPE_CMVIEW		9
//
//#define			PACK_TYPE_UHDFULL		0
//#define			PACK_TYPE_UHDNORMAL		1
//#define			PACK_TYPE_1_60HZ		2
//#define			PACK_TYPE_1_120HZ		3
//#define			PACK_TYPE_10			4
//#define			PACK_TYPE_8				5
//#define			PACK_TYPE_5				6
//#define			PACK_TYPE_UHDEPI		7
//#define			PACK_TYPE_UHDEPI60		8
//#define			PACK_TYPE_CMVIEW		9
//#define			PACK_TYPE_5_HD_Rev		10
//#define			PACK_TYPE_5_Rev_2019	11
//
///*
//0. UHD Full Speed;
//1. UHD Normal Speed;
//2. Pack1 (51P Only);
//3. Pack1 (51P+41P);
//4. Pack10;
//5. Pack8;
//6. Pack#5;
//*/
//
//
//#ifdef _DLL_COMPILE
//class DLL_API CPciExpWrapper  
//#else
//class CPciExpWrapper  
//#endif
//{
//private :	
////	bool			m_bRevInfo ;
////	tagIMG_INFO		m_tImgInfo ;
////	int				m_iframe_cnt ;
////	int				m_iline_cnt ;
////	int				m_iWidth ;
////	int				m_iRaw10Width ;
//// 	int				m_iHeight ;
//// 	__int64			m_iGrabLength ;
//// 	int				m_iOutFormat ;
//// 	tagGRABBER_PARAM	m_tGrabberParam ;
//// 	tagFW_VESION	m_tFwVersion ;
////	bool			m_bIFCardEndian ;
//
//public:
//	int					m_nPara;
//	bool				m_bErrorLog ;
//	CRITICAL_SECTION	m_csI2C ;
//	// PCIe
//	int GetDeviceInfo				(int nMaxDevNum, ISD_DEV_INFO	*devInfo);
//	int DeviceOpen					(int nDeviceNum);	// to open device
//	int DeviceOpenWithDevInfo		(int nReqBusNum, int nReqFuncNum, int nReqDevNum);
//	int GetDrvInfo					(int *npPciNum, int *npDrvVer, int *npBusNo, int *npFuncNo, int *npDevNo);
//	int DeviceClose					();
//	int DeviceReset					();	// device reset & I/F Card reset	
//
//	int	SRIO_CfgRd					(int nInDexNum, int nDstID, int nHop, int nOff, int *nRdData, int nLocal);
//	int	SRIO_CfgWr					(int nInDexNum, int nDstID, int nHop, int nOff, int *nRdData, int nLocal);
//
//	int CM_RegisterWrite			(int nOffset, __int64 n64Value) ;
//	int CM_RegisterRead				(int nOffset, __int64 *n64Value) ;
//	int	CM_CheckReadDone			(int k, int nThreadID, int *nReadValue) ;
//
//	int CM_ChkGrabDone				(int *nCurrentIndex);
//	int	CM_ImageCopySafe			(int nIndexNum, int nLength, int **nBuffer);
//
// 	int CM_MemInit					(__int64 nGrabLength, int nTimeOutMs);
// 	int CM_ImageCopy				(int nIndexNum, int *nBuffer, int nLength);	
// 	int CM_GrabEnable				(bool bOn) ;
//
//	void Aurora_ReInit				();
//
////	int	DFT3_UHDGrabStart			(int nModel, int nShiftVal, int nWidth, int nHeight, int nOffset, int nDelay);	// 150422 cks수정
//	int	DFT3_UHDGrabStart(int nModel, int nShiftVal, int nWidth, int nHeight, int nOffset, int nDelay, int nLvdsType);
//	int DFT3_AnalogGrabStart		(int nModel, int nWidth, int nHeight);
//	int DFT3_HDMIGrabStart			(int nModel, int nWidth, int nHeight, int nShift);
//	int	DFT3_UHDGrabStop			();
//
//	int	DFT3_EPIPuzzle				(unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight, int nRotate);
//	int	DFT3_CMVPuzzle				(unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight, int nRotate);
//	int	DFT3_UHDPuzzle				(int nMode, unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight, int nRotate);
//	int	DFT3_HDMIPuzzle				(unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight, int nRotate);
//	int DFT3_AnalogPuzzle			(int nMode, unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight, int *nNewWidth, int *nNewHeight, int nRotate);
//	int DFT3_WhiteMerge				(int nMode, unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight) ;
//
//
//	BOOL FLASH_Sector_Erase			(int nFlashNum, int nAddress ); 
//	BOOL FLASH_SubSector_Erase		(int nFlashNum, int nAddress );
//	BOOL FLASH_8x32_Write			(int nFlashNum, int nAddress, unsigned char *data, unsigned char byteLength);
//	BOOL FLASH_Mod_Reset			(int nFlashNum);
//	BOOL FLASH_Verification			(int nFlashNum, wchar_t* str);
//	void FLASH_ResFlush				(int nFlashNum);
//	bool FlashGetResFifoDcnt		(int nFlashNum, int *pDataCnt);
//	bool FlashGetResFifoData		(int nFlashNum, __int64 *pData)	;
//	BOOL FLASH_SubSector_Read		(int nFlashNum, int nAddress, unsigned char* ReadPtr)			;
//	BOOL FLASH_SubSector_Write		(int nFlashNum, int nAddress, unsigned char* SubSectorInitPtr)	;
//
//	int			m_nAllocPCIXpCore	;
//	BOOL		m_bIFCardEndian		;
//	CPciExpWrapper();
//	virtual ~CPciExpWrapper();
//};
//
//#endif // !defined(AFX_PCIEXPWRAPPER_H__114BF46B_9176_44E0_983E_BDC6F7BCD4C6__INCLUDED_)
//
