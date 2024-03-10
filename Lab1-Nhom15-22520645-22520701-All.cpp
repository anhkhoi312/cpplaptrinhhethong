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
		khi âm thì bít dấu là 1->NOT bit dấu, mở rộng bằng dịch trái
		tương tự cách làm này cũng không thỏa khi n không âm, z tính ra là 0x80000000->ta thấy nếu cộng với 0x7FFFFFFF thì sẽ được giá trị z mong muốn nhưng chỉ được cộng khi n không âm nên ta AND 0x7FFFFFFFF với NOT của bit dấu.
		
		Có thể có cách code clean hơn nhưng tạm thời chưa nghĩ ra.
	*/
	int isNegative = (n >> 31) & 1;
    return ((x>>(~n+1))&((~(~isNegative<<31))|0)) | (((x << n)&((~isNegative<<31)+(0x7FFFFFFF&~isNegative))));
}




// 2.1
int isSameSign(int x, int y)
{
	int x_sign = (x >> 31) & 1; // Lấy bit dấu của x
	int y_sign = (y >> 31) & 1; // Lấy bit dấu của y

	// Sử dụng phép XOR để so sánh dấu,1 xor 0 =1 tức khác dấu sẽ trả về 1, ngược lại là 0, điều này trái với đề nên cần dùng NOT
	return ~(x_sign ^ y_sign) +2; 
	// +2 vì: not(0)=-1, -1+2=1->cùng dấu
	//		  not(1)=-2, -2+2=0->trái dấu
}

// 2.2
int is8x(int x)
{
	int x_sign = x & 0b111; // thực hiện phép AND giữa x và số 7(111) mục đích là để lấy ra 3 số cuối 
	// 1 số nguyên x có chia hết cho 8 hay không thì 3 bit cuối của số đó phải là 0 
	return !(x_sign); // nếu x_sign = 0 (tức là 3 số cuối bằng 0 -> x chia hết cho 8) vậy thì !x_sign sẽ trả về giá trị true(=1)
	// ngược lại sẽ trả về giá trị false(=0) nếu x_sign khác 0
	
}

// 2.3
int isPositive(int x)
{
	/*
		x dương thì in 1, ngược lại thì in 0
		ý tưởng: dùng bit dấu
		nếu x<0->bit dấu là 1, x>0->bit dấu là 0 => Lấy NOT bit dấu, nhưng ko thỏa với x=0
		-> để ý kết quả sẽ trái với bit dấu nên có thể OR bit dấu của x với một số trái dấu,lấy bit dấu của kết quả. Vì cần 1 biểu thức có thể thỏa hết trường hợp nên số trái dấu cần tìm sẽ là số đối của x. Xét x=0, số đối của 0 là 0 nên kết quả trả về sẽ thỏa đề
		
	*/
	int x_sign = (x >> 31) & 1; // Lấy bit dấu của x
	int is_positive = ((x_sign | (~x + 1)) >> 31) & 1;
	return is_positive;
}

// 2.4
int isLess2n(int x, int y)
{
	
	 int powerOfTwo = ~(1 << y) + 1; // 1 dịch phải y bit -> tạo ra 1 số mũ 2 bằng cách dịch trái số 1 đi y bit (2^y)
	 // tính bù 2 của 1 << y 
	 // thực hiện phép AND giữa x và bù 2 của 1 << y để kiểm tra xem kết quả có phải bằng 0 hay không
	 // nếu kết quả của phép AND bằng 0, điều đó chỉ rằng không có bit 1 nào mà 2 số x và powerOfTwo ở cùng 1 bị trí
    // kết quả sẽ trả về 0, dùng NOT(!) để phủ định kết quả, vậy kết quả trả về True khi x nhỏ hơn 2^y và ngược lại
	return (!(x & powerOfTwo)) ;
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