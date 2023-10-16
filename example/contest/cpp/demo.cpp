

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <chrono>

#include <stdlib.h>
#include <unistd.h>

#include "FileUtil.h"
#include "Schema.h"
#include "Record.h"

#include <glog/logging.h>

using namespace std;

/**
 * @author dts，just for demo.
 */
const string DATABASE_NAME = "tianchi_dts_data";                                                             // 待处理数据库名，无需修改
const string SCHEMA_FILE_DIR = "schema_info_dir";                                                            // schema文件夹，无需修改。
const string SCHEMA_FILE_NAME = "schema.info";                                                               // schema文件名，无需修改。
const string SOURCE_FILE_DIR = "source_file_dir/source_file_dir_orig";                                                            // 输入文件夹，无需修改。
const string SINK_FILE_DIR = "sink_file_dir";                                                                // 输出文件夹，无需修改。
const string SOURCE_FILE_NAME_TEMPLATE = "tianchi_dts_source_data_";                                         // 输入文件名，无需修改。
const string SINK_FILE_NAME_TEMPLATE = "tianchi_dts_sink_data_";                                             // 输出文件名模板，无需修改。
const string CHECK_TABLE_SETS = "customer,district,item,new_orders,order_line,orders,stock,warehouse";       // 待处理表集合，无需修改。

const string SLASH_SEPARATOR = "/";

class Demo {
public:
    string input_dir;
    string output_dir;
    Schema schema;
    // first: tb_name, second: Record
    unordered_map <string, set<Record>> records;

    //for debug output
    void debug_map(unordered_map <string, string> input) {
        int count = 1;
        for (auto item : input) {
            cout << "key: " << item.first << ", value: " << item.second;
            if (count++ != input.size()) {
                cout << ", ";
            }
        }
        cout << endl;
    }

    //for debug output
    void debug_schema() {
        unordered_map <string, Table> tables = schema.getTableMap();
        for (auto t : tables) {
            t.second.toString();
        }
    }

    void debug_records(const unordered_map <string, set<Record>> records) {
        cout << "table num: " << records.size() << endl;
        int sum = 0;

        for (auto tb_record : records) {
            int num = tb_record.second.size();
            sum += num;

            cout << tb_record.first << " num: " << num << endl;
            for (Record r : tb_record.second) {
                r.toString();
            }

        }
        cout << "sum: " << sum << endl;
    }

public:
    void initialSchemaInfo(string dir_name, string tables) {
        cout << "Read schema_info_dir/schema.info file and construct table in memory." << endl;
        string filename = this->input_dir + SLASH_SEPARATOR + dir_name + SLASH_SEPARATOR + SCHEMA_FILE_NAME;
        cout << "Load schema.info file: " << filename << endl;
        ifstream infile(filename.c_str());
        string line;
        Table table;
        while (getline(infile, line)) {
//            debug details:
//            cout << line << endl;
//            unordered_map<string, string> ret = FileUtil::_deserialize_schema_with_state(line);
//            debug_map(ret);
            if (FileUtil::deserialize_schema(line, table)) {
                schema.add_table(table);
                table.reset();
            }
        }
        FileUtil::setSchema(schema);
        debug_schema();
        return;
    }

    void loadSourceData(string path) {
        cout << "Read source_file_dir/tianchi_dts_source_data_* file" << endl;
        string filename_template =
                this->input_dir + SLASH_SEPARATOR + path + SLASH_SEPARATOR + SOURCE_FILE_NAME_TEMPLATE;
        for (int i = 0;; i++) {
            string line;
            string file_name = filename_template + to_string(i);
            ifstream infile(file_name.c_str());
            if (!infile.good()) {
                //不是很确定序号是否从0开始
                if (i == 0) {
                    continue;
                } else {
                    break;
                }
            } else {
                cout << "Load source data file: " << file_name << endl;
                int line_count = 1;
                while (getline(infile, line)) {
//                    cout << line_count ++ << endl;
                    FileUtil::load_source_data(records, line);
                }
            }
            //debug one file
//            break;
        }

        if (VLOG_IS_ON(1)) {
            stringstream ss;
            int tb_cnt = records.size();
            ss << "Total table count:" << tb_cnt << endl;

            int record_cnt = 0;
            for (auto it = records.cbegin(); it != records.cend(); ++it) {
                ss << "Table name:" << it->first << "\tTable record count:" << it->second.size() << endl;
                record_cnt += it->second.size();

            }

            ss << "Total records count=" << record_cnt << endl;

            cout << ss.str() << endl;
            ss.str("");
        }
//        debug_records(records);
        return;
    }

