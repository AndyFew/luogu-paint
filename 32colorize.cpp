#include <algorithm>
#include <cstring>
#include <cstdio>

struct RGB { int R, G, B; };

// 画板上的 32 色
RGB palette[32] = {
	{0, 0, 0},
	{255, 255, 255},
	{170, 170, 170},
	{85, 85, 85},
	{254, 211, 199},
	{255, 196, 206},
	{250, 172, 142},
	{255, 139, 131},
	{244, 67, 54},
	{233, 30, 99},
	{226, 102, 158},
	{156, 39, 176},
	{103, 58, 183},
	{63, 81, 181},
	{0, 70, 112},
	{5, 113, 151},
	{33, 150, 243},
	{0, 188, 212},
	{59, 229, 219},
	{151, 253, 220},
	{22, 115, 0},
	{55, 169, 60},
	{137, 230, 66},
	{215, 255, 7},
	{255, 246, 209},
	{248, 203, 140},
	{255, 235, 59},
	{255, 193, 7},
	{255, 152, 0},
	{255, 87, 34},
	{184, 63, 39},
	{121, 85, 72}
};

// 使用方法.
const char USAGE[] = {
	"Usage: 32colorize [input] [output]\n"
	"Description:\n"
	"  Reads an ASCII-format PPM image as input and 32-colorize it. Then writes an PPM image as output.\n"
	"  WARNING: ONLY SUPPORT GIMP EXPORT FORMAT!"
};

// buffer 大小. 酌情修改.
const int SIZE = 255;

// 两个颜色的切比雪夫距离
int dist(const RGB& x, const RGB& y) {
	return std::max({abs(x.R - y.R), abs(x.G - y.G), abs(x.B - y.B)});
}

// 逐行读文件的 buffer.
char buf[SIZE];
RGB tmp;

// 只有 GIMP 导出的 ppm 可以用这个程序!
int main(int argc, char **argv) {
	if (!strcmp(argv[1], "--help")) {
		puts(USAGE);
		return 0;
	}
	if (argc != 3) {
		puts(USAGE);
		return 1;
	}
	FILE *in = fopen(argv[1], "r");
	FILE *out = fopen(argv[2], "w");
	// 打个标记先
	fputs("# 32-COLORIZED BY TIGER0132\n", out);
	// 前四行复读
	for (int i = 0; i < 4; i++) {
		fgets(buf, SIZE, in);
		fputs(buf, out);
	}
	while (!feof(in)) {
		fscanf(in, "%d%d%d", &tmp.R, &tmp.G, &tmp.B);
		// 颜色距离使用切比雪夫距离 (感觉用别的容易被毒瘤数据叉掉)
		int cand = 0, dis = 0x3f3f3f3f;
		for (int i = 0; i < 32; i++) {
			if (dist(tmp, palette[i]) < dis) {
				dis = dist(tmp, palette[i]);
				cand = i;
			}
		}
		fprintf(out, "%d\n%d\n%d\n", palette[cand].R, palette[cand].G, palette[cand].B);
	}
	fclose(in);
	fclose(out);
}