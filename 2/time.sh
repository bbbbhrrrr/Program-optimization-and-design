#!/bin/bash
# 执行一次gcc 参数为$1
# 执行多次./a.out
arg=$1
gcc -g -O3 -mavx2 "$arg"

total_time=0

# 执行多次并累计时间
for ((i = 0; i < $2; i++)); do
	output=$(./a.out)
	execution_time=$(echo "$output" | grep -oE '[0-9]+\.[0-9]+' | head -1)
	total_time=$(awk "BEGIN {print $total_time + $execution_time}")
	echo "Execution time for run $((i + 1)): $execution_time nanoseconds"
done

# 计算平均时间
average_time=$(awk "BEGIN {print $total_time / $2}")

# 输出平均时间
echo
echo -e "\033[1;32m[+]\033[0m Average time: \033[1;34m$average_time\033[0m nanoseconds"