    // TODO: decide if we need split work division (threads for I/O and threads for cleaning)
    // Currently empty since raw row data is cleaned when read.
    void cleanData() {
        cout << "Clean and sort the source data." << endl;
        return;
    }

    void deduplicateData() {
    }

    void sinkData(string dir_name) {
        cout << "Sink the data." << endl;
        string sink_dir = output_dir + SLASH_SEPARATOR + dir_name;
        system(("mkdir " + sink_dir).c_str());
        string file_template = output_dir + SLASH_SEPARATOR + dir_name + SLASH_SEPARATOR + SINK_FILE_NAME_TEMPLATE;
        for (auto table_records : records) {
            string file = file_template + table_records.first;
            cout << "Sink file: " << file << endl;
            ofstream ofs;
            ofs.open(file);
            int count = 0;
            for (Record r : table_records.second) {
                string line;
                r.to_file(line);

                ofs << line;
                if (count++ != table_records.second.size() - 1) {
                    ofs << endl;
                }
            }
            ofs.close();
        }
        return;
    }

    void getCurrentDirectory() {
        char *buffer;
        //也可以将buffer作为输出参数
        if ((buffer = getcwd(NULL, 0)) == NULL) {
            perror("getCurrentDirectory error.");
        } else {
            this->input_dir = string(buffer);
            free(buffer);
            if (this->input_dir.find("debug")) {
                // pwd/cmake-build-debug
                this->input_dir = this->input_dir.substr(0, this->input_dir.length() - 17);
            }
            printf("Current working directory: %s\n", this->input_dir.c_str());
        }
    }
};

/**
Input:
1. Disordered source data (in SOURCE_FILE_DIR)
2. Schema information (in SCHEMA_FILE_DIR)

Process:
	data clean:
	1) duplicate primary key data;
	2) exceed char length data;
	3) error date time type data;
	4) error decimal type data;
	5) error data type.

	sort by pk

Output:
1. Sorted data of each table (out SINK_FILE_DIR)

**/
int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);

    Demo *demo = new Demo();
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][2] == 'i') {
            demo->input_dir = argv[i + 1];
        } else if (argv[i][0] == '-' && argv[i][2] == 'o') {
            demo->output_dir = argv[i + 1];
        }
    }
//    demo->getCurrentDirectory();
    cout << "[Start]\tload schema information." << endl;
    // load schema information.
    demo->initialSchemaInfo(SCHEMA_FILE_DIR, CHECK_TABLE_SETS);
    cout << "[End]\tload schema information." << endl;
    cout << endl;

    auto input_start = std::chrono::system_clock::now();
    // load input Start file.
    cout << "[Start]\tload input file." << endl;
    demo->loadSourceData(SOURCE_FILE_DIR);
    cout << "[End]\tload input file." << endl;
    cout << endl;
    auto input_end = std::chrono::system_clock::now();

    // data clean.
    cout << "[Start]\tdata clean." << endl;
    demo->cleanData();
    cout << "[End]\tdata clean." << endl;
    cout << endl;
    auto process_end = std::chrono::system_clock::now();

    // sink to target file
    cout << "[Start]\tsink to target file." << endl;
    demo->sinkData(SINK_FILE_DIR);
    cout << "[End]\tsink to target file." << endl;
    cout << endl;
    auto output_end = std::chrono::system_clock::now();

    if (VLOG_IS_ON(1)) {
        std::stringstream ss;
        ss << endl
        << "Input duration (ms):\t" << std::chrono::duration_cast<std::chrono::microseconds>(input_end - input_start).count() << endl
        << "Process duration(ms):\t" << std::chrono::duration_cast<std::chrono::microseconds>(process_end - input_end).count() << endl
        << "Output duration (ms):\t" << std::chrono::duration_cast<std::chrono::microseconds>(output_end - process_end).count() << endl
        << endl;

        LOG(INFO) << ss.str() << endl;
        ss.str("");
    }

    return 0;
}
