# Conway's Game of Life - 康威生命游戏
### 说明
写了一个[康威生命游戏](https://zh.wikipedia.org/wiki/%E5%BA%B7%E5%A8%81%E7%94%9F%E5%91%BD%E6%B8%B8%E6%88%8F
)的计算模拟程序~
### 程序特点
- 使用 C 语言编写
- (接近)无限的地图区域
- 动态分配, 计算, 管理和释放地图区块
### 使用方法
例: 选择一个初始模板, 例如下图所示的滑翔机.

![滑翔机](https://upload.wikimedia.org/wikipedia/commons/f/f2/Game_of_life_animated_glider.gif)

以 0 和 1 的方式表示出初始的模板, 写入程序执行目录下的 input.txt 中.
```
001
101
011
```
执行程序, 输入模拟的轮数. 输入 0 代表一直计算. 计算的结果保存在程序执行目录下的 output.txt 中. (推荐使用等宽字体查看结果.)

