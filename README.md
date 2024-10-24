# Program-optimization-and-design
这是一个关于程序优化与设计的项目，包含了ntt多线程加速、SIMD指令集加速以及cache行为仿真。

## 主要内容
	- NTT 多线程优化：通过线程拆分实现 NTT 加速。
	-	线程池优化：使用线程池减少线程创建的开销。
	-	GPU 加速：将 NTT 函数移植到 GPU 上运行。

## 实验环境
	-	CPU: Intel i7-12700H 6P8E P Core @4.70 GHz E Core@3.50 GHz （在 Linux 内核下，CPU 0-11 为 P 核心，12-19 为 E 核心）
	-	操作系统：Linux Kernel 6.9.0
	-	编译器：GCC 13.2.1

## 运行步骤

### 克隆仓库
```bash
git clone https://github.com/bbbbhrrrr/Program-optimization-and-design.git
cd Commerce-Security-Governance-Over-privacy-alliance-CSGO-/
```

### 编译并运行
```bash
g++ -o ntt ntt.cpp -O3
./ntt
```
