#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <queue>

/****************************** Definitions ******************************/

#ifdef DEBUG
#define debug(x) std::cout << x
#else
#define debug(x)
#endif

#define $ regions[id]

// User Agent. 可自行更换.
#define UA "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36"

// 存储命令的 buffer 大小. 无需更改.
const int SIZE = 511;

// 画板的长和宽.
const int N = 800+10, M = 400+10;

// loop 之间的 delay(s). 根据网速酌情更改.
const int DELAY = 31; // 31s

// paint 操作之间的 delay(s). 酌情修改.
const int DELAY2 = 1; // 1s

// 颜色数量, 不要更改.
const int COLORS = 32;

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
	int id, x, y;
};

// 任务.
struct task {
	int x, y, color;
};

// 配置.
struct config {
	int X1, Y1, X2, Y2;
	char graph[N][M];
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
inline void paint(const task& t, const user& id) {
	static char buf[SIZE];
	sprintf(buf, curl_paint_format, id.__client_id, id.uid, t.x, t.y, t.color);
	system(buf);
	debug("uid = " << id.uid << " painted (" << t.x << ", " << t.y << ") to " << t.color << ".\n");
	// system("cat tmp >> log");
	// system("echo >> log");
}

// 获得地图并更新差异列表
inline void getMap(char map[N][M], std::queue<point>& tasks) {
	system(curl_getboard_format);
	rewind(tempFile);
	for (int i = 0; i < N; i++) {
		fgets(map[i], M, tempFile);
	}
	for (int id = 0; id < REGION_CNT; id++) {
		for (int i = 0; i <= $.X2 - $.X1; i++) {
			for (int j = 0; j <= $.Y2 - $.Y1; j++) {
				if (map[$.X1 + i][$.Y1 + j]
				 != $.graph[i][j]) {
					tasks.push((point){id, i, j});
				}
			}
		}
	}
}

// 获得新任务. 如果没有任务, 则随机填充.
inline task getTask(std::queue<point>& tasks) {
	int x, y;
	if (tasks.empty()) {
		while (1) {
			x = rand() % N;
			y = rand() % M;
			for (int id = 0; id < REGION_CNT; id++) {
				if ($.X1 <= x && x <= $.X2 &&
					$.Y1 <= y && y <= $.Y2) continue;
			}
		}
		return (task){x, y, rand() % COLORS};
	}
	x = tasks.front().x;
	y = tasks.front().y;
	int id = tasks.front().id;
	tasks.pop();
	return (task){$.X1 + x, $.Y1 + y, char2int($.graph[x][y])};
}

// 初始化.
inline void init() {
	srand(time(0));
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
		for (int i = 0; i < COOKIE_LEN; i++) {
			paint(getTask(tasks), cookies[i]);
			sleep(DELAY2);
		}
		sleep(DELAY);
		while (!tasks.empty()) tasks.pop();
	}
}