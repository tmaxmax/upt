while read line; do
    echo "Test: $line"
    ./a.out <<< "$line"
done < tests.in
