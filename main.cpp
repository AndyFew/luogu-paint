#include <cstdlib>
#include <cstdio>

/****************************** Definitions ******************************/

// User Agent. 可自行更换.
#define UA "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36"

// 存储命令的 buffer 大小. 无需更改.
const int SIZE = 255;

// loop 之间的 delay(ms). 根据网速酌情更改.
const int DELAY = 31000; // 31s

// 参数分别为:
// __client_id, uid, x, y, color.
const char curl_format[] =
	"curl -k 'https://www.luogu.org/paintBoard/paint' "
	"-H 'cookie: __client_id=%s; _uid=%d' "
	"-H 'user-agent: " UA "' "
	"-H 'referer: https://www.luogu.org/paintBoard' "
	"--data 'x=%d&y=%d&color=%d' --compressed"
;

struct user { // 用户结构体
	char __client_id[41];
	int uid;
};

extern const int COOKIE_LEN;
extern const user cookies[COOKIE_LEN];

#include <config> // 配置文件, 存储 cookies 相关. 格式参考 <sample-config>.

/****************************** Functions ******************************/

void paint(int x, int y, int color, const user& id) {
	static char buf[SIZE];
	sprintf(buf, curl_format, id.__client_id, id.uid, x, y, color);
	system(buf);
}

/****************************** Main ******************************/

int main() {
	while ("CZ AK IOI") {
		
	}
}