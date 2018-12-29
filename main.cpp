#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <queue>

/****************************** Definitions ******************************/

// User Agent. 可自行更换.
#define UA "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36"

// 存储命令的 buffer 大小. 无需更改.
const int SIZE = 255;

// 画板的长和宽.
const int N = 800+10, M = 400+10;

// loop 之间的 delay(ms). 根据网速酌情更改.
const int DELAY = 31000; // 31s

// cURL 画点的格式串.
// 参数分别为: __client_id, uid, x, y, color.
const char curl_paint_format[] =
	"curl -k 'https://www.luogu.org/paintBoard/paint' "
	"-H 'cookie: __client_id=%s; _uid=%d' "
	"-H 'user-agent: " UA "' "
	"-H 'referer: https://www.luogu.org/paintBoard' "
	"--data 'x=%d&y=%d&color=%d' --compressed "
	"-o tmp 1> /dev/null 2>&1"
;

// cURL 获得画板的命令.
const char curl_getboard_format[] =
	"curl -k 'https://www.luogu.org/paintBoard/board' "
	"-o tmp 1> /dev/null 2>&1"
;

// 用户结构体.
struct user {
	char __client_id[41];
	int uid;
};

// 坐标.
struct point {
	int x, y;
};

// 配置文件, 存储 cookies 相关. 格式参考 <sample-config>.
#include "config"

/****************************** Functions ******************************/

FILE *tempFile;

// 32 进制字符转数字.
inline int char2int(char ch) {
	if (isdigit(ch)) return ch - '0';
	return ch - 'a' + 10;
}

// 让用户 "id" 在画板的 (x, y) 处画上 "color" 色的点.
inline void paint(int x, int y, int color, const user& id) {
	static char buf[SIZE];
	sprintf(buf, curl_paint_format, id.__client_id, id.uid, x, y, color);
	system(buf);
}

// 获得地图并更新差异列表
inline void getMap(char map[N][M], std::queue<point>& tasks) {
	system(curl_getboard_format);
	rewind(tempFile);
	for (int i = 0; i < N; i++) {
		fgets(map[i], M, tempFile);
	}
	for (int i = 0; i <= X2-X1; i++) {
		for (int j = 0; j <= Y2-Y2; j++) {
			if (map[X1 + i][Y1 + j] != graph[i][j]) {
				tasks.push((point){i, j});
			}
		}
	}
}

inline void DBG_putMap(char map[N][M]) {
	FILE *fp = fopen("./out", "w");
	for (int i = 0; i < N; i++) {
		fputs(map[i], fp);
	}
	fclose(fp);
}

// 初始化文件.
inline void init() {
	tempFile = fopen("./tmp", "rw");
}

/****************************** Main ******************************/

std::queue<point> tasks;
char map[N][M];

int main() {
	// 运行时请保证所有 id 的 CD 均为 0.
	init();
	while ("CZ AK IOI") {
		getMap(map, tasks);
	}
}