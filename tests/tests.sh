obj="./build/triangles"

test_folder="./tests/test_files/"
correct_folder="./tests/correct_files/"
answer_folder="./tests/answer_files/"

echo "TESTS:"
echo
for ((i = 1; i <= 26; i++)) do
    echo $i.dat
    ${obj} < ${test_folder}$i.dat > ${answer_folder}${i}ans.dat
    echo diff:
    diff ${correct_folder}${i}ans.dat ${answer_folder}${i}ans.dat
    echo
    echo
done
