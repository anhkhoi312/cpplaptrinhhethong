#include <stdio.h>

void PrintBits(unsigned int x)
{
	int i;
	for (i = 8 * sizeof(x) - 1; i >= 0; i--)
	{
		(x & (1 << i)) ? putchar('1') : putchar('0');
	}
	printf("\n");
}
void PrintBitsOfByte(unsigned int x)
{
	int i;
	for (i = 7; i >= 0; i--)
	{
		(x & (1 << i)) ? putchar('1') : putchar('0');
	}
	printf("\n");
}

// 1.1
int bitAnd(int x, int y)
{
	//PrintBits(x);
	//PrintBits(y);
	return ~(~x | ~y);
}

// 1.2
int negative(int x)
{
// Muốn tìm số âm của số nguyên x: ta dùng bù 2 ( bù 1 + 1)
	return ~x + 1;
}

// 1.3
int getByte(int x, int n)
{
    /* chuyển byte cần lấy về cuối để dễ lấy ( AND 0xFF )
	 lấy byte thứ n thì dịch byte sang phải n lần
	 vd:lấy byte thứ 1 cần dịch phải 1 lần tức qua 8 bit -> (1<<3)=1*2^3=8
		lấy byte thứ 2 cần dịch phải 2 lần tức qua 16 bit ->(2<<3)=2*2^3=16
					   ->cần dịch phải (n<<3) bit
	*/
	return (x >> (n << 3)) & 0xFF;
}

// 1.4
int getnbit(int x, int n)
{
	return (x&(~(0xFFFFFFFF<<n)));
	/*
		dựa trên: x & 1 = x, để lấy n bit từ phải qua trái, Mask phải có số bit 1 từ phải qua trái bằng n, các bit còn lại là 0.
		->dịch trái mask ban đầu sang trái n lần, nhưng khi dịch trái thì bit thêm vào sau là 0
		->Dùng NOT(vd: ~(11100)=00011 với n=2)->mask ban đầu là 0xFFFFFFFF 

	*/
}

// 1.5
int mulpw2(int x, int n)
{
	/*
		return ((x>>(~n+1))&y) | ((x<<n)&z) với lúc n âm thì y=0xFFFFFFFF,z=0x0; ngược lai thì y=0x0,z=0xFFFFFFFF
		với y:
		khi n âm thì bit dấu là 1->NOT bit dấu, mở rộng thành 32 bit bằng dịch trái, sau đó NOT lần nữa sẽ được y mong muốn
		cách làm này ko thỏa với n không âm vì y tính ra là 0x7FFFFFFF-> OR tiếp với 0,y sẽ là 0x0. Với n âm tức y=0xFFFFFFFF thì khi OR với 0 sẽ không ảnh hưởng nên vẫn thỏa
		với z:
		thay vì tìm một biểu thức mới để tính z, ta thấy giá trị của z trong mỗi trường hợp sẽ có liên hệ với y
		-> z=y+0xFFFFFFFF (tuy y=0xFFFFFFFF thì sẽ gây ra tràn bit nhưng thu được z mong muốn)
	*/
	int isNegative = (n >> 31) & 1;
	int y=(~(~isNegative<<31))|0;
    return ((x>>(~n+1))&(y)) | ((x << n)&(y+0xFFFFFFFF));
}


// 2.1
int isSameSign(int x, int y)
{
	int x_sign = (x >> 31) & 1; // Lấy bit dấu của x và y
	int y_sign = (y >> 31) & 1;

	//return ~(x_sign^y_sign) +2,  	
	return !(x_sign ^ y_sign);
	
}

// 2.2
int is8x(int x)
{
	int x_sign = x & 0b111; 
	return !(x_sign); 
}
// 2.3
int isPositive(int x)
{	
	return ((~x + 1) >> 31) & 1; 
}

// 2.4
int isLess2n(int x, int y)
{
	int x_after= (x>>y);
	return !(x_after) ;
}

int main()
{
	int score = 0;
	printf("Your evaluation result:");
	printf("\n1.1 bitAnd");
	if (bitAnd(3, -9) == (3 & -9))
	{
		printf("\tPass.");
		score += 1;
	}
	else
		printf("\tFailed.");

	printf("\n1.2 negative");
	if (negative(0) == 0 && negative(9) == -9 && negative(-5) == 5)
	{
		printf("\tPass.");
		score += 1;
	}
	else
		printf("\tFailed.");

	//1.3
	printf("\n1.3 getByte");
	if (getByte(8, 0) == 8 && getByte(0x11223344, 1) == 0x33)
	{
		printf("\tPass.");
		score += 2;
	}
	else
		printf("\tFailed.");

	printf("\n1.4 getnbit");
	if (getnbit(0, 0) == 0 && getnbit(31, 3) == 7 && getnbit(16, 4) == 0)
	{
		printf("\tPass.");
		score += 3;
	}
	else
		printf("\tFailed.");
	//1.5
	printf("\n1.5 mulpw2");
	if (mulpw2(10, -1) == 5 && mulpw2(15, -2) == 3 && mulpw2(32, -4) == 2)
	{
		if (mulpw2(10, 1) == 20 && mulpw2(50, 2) == 200)
		{
			printf("\tAdvanced Pass.");
			score += 4;
		}
		else
		{
			printf("\tPass.");
			score += 3;
		}
	}
	else
		printf("\tFailed.");

	printf("\n2.1 isSameSign");
	if (isSameSign(4, 2) == 1 && isSameSign(13, -4) == 0 && isSameSign(0, 10) == 1)
	{
		printf("\tPass.");
		score += 2;
	}
	else
		printf("\tFailed.");

	printf("\n2.2 is8x");
	if (is8x(16) == 1 && is8x(3) == 0 && is8x(0) == 1)
	{
		printf("\tPass.");
		score += 2;
	}
	else
		printf("\tFailed.");

	printf("\n2.3 isPositive");
	if (isPositive(10) == 1 && isPositive(-5) == 0 && isPositive(0) == 0)
	{
		printf("\tPass.");
		score += 3;
	}
	else
		printf("\tFailed.");

	printf("\n2.4 isLess2n");
	if (isLess2n(12, 4) == 1 && isLess2n(8, 3) == 0 && isLess2n(15, 2) == 0)
	{
		printf("\tPass.");
		score += 3;
	}
	else
		printf("\tFailed.");

	printf("\n--- FINAL RESULT ---");
	printf("\nScore: %.1f", (float)score / 2);
	if (score < 5)
		printf("\nTrouble when solving these problems? Contact your instructor to get some hints :)");
	else
	{
		if (score < 8)
			printf("\nNice work. But try harder.");
		else
		{
			if (score >= 10)
				printf("\nExcellent. We found a master in bit-wise operations :D");
			else
				printf("\nYou're almost there. Think more carefully in failed problems.");
		}
	}


}