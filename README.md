# Luogu Paint

A c++ program for Luogu winter paintboard.

Just for fun~

今年像素画准备不充分。养精蓄锐，明年再战！

# Usage

## main

### 用途

主程序。

### 使用方法

直接编译运行即可。

配置相关参见 `config` 栏。

`g++ main.cpp -o main --std=c++11 && ./main`。

## config

### 用途

配置文件。

### 使用方法

创建一个文件 `config`，或者 `mv sample-config config`。

然后将获得到的 cookies 按照格式存入 `cookies[]` 数据。记得更新 `COOKIE_LEN` 常量。

注意：`graph` 图像行列是反的。

如果你想画的是：

```
01110
11011
```

那么数组里应该填：

```cpp
{"01", "11", "10", "11", "01"};
```

## 32colorize

### 用途

将一张 RGB 图片转成 32 色的。

### 使用方法

直接编译运行即可。

`g++ 32colorize.cpp -o 32colorize`。

然后输入 `./32colorize <input> <output> -A` 就可以转换成 `config` 文件里使用的格式了！

图片转 `ppm` 格式的方法：

1. 先 `sudo apt install gimp`。
2. 然后 `gimp <文件名>` 打开你要转格式的图片。
3. 接下来打开 `文件 -> Export`，选择 `.ppm` 格式，确定后在弹出的对话框中选择 **ASCII**。

### 参数说明

```
Usage: 32colorize [input] [output] [options]
Description:
  Reads an ASCII-format PPM image as input and 32-colorize it. Then writes an PPM image as output.
  WARNING: ONLY SUPPORT GIMP EXPORT FORMAT!
Options:
  -H --help       Display this help.
  -P --ppm        Export using ASCII PPM format (Default).
  -A --array      Export using 32-based char array format for config.\n"
```