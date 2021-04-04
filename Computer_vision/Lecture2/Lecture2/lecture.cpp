#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

	//영상 반전
void InverseImage(BYTE *Img, BYTE *Out, int W, int H)
{
	
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = 255 - Img[i];
	}
}

	//영상 밝기 조절
void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] + Val > 255) //클리핑 처리
		{
			Out[i] = 255;
		}
		else if (Img[i] + Val < 0)
		{
			Out[i] = 0;
		}
		else Out[i] = Img[i] + Val;
	}
}

	//영상 대비 조절
void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, double Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] + Val > 255.0) //클리핑 처리
		{
			Out[i] = 255;
		}
		else Out[i] = (BYTE)Img[i] + Val; 
						//BYTE는 1바이트 double은 8바이트므로 강제 형변환
	}
}

	//히스토그램 만들기
void ObtainHistogram(BYTE* Img, int* Histo, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
		Histo[Img[i]]++;
	/*for (int i = 0; i < 256; i++)
		printf("%d\n", Histo[i]);*/
}

	//누적 히스토그램 만들기
void ObtainAHistogram(int* Histo, int* AHisto)
{
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < i; j++) // j(작은수)가 i(큰수)로 될 때까지 더함
		{
			AHisto[i] += Histo[j];
		}
	}
}

	//스트레칭 코드
void HistogramStretching(BYTE* Img, BYTE* Out, int* Histo, int W, int H)
{
	int ImgSize = W * H;
	BYTE Low, High;
	for (int i = 0; i < 256; i++)
	{
		if (Histo[i] != 0)
		{
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--)
	{
		if (Histo[i] != 0)
		{
			High = i;
			break;
		}
	}
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = (BYTE)((double)(Img[i] - Low) / (High - Low) * 255.0); // 공식
	}
}

	//평활화 코드
void HistogramEqualization(BYTE* Img, BYTE* Out, int* AHisto, int W, int H)
{
	int ImgSize = W * H;
	int Nt = W * H, Gmax = 255;
	double Ratio = Gmax / (double)Nt;
	BYTE NormSum[256]; //정규화합 히스토그램
	for (int i = 0; i < 256; i++)
	{
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);	//평활화 공식
	}
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = NormSum[Img[i]];	//모든 영상의 화소값을 정규화합으로 바꿔야함.
	}
}

	// 이진화 코드
void Binarization(BYTE *Img, BYTE *Out, int W, int H, BYTE Threshold) //Threshold : 임계치
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}

	// 곤잘레즈 경계값 자동 결정 방법

//int GozalezBinThresh(BYTE* Img, BYTE* Out, int* Histo, int W, int H, int ipsilon)
//{
//	// 입실론 값은 3으로
//	
//	BYTE Low, High;
//
//	//1번
//	for (int i = 0; i < 256; i++)	// 1번의 Low값
//	{
//		if (Histo[i] != 0)
//		{
//			Low = i;
//			break;
//		}
//	}
//	for (int i = 255; i >= 0; i--)	// 1번의 High값
//	{
//		if (Histo[i] != 0)
//		{
//			High = i;
//			break;
//		}
//	}
//	int T = ((int)(Low + High) / 2);
//	
//	int G1, G2 = 0;
//	int Highdi, Lowdi = 0;
//	int newT = 0;
//
//	for (int i = 1; i > 0; i++) //4번
//	{
//		for (int i = Low; i < T; i++) 
//		{
//			G1 += i * Histo[i];
//			Highdi += Histo[i];
//		}
//		G1 = G1 / Lowdi; //2번
//
//		for (int i = T; i < High; i++) 
//		{
//			G2 += i * Histo[i];
//			Highdi += Histo[i];
//		}
//
//		G2 = G2 / Highdi; //2번
//
//		newT = (G1 + G2) / 2; //3번
//		
//		if (ipsilon > abs(newT - T))	// 5번
//			return newT;
//		
//		else
//		{
//			T = newT;
//			G1 = 0;
//			G2 = 0;
//			Highdi = 0;
//			Lowdi = 0;
//		}
//	}
//
//	return T;
//}

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo,
	RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName)
{
	FILE* fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), W*H, fp);
	fclose(fp);
}

//---------------------------------main------------------------------------
void main()
{
	BITMAPFILEHEADER hf; // BMP 파일헤더 14Bytes
	BITMAPINFOHEADER hInfo; // BMP 인포헤더 40Bytes
	RGBQUAD hRGB[256]; // 팔레트 (256 * 4Bytes)
	FILE* fp;
	fp = fopen("coin.bmp", "rb");
	if (fp == NULL) return;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE* Image = (BYTE*)malloc(ImgSize);
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	int Histo[256] = { 0 }; // 0으로 모두 초기화
	int AHisto[256] = { 0 }; // 누적히스토그램 저장할 배열

	/*ObtainHistogram(Image, Histo, hInfo.biWidth, hInfo.biHeight);
	ObtainAHistogram(Histo, AHisto);*/ 
	//자동주석 : Ctrl+K+C 해제 : Ctrl+K+U

	//Gonzalez 임계치 자동 결정 방법에 의해 이진화
	//int Thres = GozalezBinThresh(Image, Output, Histo, hInfo.biWidth, hInfo.biHeight, 3); //입실론 값=3
	//Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, Thres);

	//HistogramStretching(Image, Output, Histo, hInfo.biWidth, hInfo.biHeight);
	//HistogramEqualization(Image, Output, AHisto, hInfo.biWidth, hInfo.biHeight);

	//InverseImage(Image, Output, hInfo.biWidth, hInfo.biHeight);
	//BrightnessAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 70);
	//ContrastAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 1.5);

	//1번
	SaveBMPFile(hf, hInfo, hRGB, Image, hInfo.biWidth, hInfo.biHeight, "output4.bmp");
	
	//2번
	for (int i = 0; i < ImgSize; i++) Output[i] = Image[i] + 50;
	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "output5.bmp");
	//3번
	for (int i = 0; i < ImgSize; i++) Output[i] = 255 - Image[i];
	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "output6.bmp");


	free(Image);
	free(Output);
}