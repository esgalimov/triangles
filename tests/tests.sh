obj="./build/triangles.o"

little_test_folder="./tests/little_tests/"
little_test_files_names=("01.dat" "02.dat" "03.dat" "04.dat" "05.dat" "06.dat" "07.dat" "08.dat" "09.dat" "10.dat")
little_answers=("0 1" "0 1 2 3" "0 1 2" "0 1 2" "0 1" "0 1 2" "0 4 8" " " "0 1 2" "0 1 2 3")

big_test_folder="./tests/big_tests/"
big_test_files_names=("03.dat" "04.dat" "05.dat" "06.dat" "07.dat" "08.dat" "09.dat" "10.dat")


echo "little tests:"
echo
for i in ${!little_test_files_names[@]}; do
    echo ${little_test_files_names[$i]}
    ${obj} < ${little_test_folder}${little_test_files_names[$i]}
    echo correct = ${little_answers[$i]}
    echo
done

echo "big tests:"
echo
for i in ${!big_test_files_names[@]}; do
    echo ${big_test_files_names[$i]}
    ${obj} < ${big_test_folder}${big_test_files_names[$i]}
    echo
done
