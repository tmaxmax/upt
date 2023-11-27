#!/usr/bin/env zsh

num_attempts=${1:-50}

function print_results () {
    touch "$2"
    len_paths=$3
    atts=${4:-$num_attempts}
    for (( j = 1; j <= $atts; j++ )); do
        for (( i = 0; i < $len_paths; i++ )); do
            (( index = j + i * $atts ))
            echo -n "${(P)${1}[$index]}" >> "$2"
            let "comma_idx = $len_paths - 1"
            if [ $i -ne $comma_idx ]; then
                echo -n "," >> "$2"
            fi
        done
        echo >> "$2"
    done
}

sample_sizes=(`find ex-6-tests -name "*.in" | sed -e "s/^ex-6-tests\\/s//" -e "s/-n.*$//" | sort -u`)

rm ex-6-results/*.out.csv

for sample_size in "${sample_sizes[@]}"; do
    paths=(`find ex-6-tests -name "s${sample_size}-*.in" | sort`)

    declare -a results_vec=()
    declare -a results_ht=()

    for file in $paths; do
        echo "Testing $file..."
        echo -n "- array"

        ./ex-6.out < $file > /dev/null
        for (( i = 1; i <= $num_attempts; i++ )); do
            typeset -F SECONDS=0
            ./ex-6.out < $file > /dev/null

            results_vec+=("${SECONDS%0000}")
        done

        echo " [done]"
        echo -n "- hash table"

        ./ex-6-ht.out < $file > /dev/null
        for (( i = 1; i <= $num_attempts; i++ )); do
            typeset -F SECONDS=0
            ./ex-6-ht.out < $file > /dev/null

            results_ht+=("${SECONDS%0000}")
        done

        echo " [done]"
    done

    echo "Writing files..."

    print_results "results_vec" "ex-6-results/s${sample_size}.out.csv" ${#paths[@]}
    print_results "results_ht" "ex-6-results/s${sample_size}-ht.out.csv" ${#paths[@]}

    echo
done
