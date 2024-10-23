echo "---------------ORIGINAL-----------------"
original_output=$(gcc -g -O3 -mavx2 bhr.c -o bhr && ./bhr)
rm original.log
echo "$original_output" >>original.log
original_time=$(echo "$original_output" | grep -oE '[0-9]+\.[0-9]+' | head -1)

# echo "---------------BHRCODE--------------------"
# newcode_output=$(gcc -g -mavx2 ./bhrrrr.c -o bhrrr && ./bhrrr)
# rm bhr.log
# echo "$newcode_output" >>bhr.log
# newcode_time=$(echo "$newcode_output" | grep -oE '[0-9]+\.[0-9]+' | head -1)

echo "---------------CWLCODE--------------------"
newcode_output=$(gcc -g -O3 -mavx2 ./cwl_thread.c -o cwl && ./cwl)
rm cwl.log
echo "$newcode_output" >>cwl.log
newcode_cwltime=$(echo "$newcode_output" | grep -oE '[0-9]+\.[0-9]+' | head -1)

# echo "---------------ZAYCODE--------------------"
# newcode_output=$(gcc -g -mavx2 zay.c -o zay && ./zay)
# rm zay.log
# echo "$newcode_output" >>zay.log
# newcode_zaytime=$(echo "$newcode_output" | grep -oE '[0-9]+\.[0-9]+' | head -1)

echo "Comparison:"
echo -e "\033[1;32m[+] \033[0mOriginal code: $original_time milliseconds"
# echo -e "\033[1;32m[+] \033[0mbhr's code: $newcode_time milliseconds"
echo -e "\033[1;32m[+] \033[0mcwl's code: $newcode_cwltime milliseconds"
# echo -e "\033[1;32m[+] \033[0mzay's code: $newcode_zaytime milliseconds"
# 计算加速比(original_time和newcode_cwltime的比值)
echo "[!] faster $(echo "scale=2; $original_time / $newcode_cwltime" | bc)"
