#include <stdio.h>
#include <string.h>
#include <stdint.h>

unsigned int A0 = 0x67452301;
unsigned int B0 = 0xEFCDAB89;
unsigned int C0 = 0x98BADCFE;
unsigned int D0 = 0x10325476;

// int g[64] = {
//   0, 1, 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
//   1, 6, 11, 0,  5,  10, 15, 4,  9,  14, 3,  8,  13, 2,  7,  12,
//   5, 8, 11, 14, 1,  4,  7,  10, 13, 0,  3,  6,  9,  12, 15, 2,
//   0, 7, 14, 5,  12, 3,  10, 1,  8,  15, 6,  13, 4,  11, 2,  9
// };

#define F(b, c, d) (((b) & (c)) | ((~b) & (d)))
#define G(b, c, d) (((b) & (d)) | ((c) & (~d)))
#define H(b, c, d) ((b) ^ (c) ^ (d))
#define I(b, c, d) ((c) ^ ((b) | (~d))) 

unsigned int* padding(char* src, int length, int* len) {
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
  int* slide = (int*)malloc(sizeof(int) * (K + P + 64) / 32);
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

unsigned int leftortate(unsigned int x, unsigned int c) {
  return (x << c) | (x >> (32 - c));
}

char* compress(unsigned int* chunks, int num) {
  unsigned int s[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
  };
  unsigned int K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
  };
  unsigned int a0 = A0;
  unsigned int b0 = B0;
  unsigned int c0 = C0;
  unsigned int d0 = D0;

  for (int i = 0; i < num; ++i) {
    unsigned int M[16];
    memcpy(M, chunks + i * 16, 16 * sizeof(unsigned int));
    unsigned int A = a0;
    unsigned int B = b0;
    unsigned int C = c0;
    unsigned int D = d0;
    for (int j = 0; j <= 63; ++j) {
      unsigned int f, g;
      if (j >= 0 && j <= 15) {
        f = F(B, C, D);
        g = j;
      } else if (j >= 16 && j <= 31) {
        f = G(B, C, D);
        g = (5 * j + 1) % 16;
      } else if (j >= 32 && j <= 47) {
        f = H(B, C, D);
        g = (3 * j + 5) % 16;
      } else if (j >= 48 && j <= 63) {
        f = I(B, C, D);
        g = (7 * j) % 16;
      }
      f = f + A + K[j] + M[g];
      A = D;
      D = C;
      C = B;
      B += leftortate(f, s[j]);
    }
    a0 = a0 + A;
    b0 = b0 + B;
    c0 = c0 + C;
    d0 = d0 + D;
  }
  char* digest = (char*)malloc(sizeof(char) * 16);
  memcpy(digest, &a0, sizeof(int));
  memcpy(digest + sizeof(int) * 1, &b0, sizeof(int));
  memcpy(digest + sizeof(int) * 2, &c0, sizeof(int));
  memcpy(digest + sizeof(int) * 3, &d0, sizeof(int));
  return digest;
}

char* toHex(unsigned int a) {
  const char* hexContent = "0123456789abcdef";
  char* ans = (char*)malloc(sizeof(char) * 8);
  int idx = 0;
  for (int i = 0; i < 4; ++i) {
    int part = ((a >> i * 8) % (1 << 8)) & 0xff;
    ans[idx] = hexContent[(part / 16) % 16];
    idx ++;
    ans[idx] = hexContent[part % 16];
    idx ++;
  }
  return ans;
}

int main(int argc, char const *argv[]) {
  char src[1000] = {0};
  int idx = 0;
  while (1) {
    char temp = 0;
    temp = getchar();
    if (temp == '\n') {
      break;
    }
    src[idx ++] = temp;
  }
  src[idx] = '\0';
  int* num = (int*)malloc(sizeof(int));
  unsigned int* pad = padding(src, idx, num);
  char* digest = compress(pad, *num);
  unsigned int udigest[4] = {0};
  for (int i = 0; i < 4; ++i) {
    memcpy(udigest + i, digest + i * 4, sizeof(unsigned int));
  }
  char result[32] = {0};
  for (int i = 0; i < 4; ++i) {
    char* temp = toHex(udigest[i]);
    memcpy(result + i * 8, temp, sizeof(char) * 8);
    free(temp);
  }
  for (int i = 0; i < 32; ++i) {
    printf("%c", result[i]);
  }
  printf("\n");
  free(digest);
  free(num);
  free(pad);
  return 0;
}
