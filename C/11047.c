// BAEKJOON
// 15(그리디 알고르즘) - 1
// 11047번 동전 0

#include <stdio.h>

int main()
{
	int N = 0, K = 0;
	int arr[15] = { 0, }; // 동전들의 가치들이 들어갈 배열
	int sum = 0;
	int count = 0;

	scanf("%d %d", &N, &K);

	for (int i = 0; i < N; i++)	// arr[0][1][2]---[9]
	{
		scanf("%d ", &arr[i]);
	}

	for (int j = N - 1; j >= 0; j--)		// arr[9][8][7]---[0]
	{
		if (arr[j] < K && sum + arr[j] <= K) //이부분을 반복해야함.
		{
			sum += arr[j];
			count++;
			j++;	//해당되면 반복시키기 위해
		}
		if (sum == K)
			break;

	}

	printf("%d \n", count);
	return 0;
}
