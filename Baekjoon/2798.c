// BAEKJOON
// 11(브루트 포스) - 1
// 2798번 블랙잭

#include <stdio.h>

int main()
{
	int N = 0, M = 0;		//N은 카드의 개수, M은 넘지 말아야 할 최대의 수
	int arr[101] = { 0, };  //배열 자리
	int sum = 0;			//가상의 자리 a+b+c 의 합
	int output = 0;			//가장 M가 가까운 sum

	scanf("%d %d", &N, &M);

	for (int i = 0; i < N; i++) // 배열 입력
	{
		scanf("%d ", &arr[i]);
	}

	for (int a = 0; a < N; a++) // a의 자리
	{
		for (int b = a + 1; b < N; b++) // b의 자리
		{
			for (int c = b + 1; c < N; c++) //  c의 자리
			{
				sum = arr[a] + arr[b] + arr[c];

				if (sum <= M && sum > output) // output <= sum <= M
				{
					output = sum;
				}
			}
		}
	}
	printf("%d", output);
	return 0;
}
