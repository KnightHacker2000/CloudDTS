#!/bin/bash

# Diff the output files and the expected files
output_dir=/home/luxi/git/dts_contest/demo-test/sink_file_dir/sink_file_dir
expect_dir=/home/luxi/git/dts_contest/demo-test/expect_file_dir

output_prefix=tianchi_dts_sink_data
expect_prefix=tianchi_dts_expect_data

tables=(customer district item new_orders order_line orders stock warehouse)

for tab in ${tables[@]}; do
        echo "table: $tab"
        diff ${output_dir}/${output_prefix}_$tab ${expect_dir}/${expect_prefix}_$tab
        printf ""
done
