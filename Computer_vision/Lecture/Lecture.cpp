#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

//영상 반전
void InverseImage(BYTE* Img, BYTE* Out, int W, int H)
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
void Binarization(BYTE* Img, BYTE* Out, int W, int H, BYTE Threshold) //Threshold : 임계치
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
//			Lowdi += Histo[i];
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
BYTE DetermThGonzalez(int* H)
{
	BYTE ep = 3; //입실론
	BYTE Low, High; // 최소, 최대값
	BYTE Th, NewTh;
	int G1 = 0, G2 = 0, cnt1 = 0, cnt2 = 0, mu1, mu2;
	for (int i = 0; i < 256; i++)
	{
		if (H[i] != 0)
		{
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--)	
	{
		if (H[i] != 0)
		{
			High = i;
			break;
		}
	}

	Th = (Low + High) / 2; // 1단계

	printf("%d\n", Th);

	while (1) // 4단계 - If문 될때까지 무한반복
	{
		for (int i = Th + 1; i <= High; i++)
		{
			G1 += (H[i] * i);
			cnt1 += H[i];
		}
		for (int i = Low; i <= Th; i++)
		{
			G2 += (H[i] * i);
			cnt2 += H[i];
		}
		// 2단계

		if (cnt1 == 0) cnt1 = 1; // 나눗셈 오류 예외처리
		if (cnt2 == 0) cnt2 = 1;

		mu1 = G1 / cnt1; //mu = 밝기의 평균값
		mu2 = G2 / cnt2; //3단계

		NewTh = (mu1 + mu2) / 2; // 4단계

		if (abs(NewTh - Th) < ep)
		{
			Th = NewTh;
			break;
		}
		else
		{
			Th = NewTh;
		}
		G1 = G2 = cnt1 = cnt2 = 0;
		printf("%d\n", Th);
	}

	return Th;
	
}


void AverageConv(BYTE* Img, BYTE* Out, int W, int H) // 박스평활화
{
	double Kernel[3][3] = { 0.11111, 0.11111, 0.11111,
										0.11111, 0.11111, 0.11111,
										0.11111, 0.11111, 0.11111 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void GaussAvrConv(BYTE* Img, BYTE* Out, int W, int H) // 가우시안평활화
{
	double Kernel[3][3] = { 0.0625, 0.125, 0.0625,
										0.125, 0.25, 0.125,
										0.0625, 0.125, 0.0625 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
										-1.0, 0.0, 1.0,
										-1.0, 0.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										0.0, 0.0, 0.0,
										1.0, 1.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
										-2.0, 0.0, 2.0,
										-1.0, 0.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 1020  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

void Sobel_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -2.0, -1.0,
										0.0, 0.0, 0.0,
										1.0, 2.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										-1.0, 8.0, -1.0,
										-1.0, -1.0, -1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 2040  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 8;
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv_DC(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										-1.0, 9.0, -1.0,
										-1.0, -1.0, -1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			//Out[i * W + j] = abs((long)SumProduct) / 8;
			if (SumProduct > 255.0) Out[i * W + j] = 255;
			else if (SumProduct < 0.0) Out[i * W + j] = 0;
			else Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo,
	RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName)
{
	FILE* fp = fopen(FileName, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), W * H, fp);
	fclose(fp);
}

void swap(BYTE* a, BYTE* b)
{
	BYTE temp = *a;
	*a = *b;
	*b = temp;
}

BYTE Median(BYTE* arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j])
				swap(&arr[i], &arr[j]); //call by reference
		}
	}
	return arr[S / 2];
}

BYTE MaxPooling(BYTE* arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[S - 1];
}

BYTE MinPooling(BYTE* arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[0];
}



int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top)
{
	if (*top >= arr_size) return(-1);
	(*top)++;
	stackx[*top] = vx;
	stacky[*top] = vy;
	return(1);
}

int pop(short* stackx, short* stacky, short* vx, short* vy, int* top)
{
	if (*top == 0) return(-1);
	*vx = stackx[*top];
	*vy = stacky[*top];
	(*top)--;
	return(1);
}

