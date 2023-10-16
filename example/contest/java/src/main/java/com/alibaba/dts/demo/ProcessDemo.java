package com.alibaba.dts.demo;

import com.alibaba.dts.demo.common.DataRecord;
import com.alibaba.dts.demo.common.SchemaInfo;
import com.alibaba.dts.demo.common.TableInfo;
import org.apache.commons.lang3.StringUtils;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * @author dts，just for demo.
 */
public class ProcessDemo {

    private static final String DATABASE_NAME = "tianchi_dts_data";                                                             // database name.
    private static final String SCHEMA_FILE_DIR = "schema_info_dir";                                                            // schema information folder.
    private static final String SCHEMA_FILE_NAME = "schema.info";                                                               // schema information file name.
    private static final String SOURCE_FILE_DIR = "source_file_dir";                                                            // input data folder.
    private static final String SINK_FILE_DIR = "sink_file_dir";                                                                // output data folder.
    private static final String SOURCE_FILE_NAME_TEMPLATE = "tianchi_dts_source_data_";                                         // input file name.
    private static final String SINK_FILE_NAME_TEMPLATE = "tianchi_dts_sink_data_%s";                                           // output file name.
    private static final String CHECK_TABLE_SETS = "customer,district,item,new_orders,order_line,orders,stock,warehouse";       // table set to be processed.

    private String sourceDirectory;
    private String sinkDirectory;

    private SchemaInfo schemaInfo;

    public static void main(String[] args) throws IOException {
        System.out.println("################################################");
        System.out.println("start to process file.");
        System.out.println("################################################");


        ProcessDemo demo = new ProcessDemo();

        final Set<String> tableSet = new HashSet<String>(Arrays.asList(CHECK_TABLE_SETS.split(",")));

        for (int i = 0; i < args.length; ++i) {
            if (args[i].toLowerCase().startsWith("--input_dir")
                    || args[i].toLowerCase().startsWith("input_dir")) {
                demo.sourceDirectory = StringUtils.substringAfterLast(args[i], "=");
            }

            if (args[i].toLowerCase().startsWith("--output_dir")
                    || args[i].toLowerCase().startsWith("output_dir")) {
                demo.sinkDirectory = StringUtils.substringAfterLast(args[i], "=");
            }
        }

        String schemaDir = demo.getSchemaSourceSinkFileDirectory("schema");
        String sourceDir = demo.getSchemaSourceSinkFileDirectory("source");
        String sinkDir = demo.getSchemaSourceSinkFileDirectory("sink");


        // load schema information.
        System.out.println("################################################");
        System.out.println("start to load schema info file.");
        System.out.println("################################################");
        demo.initialSchemaInfo(tableSet.size());
        System.out.println("################################################");
        System.out.println("complete load schema info file");
        System.out.println("################################################");


        // initial record map, unique and sort.

        final Map<String, List<DataRecord>> recordMap = new HashMap<String, List<DataRecord>>(3) {{
            for (String table : tableSet) {
                put(table, new ArrayList<DataRecord>(3));
            }
        }};


        // load input file.
        System.out.println("################################################");
        System.out.println("start to load, clean, unique, sort source data.");
        System.out.println("################################################");
        File file = new File(sourceDir);
        String[] fileNames = file.list(new FilenameFilter() {
            @Override
            public boolean accept(File dir, String name) {
                return name.startsWith(SOURCE_FILE_NAME_TEMPLATE);
            }
        });

        for (String fileName : fileNames) {
            File sourceFile = new File(new File(sourceDir), fileName);
            FileInputStream fis = new FileInputStream(sourceFile);
            InputStreamReader isr = new InputStreamReader(fis, "UTF-8");
            BufferedReader reader = new BufferedReader(isr);

            String sourceContent = null;

            while ((sourceContent = reader.readLine()) != null) {
                String[] recordValues = sourceContent.split("\t");
                DataRecord record = demo.serializeRecord(recordValues);

                if (record == null) {
                    continue;
                }

                if (DATABASE_NAME.equals(record.getDatabase()) && tableSet.contains(record.getTable())) {

                    TableInfo tableInfo = demo.schemaInfo.getTableInfoMap().get(record.getTable());

                    DataRecord recordResult = demo.cleanDataRecord(tableInfo, record);

                    if (null == recordResult) {
                        continue;
                    }

                    recordMap.get(record.getTable()).add(record);
                }
            }
        }
        System.out.println("################################################");
        System.out.println("complete load, clean, sort and unique source data");
        System.out.println("################################################");


        // sink to target file
        System.out.println("################################################");
        System.out.println("start to clean and split source data.");
        System.out.println("################################################");
        for (String table : tableSet) {
            List<DataRecord> list = recordMap.get(table);

            File dir = new File(sinkDir);
            if (!dir.exists()) {
                dir.mkdir();
            }

            File tableFile = new File(dir, String.format(SINK_FILE_NAME_TEMPLATE, table));

            System.out.println(tableFile.getAbsoluteFile());


            if (!tableFile.exists()) {
                tableFile.createNewFile();
            }


            BufferedWriter writter = null;
            writter = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(tableFile), "UTF-8"));
            boolean firstLine = true;

            for (DataRecord record : list) {
                if (firstLine) {
                    firstLine = false;
                } else {
                    writter.newLine();
                }

                if (null == record) {
                    continue;
                }

                writter.write(record.toString());
            }

            writter.flush();
        }
        System.out.println("################################################");
        System.out.println("complete sink to data file");
        System.out.println("################################################");


        System.out.println("################################################");
        System.out.println("all tasks complete");
        System.out.println("################################################");
    }

    private String getSchemaSourceSinkFileDirectory(String point) throws IOException {
        if (null == sourceDirectory || sourceDirectory.trim().isEmpty()) {
            sourceDirectory = System.getProperty("user.dir");
        }

        File fileSource = new File(this.sourceDirectory);
        if (!fileSource.exists()) {
            throw new IOException("not find dictionary");
        }

        if (null == sinkDirectory || sinkDirectory.trim().isEmpty()) {
            sinkDirectory = System.getProperty("user.dir");
        }

        File fileSink = new File(this.sinkDirectory);
        if (!fileSink.exists()) {
            throw new IOException("not find dictionary");
        }

        if ("source".equalsIgnoreCase(point)) {
            return sourceDirectory + File.separatorChar + SOURCE_FILE_DIR;
        } else if ("sink".equalsIgnoreCase(point)) {
            return sinkDirectory + File.separatorChar + SINK_FILE_DIR;
        } else if ("schema".equalsIgnoreCase(point)) {
            return sourceDirectory + File.separator + SCHEMA_FILE_DIR;
        } else {
            throw new IOException("error point");
        }
    }

    private void initialSchemaInfo(int tableSize) {

        // load schema information from schema.info file.

        this.schemaInfo = null;
    }

    private DataRecord serializeRecord(String[] json) {
        DataRecord record = null;

        return record;
    }

    private DataRecord cleanDataRecord(TableInfo tableInfo, DataRecord record) {
        // data clean.
        // 1.duplicate primary key data; 2.exceed char length data; 3.error date time type data; 4.error decimal type data; 5.error data type.

        return record;
    }
}
