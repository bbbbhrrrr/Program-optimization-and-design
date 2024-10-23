echo "---------------ORIGINAL-----------------"
original_output=$(gcc -O3 -g -mavx2 bhr.c -o bhr && ./bhr)
rm original.log
echo "$original_output" >>original.log
original_time=$(echo "$original_output" | grep -oE '[0-9]+\.[0-9]+' | head -1)

echo "---------------BHRCODE--------------------"
newcode_output=$(gcc -O3 -g -mavx2 ./cwl_bhr.c -o bhrrr && ./bhrrr)
rm bhr.log
echo "$newcode_output" >>bhr.log
newcode_time=$(echo "$newcode_output" | grep -oE '[0-9]+\.[0-9]+' | head -1)

echo "---------------CWLCODE--------------------"
newcode_output=$(gcc -O3 -g -mavx2 cwl.c -o cwl && ./cwl)
rm cwl.log
echo "$newcode_output" >>cwl.log
newcode_cwltime=$(echo "$newcode_output" | grep -oE '[0-9]+\.[0-9]+' | head -1)

echo "---------------CWL_THREAD_CODE--------------------"
newcode_output=$(gcc -O3 -g -mavx2 cwl_thread.c -o cwl && ./cwl)
rm cwl.log
echo "$newcode_output" >>cwl.log
newcode_cwlthtime=$(echo "$newcode_output" | grep -oE '[0-9]+\.[0-9]+' | head -1)

echo "---------------ZAYCODE--------------------"
newcode_output=$(gcc -O3 -g -mavx2 zay.c -o zay && ./zay)
rm zay.log
echo "$newcode_output" >>zay.log
newcode_zaytime=$(echo "$newcode_output" | grep -oE '[0-9]+\.[0-9]+' | head -1)

echo "Comparison:"
echo -e "\033[1;32m[+] \033[0mOriginal code: $original_time milliseconds"
echo -e "\033[1;32m[+] \033[0mbhr's code: $newcode_time milliseconds"
echo -e "\033[1;32m[+] \033[0mcwl's code: $newcode_cwltime milliseconds"
echo -e "\033[1;32m[+] \033[0mcwlth's code: $newcode_cwlthtime milliseconds"
echo -e "\033[1;32m[+] \033[0mzay's code: $newcode_zaytime milliseconds"
# 计算加速比(original_time和newcode_cwltime的比值)
echo "[!] cwlth faster than original_time $(echo "scale=2; $original_time / $newcode_cwlthtime" | bc)"
echo "[!] cwlth faster than cwl_time $(echo "scale=2; $newcode_cwltime / $newcode_cwlthtime" | bc)"