// GlassFire 알고리즘을 이용한 라벨링 함수
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	//	BYTE** CutImage2;
	Out_Area = 1;

	// 스택으로 사용할 메모리 할당
	short* stackx = new short[height * width];
	short* stacky = new short[height * width];
	short* coloring = new short[height * width];

	int arr_size = height * width;

	// 라벨링된 픽셀을 저장하기 위해 메모리 할당

	for (k = 0; k < height * width; k++) coloring[k] = 0;  // 메모리 초기화

	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함
			if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
			r = i; c = j; top = 0; area = 1;
			curColor++;

			while (1)
			{
			GRASSFIRE:
				for (m = r - 1; m <= r + 1; m++)
				{
					index = m * width;
					for (n = c - 1; n <= c + 1; n++)
					{
						//관심 픽셀이 영상경계를 벗어나면 처리 안함
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor; // 현재 라벨로 마크
							if (push(stackx, stacky, arr_size, (short)m, (short)n, &top) == -1) continue;
							r = m; c = n; area++;
							goto GRASSFIRE;
						}
					}
				}
				if (pop(stackx, stacky, &r, &c, &top) == -1) break;
			}
			if (curColor < 1000) BlobArea[curColor] = area;
		}
	}

	float grayGap = 255.0f / (float)curColor; //curColor 총 컴포넌트의 개수


	//F9 코드 break
	// 가장 면적이 넓은 영역을 찾아내기 위함 
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i; //BlobArea 덩어리들의 양적 정보, Out_Area 가장 큰 것
	}
	// CutImage 배열 클리어~
	for (k = 0; k < width * height; k++) CutImage[k] = 255;
	// Output이미지를 받아오는것 - 전체 배열 하얗게(255) 한 뒤에.

	// coloring에 저장된 라벨링 결과중 (Out_Area에 저장된) 영역이 가장 큰 것만 CutImage에 저장
	for (k = 0; k < width * height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 0;  // 가장 큰 것만 저장하여 까맣게 칠함(size filtering)
		//if (BlobArea[coloring[k]] > 500) CutImage[k] = 0; // 500덩어리(특정 면적) 이상만 출력
		//CutImage[k] = (unsigned char)(coloring[k] * grayGap); // 모든 라벨링 결과들 그대로 출력
		  //아까 나눈값(489줄) grayGap 각 컴포넌트들의 해당 밝기값을 채워줌
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}
// 라벨링 후 가장 넓은 영역에 대해서만 뽑아내는 코드 포함

void BinaryimageEdgeDetection(BYTE* Bin, BYTE* Out, int W, int H)
{
	
	for (int i = 0; i < H; i++)//경계 검출
	{
		for (int j = 0; j < W; j++)
		{
			if (Bin[i * W + j] == 0) //전경화소라면?
			{
				if (!(Bin[(i - 1) * W + j] == 0 && Bin[(i + 1) * W + j] == 0 &&
					Bin[i * W + j - 1] == 0 && Bin[i * W + j + 1] == 0)) // 4방향 화소 중 하나라도 전경이아니라면
				{
					Out[i * W + j] = 255; //경계는 0으로 출력
				}

			}
		}
	}
}

