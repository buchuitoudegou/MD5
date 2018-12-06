#include <stdio.h>
#include <string.h>

int A0 = 0x67452301;
int B0 = 0xEFCDAB89;
int C0 = 0x98BADCFE;
int D0 = 0x10325476;

#define F(b, c, d) (b & c) | ((~b) & d)
#define G(b, c, d) (b & d) | (c & (~d))
#define H(b, c, d) b ^ c ^ d
#define I(b, c, d) c ^ (b | (~d)) 

unsigned int* padding(char[] src, int length, int* len) {
  int K = length;
  K *=  8;
  int P = -1;
  for (int i = 1; i <= 512; ++i) {
    if ((i + K) % 512 == 448) {
      P = i;
      break;
    }
  }
  char first = 0x80;
  int complement = P / 8;
  unsigned long long t = K;
  int num = (K + P + 64) / 512;
  char* result = (char*)malloc(sizeof(char) * (K + P + 64) / 8);
  char* temp = (char*)malloc(sizeof(char) * complement);
  int* slide = (int*)malloc(sizeof(unsigned int) * (K + P + 64) / 32);
  for (int i = 0; i < complement; ++i) {
    if (i == 0) {
      temp[i] = 0x80;
    } else {
      temp[i] = 0x00;
    }
  }
  memcpy(result, src, sizeof(char) * length);
  memcpy(result + length, temp, sizeof(char) * complement);
  memcpy(result + length + complement, &t, sizeof(unsigned long long) * 1);
  memcpy(slide, result, sizeof(char) * (K + P + 64) / 8);
  free(result);
  free(temp);
  *len = num;
  return slide;
}

void compress(unsigned int* M) {
  int s[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
  };
  int K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee }
K[ 4.. 7] := { 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 }
K[ 8..11] := { 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be }
K[12..15] := { 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 }
K[16..19] := { 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa }
K[20..23] := { 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 }
K[24..27] := { 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed }
K[28..31] := { 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a }
K[32..35] := { 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c }
K[36..39] := { 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 }
K[40..43] := { 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 }
K[44..47] := { 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 }
K[48..51] := { 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 }
K[52..55] := { 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 }
K[56..59] := { 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 }
K[60..63] := { 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 }
  }
}

int main(int argc, char const *argv[]) {
  char src[1000] = {0};
  scanf("%s", src);

  return 0;
}
