C编程练习-大数乘法及其高效算法





大数相乘（Multiplication algorithm）是指两数相乘的结果超出了基本数据类型类型的表示范围，不能简单的直接相乘。

目前大数乘法算法主要有以下几种思路：

1. **模拟手算**：乘法的竖式手算；
2. **分治乘法**：最简单的是Karatsuba乘法，一般化以后有Toom-Cook乘法；
3. **快速傅里叶变换FFT**：（为了避免精度问题，可以改用快速数论变换FNTT），时间复杂度O(N lgN lglgN)。可参照[Schönhage–Strassen algorithm](https://en.wikipedia.org/wiki/Sch%C3%B6nhage%E2%80%93Strassen_algorithm)；
4. **中国剩余定理**：把每个数分解到一些互素的模上，然后将每个同余方程对应相乘；
5. **Furer’s algorithm**：在渐进意义上FNTT还快的算法。可参考维基百科[Fürer’s algorithm](https://en.wikipedia.org/wiki/F%C3%BCrer%27s_algorithm)

GNU的GMP库有对大数乘法非常好的实现，[GMP文档](https://gmplib.org/manual/Multiplication-Algorithms.html#Multiplication-Algorithms)对用到的算法有较详细的描述，有兴趣可以深入研究。

本文只探讨模拟乘法竖式手算的算法和Karatsuba算法

### 模拟乘法竖式手算

```
  1 2 3 4 1 2 3 4
× 4 3 2 1 4 3 2 1       
-----------------
  1 2 3 4 1 2 3 4   <---- 第1趟 
2 4 6 8 2 4 6 8     <---- 第2趟 
   ..........       <---- 第n趟 
-----------------
  ? ? ? ? ? ? ? ?   <---- 累加进位
```

模拟手算，每次计算一趟，同时处理进位。

```c
char* longMultiplication(char *multiplicand, char *multiplier,char* result) {
	//结果清零,数字0
	int i = 0;
	for (; i < strlen(multiplicand) + strlen(multiplier) + 3;i++) {
		result[i] = '0';
	}
	result[i] = '\0';
	//处理符号位
	char sign = '+';
	if (multiplicand[0] == '+') {
		multiplicand++;
	}
	if (multiplier[0] == '+') {
		multiplier++;
	}
	if (multiplicand[0] == '-' && multiplier[0] != '-') {
		multiplicand++;
		sign = '-';
	}
	else if (multiplicand[0] != '-' && multiplier[0] == '-') {
		multiplier++;
		sign = '-';
	}
	else if (multiplicand[0] == '-' && multiplier[0] == '-') {
		multiplicand++;
		multiplier++;
	}
	//乘数从最后一位开始
	char *p2 = multiplier + strlen(multiplier)-1;//乘数最后一位
	char *pret = result;//结果先逆序存储，最后反转
	//算法部分
	while (p2 >= multiplier) {
		int carry = 0;
		char * pone = pret;
		for (char* p1 = multiplicand + strlen(multiplicand) - 1;
             p1 >= multiplicand; p1--) {
			int digit = *pone -'0'+(*p1-'0') * (*p2-'0') + carry;
			*pone = digit % 10 + '0';
			carry = digit / 10;
			pone++;
		}
		//每乘一趟后处理进位
		for (char* pca = pone; carry > 0;carry/=10, pca++) {
			*pca = *pca + carry % 10;
		}
		p2--;
		pret++;
	}
    //处理结果
	i = 0;
	for (i = strlen(multiplicand) + strlen(multiplier) + 2; 
         (result[i] == '0') && (i>0); i--);
	if (sign == '-' && i!=0) {
		result[i+1] = '-';
		result[i + 2] = '\0';
	}
	else {
		result[i + 1] = '\0';
	}
	_strrev(result);
	return result;
}
```

### Karatsuba算法

Karatsuba于1960年发明在 O(nlog23) 步骤内将两个n位数相乘的Karatsuba算法。它反证了安德雷·柯尔莫哥洛夫于1956年认为这个乘法需要 Ω(n2) 步骤的猜想。

算法原理如下:

> 假设要相乘的两个数X,Y,假设都为N位。X,Y可以分别表示为:
>
> ​    X = a * 10^(n/2) + b;  
>
> ​    Y = c * 10^(n/2) + d;
>
> 则X*Y可以换算为：
>
>    X*Y = (a * 10^(n/2) + b) * ( c * 10^(n/2) + d)
>
> ​            =ac*10^n + (ad+bc) * 10^(n/2) + bd
>
> 其中 (ad + bc) 又可转化为((a+b)*(c+d) - ac - bd)，
>
> 这样就能利用ac,bd的结果将两次乘法转为一次乘法。
>
> 故X*Y可进一步转化为ac*10^n + ((a+b)*(c+d) - ac - bd)* 10^(n/2) + bd

C语言算法如下：

```c

```