//---------------------------------main------------------------------------
int main()
{
	BITMAPFILEHEADER hf; // BMP 파일헤더 14Bytes
	BITMAPINFOHEADER hInfo; // BMP 인포헤더 40Bytes
	RGBQUAD hRGB[256]; // 팔레트 (256 * 4Bytes)
	FILE* fp;
	fp = fopen("coin.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE* Image = (BYTE*)malloc(ImgSize);
	BYTE* Temp = (BYTE*)malloc(ImgSize); // 임시배열
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	int H = hInfo.biHeight, W = hInfo.biWidth;
	int Histo[256] = { 0 }; // 0으로 모두 초기화
	int AHisto[256] = { 0 }; // 누적히스토그램 저장할 배열

	//자동주석 : Ctrl+K+C 해제 : Ctrl+K+U
	BYTE Th;
	ObtainHistogram(Image, Histo, W, H); //히스토그램을 먼저 구함.
	Th = DetermThGonzalez(Histo);
	Binarization(Image, Output, W, H, Th);

	//동공 경계선 그리기
	//Binarization(Image, Temp, W, H, 50); //임계치가 높아 잘 구분못할수도 있음.
	//InverseImage(Temp, Temp, W, H); //2진화된 영상Temp
	//m_BlobColoring(Temp, H, W);
	//for (int i = 0; i < ImgSize; i++) Output[i] = Image[i]; //전체 배열에 어떻게? Image[i] / 255하면 흰색 초기화
	//BinaryimageEdgeDetection(Temp, Output, W, H);
	

	//Gonzalez 임계치 자동 결정 방법에 의해 이진화
	//int Thres = GozalezBinThresh(Image, Output, Histo, hInfo.biWidth, hInfo.biHeight, 3); //입실론 값=3
	//Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, Thres);

	/*Sobel_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Sobel_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	for (int i = 0; i < ImgSize; i++) {
		if (Temp[i] > Output[i]) 	Output[i] = Temp[i];
	}*/

	/*GaussAvrConv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Laplace_Conv_DC(Temp, Output, hInfo.biWidth, hInfo.biHeight);*/

	//HistogramStretching(Image, Output, Histo, hInfo.biWidth, hInfo.biHeight);
	//HistogramEqualization(Image, Output, AHisto, hInfo.biWidth, hInfo.biHeight);

	//InverseImage(Image, Output, hInfo.biWidth, hInfo.biHeight);
	//BrightnessAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 70);
	//ContrastAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 1.5);

	/* Median filtering */
	//BYTE temp[9];
	//int W = hInfo.biWidth, H = hInfo.biHeight;
	//int i, j;
	//for (i = 1; i < H - 1; i++) {
	//	for (j = 1; j < W - 1; j++) {
	//		temp[0] = Image[(i - 1) * W + j - 1];
	//		temp[1] = Image[(i - 1) * W + j];
	//		temp[2] = Image[(i - 1) * W + j + 1];
	//		temp[3] = Image[i * W + j - 1];
	//		temp[4] = Image[i * W + j];
	//		temp[5] = Image[i * W + j + 1];
	//		temp[6] = Image[(i + 1) * W + j - 1];
	//		temp[7] = Image[(i + 1) * W + j];
	//		temp[8] = Image[(i + 1) * W + j + 1];
	//		Output[i * W + j] = Median(temp, 9); //impulse 노이즈 없애기
	//		//Output[i * W + j] = MaxPooling(temp, 9); //pepper 노이즈 없애기
	//		//Output[i * W + j] = MinPooling(temp, 9); //salt 노이즈 없애기
	//	}
	//}

	//int Length = 3;  // 마스크의 한 변의 길이
	//int Margin = Length / 2; // 마스크의 중심으로부터의 거리를 측정해 마진값을 계산
	//int WSize = Length * Length; // 윈도우 사이즈
	//BYTE* temp = (BYTE*)malloc(sizeof(BYTE) * WSize); //한 변의 크기로 윈도우를 만듬
	//int W = hInfo.biWidth, H = hInfo.biHeight; 
	//int i, j, m, n;
	//for (i = Margin; i < H - Margin; i++) { // 이중포문을 통한 미디안 필터링
	//	for (j = Margin; j < W - Margin; j++) { 
	//		for (m = -Margin; m <= Margin; m++) { //윈도우 사이즈 만큼 픽셀값
	//			for (n = -Margin; n <= Margin; n++) {
	//				temp[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + j + n]; // 해당 위치에 영상 처리
	//			}
	//		}
	//		Output[i * W + j] = Median(temp, WSize); //median 중간값 반환하여 Window 사이즈만큼 impulse 노이즈 없애기
	//	}
	//}
	/* Median filtering */

	//AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight);

	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "output_gonzalez.bmp");


	free(Image);
	free(Output);
	free(Temp);
	return 0;
}