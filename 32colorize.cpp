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
	"Usage: 32colorize [input] [output] [options]\n"
	"Description:\n"
	"  Reads an ASCII-format PPM image as input and 32-colorize it. Then writes an PPM image as output.\n"
	"  WARNING: ONLY SUPPORT GIMP EXPORT FORMAT!\n"
	"Options:\n"
	"  -H --help       Display this help.\n"
	"  -P --ppm        Export using ASCII PPM format (Default).\n"
	"  -A --array      Export using 32-based char array format for config.\n"
};

// buffer 大小. 酌情修改.
const int SIZE = 255;

// 画布长宽, 酌情更改.
const int N = 810, M = 410;

// 两个颜色的切比雪夫距离
int dist(const RGB& x, const RGB& y) {
	return std::max({abs(x.R - y.R), abs(x.G - y.G), abs(x.B - y.B)});
}

// 读文件的 buffer.
char buf[SIZE];
RGB tmp;

// 文件指针
FILE *in;
FILE *out;

// 数字转 32 进制字符
char int2char(int x) {
	if (x < 10) return x + '0';
	return x - 10 + 'a'; 
}

// PPM 格式导出
void exportPPM_() {
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
}

// 数组格式导出
void exportArray_() {
	int n, m, x = 0, y = 0; // 长宽和坐标
	static char map[N][M] = {};
	// 打个标记先
	fputs("// 32-COLORIZED BY TIGER0132\n", out);
	// 文件头处理一下
	//   丢掉前两行
	fgets(buf, SIZE, in);
	fgets(buf, SIZE, in);
	//   读一下长宽，最大像素值扔了
	fscanf(in, "%d%d%*d", &n, &m);
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
		// 丢到 buffer 里去
		map[y][x] = int2char(cand);
		y += ++x / n;
		x %= n;
	}
	// 把结果塞到 output 里去
	fputs("const char graph[X2 - X1 + 1][Y2 - Y1 + 2] = {\n", out);
	for (int i = 0; i < m; i++) {
		fputs("\t\"", out);
		fputs(map[i], out);
		fputs(i == m-1 ? "\"\n" : "\",\n", out);
	}
	fputs("};\n", out);
}

void displayHelp_(int retval) {
	puts(USAGE);
	exit(retval);
}

bool displayHelp; // --help
// bool exportPPM; // -P
bool exportArray; // -M
bool error; // 不可识别的 option

// 输入输出文件名
char inPath[SIZE], outPath[SIZE];

// 只有 GIMP 导出的 ppm 可以用这个程序!
int main(int argc, char **argv) {
	if (argc < 3) error = true;
	// 解析命令行参数
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-H")) displayHelp = true;
		else if (!strcmp(argv[i], "--ppm") || !strcmp(argv[i], "-P")) ;//exportPPM = true;
		else if (!strcmp(argv[i], "--array") || !strcmp(argv[i], "-A")) exportArray = true;
		else if (*(argv[i]) == '-') error = true;
		else if (!*inPath) strcpy(inPath, argv[i]);
		else strcpy(outPath, argv[i]);
	}
	// 出错或 --help 或输入 / 出文件缺失
	if (error || displayHelp || !*inPath || !*outPath) displayHelp_(error);
	// 开文件
	in = fopen(inPath, "r");
	out = fopen(outPath, "w");
	if (!exportArray) exportPPM_(); // -P
	if (exportArray) exportArray_(); // -M
	fclose(in);
	fclose(out);
}