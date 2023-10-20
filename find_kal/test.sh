obj="./build/triangles.o"

little_test_folder="./find_kal/"
little_test_files_names=("t0.txt" "t1.txt" "t2.txt" "t3.txt" "t4.txt" "t5.txt" "t6.txt" "t7.txt" "t8.txt" "t9.txt" "t10.txt" "t11.txt" "t12.txt" "t13.txt" "t14.txt" "t15.txt" "t16.txt" "t17.txt" "t18.txt" "t19.txt" "t20.txt" "t21.txt" "t22.txt" "t23.txt" "t24.txt" "t25.txt" "t26.txt" "t27.txt" "t28.txt" "t29.txt" "t30.txt" "t31.txt" "t32.txt" "t33.txt" "t34.txt" "t35.txt" "t36.txt" "t37.txt" "t38.txt" "t39.txt")


echo "find kal tests:"
echo
for i in ${!little_test_files_names[@]}; do
    echo ${little_test_files_names[$i]}
    ${obj} < ${little_test_folder}${little_test_files_names[$i]}
    echo
done
