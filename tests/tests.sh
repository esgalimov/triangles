obj="./triangles.o"
test_folder="./tests/"
test_files_names=("01.dat" "02.dat" "03.dat" "04.dat" "05.dat" "06.dat" "07.dat" "08.dat" "09.dat")

for t in ${test_files_names[@]}; do
    echo $t
    ${obj} < ${test_folder}${t} > ${t}.ans
    echo
done

