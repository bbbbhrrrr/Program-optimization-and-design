#include<iostream>
#include<algorithm>
#include<cstring>
#include<chrono>
#include<thread>
#include<vector>
using namespace std;
const int N = 300100, P = 998244353;	//模数为p，数组长度限制为N
int A[N], B[N], C[N], r[N];
int qpow(int x, int y)//快速模幂算法
{
	int res(1);
	while (y)
	{
		if (y & 1) res = 1ll * res*x%P;
		x = 1ll * x*x%P;
		y >>= 1;
	}
	return res;
}
 
void ntt(int *x, int lim, int opt)//请使用多线程编程思想，对此函数进行优化
{
	int i, j, k, m, gn, g, tmp;
	for (i = 0; i < lim; ++i)
		if (r[i] < i)
			swap(x[i], x[r[i]]);
	for (m = 2; m <= lim; m <<= 1)
	{
		k = m >> 1;
		gn = qpow(3, (P - 1) / m);
		for (i = 0; i < lim; i += m)
		{
			g = 1;
            //1ll的目的是防止乘法的数据溢出
			for (j = 0; j < k; j++, g = 1ll * g*gn%P)
			{
				tmp = 1ll * x[i + j + k] * g%P;
				x[i + j + k] = (x[i + j] - tmp + P) % P;
				x[i + j] = (x[i + j] + tmp) % P;
			}
		}
	}
//  逆ntt计算流程，与实验无关
// 	if (opt == -1)
// 	{
// 		reverse(x + 1, x + lim);
// 		int inv = qpow(lim, P - 2);
// 		for (i = 0; i < lim; ++i)
// 			x[i] = 1ll * x[i] * inv%P;
// 	}
}

int main() {
    // 初始化随机数生成器
    srand(time(nullptr));
    int i, lim = 1, n = N / 2; // 设 n 是 N 的一半，确保进行乘法后数据位数不会溢出
    // 用随机数填充 A 和 B，用大数组模拟十进制下的大数表示
    for (i = 0; i < n; i++) {
        A[i] = rand() % 10; 
        B[i] = rand() % 10;
    }
    // 计算适当的 lim 值
    while (lim < n) lim <<= 1;
    // 初始化 r 数组
    for (i = 0; i < lim; ++i) {
        r[i] = (i & 1) * (lim >> 1) + (r[i >> 1] >> 1);
    }
    auto start = chrono::high_resolution_clock::now();
    // 对 A 和 B 进行 NTT
    ntt(A, lim, 1);
    ntt(B, lim, 1);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, nano> elapsed = end - start;
    cout << "NTT took " << elapsed.count() << " nanoseconds to execute.\n";//计算两次NTT的总时间，单位为纳秒
    // 计算 C = A * B的过程，与实验无关
    // for (i = 0; i < lim; ++i) {
    //     C[i] = 1ll * A[i] * B[i] % P;
    // }
    // 对 C 进行逆 NTT
    // ntt(C, lim, -1);
    // 进行乘法进位处理
    // for (i = 0; i < lim; ++i) {
    //     C[i + 1] += C[i] / 10;
    //     C[i] %= 10;
    // }
    return 0;
}
