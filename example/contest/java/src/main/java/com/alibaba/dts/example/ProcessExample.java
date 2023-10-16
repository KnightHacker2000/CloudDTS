package com.alibaba.dts.example;

import com.alibaba.dts.example.common.DataRecord;
import com.alibaba.dts.example.common.MyColumn;
import com.alibaba.dts.example.common.MyIndex;
import com.alibaba.dts.example.common.Operation;
import com.alibaba.dts.example.common.SchemaInfo;
import com.alibaba.dts.example.common.TableInfo;
import com.alibaba.fastjson.JSONObject;
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
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

/**
 * @author dts, the example for validation results, for myself only。
 */
public class ProcessExample {

    private static final String DATABASE_NAME = "tianchi_dts_data";
    private static final String SCHEMA_FILE_DIR = "schema_info_dir";
    private static final String SCHEMA_FILE_NAME = "schema.info";
    private static final String SOURCE_FILE_DIR = "source_file_dir";
    private static final String SINK_FILE_DIR = "sink_file_dir";
    private static final String SOURCE_FILE_NAME_TEMPLATE = "tianchi_dts_source_data_";
    private static final String SINK_FILE_NAME_TEMPLATE = "tianchi_dts_sink_data_%s";
    private static final String CHECK_TABLE_SETS = "customer,district,item,new_orders,order_line,orders,stock,warehouse";
    private static final String REGEX_STR = ".*[a-zA-Z]+.*";

    private String sourceDirectory;
    private String sinkDirectory;

    private SchemaInfo schemaInfo;

    public static void main(String[] args) throws IOException {
        ProcessExample example = new ProcessExample();
        final Set<String> tableSet = new HashSet<String>(Arrays.asList(CHECK_TABLE_SETS.split(",")));

        assert args.length == 2;

        for (int i = 0; i < args.length; ++i) {
            if (args[i].toLowerCase().startsWith("--input_dir")
                    || args[i].toLowerCase().startsWith("input_dir")) {
                example.sourceDirectory = StringUtils.substringAfterLast(args[i], "=");
            }

            if (args[i].toLowerCase().startsWith("--output_dir")
                    || args[i].toLowerCase().startsWith("output_dir")) {
                example.sinkDirectory = StringUtils.substringAfterLast(args[i], "=");
            }
        }

        String schema_dir = example.getSchemaSourceSinkFileDirectory("schema");
        String source_dir = example.getSchemaSourceSinkFileDirectory("source");
        String sink_dir = example.getSchemaSourceSinkFileDirectory("sink");

        // load schema information.
        example.initialSchemaInfo(schema_dir, tableSet.size());

        // Data cleaning.
        final Map<String, Map<Long, DataRecord>> recordMap = new HashMap<String, Map<Long, DataRecord>>(3) {{
            for (String table : tableSet) {
                put(table, new TreeMap<>());
            }
        }};

        File file = new File(source_dir);
        String[] fileNames = file.list(new FilenameFilter() {
            @Override
            public boolean accept(File dir, String name) {
                return name.startsWith(SOURCE_FILE_NAME_TEMPLATE);
            }
        });


        long beginTime = System.currentTimeMillis();

        for (String fileName : fileNames) {

            File sourceFile = new File(new File(source_dir), fileName);
            FileInputStream fis = new FileInputStream(sourceFile);
            InputStreamReader isr = new InputStreamReader(fis, "UTF-8");
            BufferedReader reader = new BufferedReader(isr);

            String sourceContent = null;

            while ((sourceContent = reader.readLine()) != null) {
                String[] recordValues = sourceContent.split("\t");
                DataRecord record = example.serializeRecord(recordValues);

                if (DATABASE_NAME.equals(record.getDatabase()) && tableSet.contains(record.getTable())) {

                    TableInfo tableInfo = example.schemaInfo.getTableInfoMap().get(record.getTable());

                    DataRecord recordResult = example.cleanDataRecord(tableInfo, record);

                    if (null == recordResult) {
                        continue;
                    }

                    long duplicateValues = example.getDuplicateValues(tableInfo, record);

                    recordMap.get(record.getTable()).put(duplicateValues, record);

                }
            }
        }

        long readerTime = System.currentTimeMillis();
        System.out.println("read file use time : " + (readerTime - beginTime));

        // sink to target file
        for (String table : tableSet) {
            Map<Long, DataRecord> recordList = recordMap.get(table);

            File tableFile = new File(new File(sink_dir), String.format(SINK_FILE_NAME_TEMPLATE, table));
            BufferedWriter writter = null;
            writter = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(tableFile), "UTF-8"));
            boolean firstLine = true;

            for (DataRecord record : recordList.values()) {
                if (firstLine) {
                    firstLine = false;
                } else {
                    writter.newLine();
                }

                if (null == record) {
                    continue;
                }

                writter.write(record.myToString());
                writter.flush();
            }
        }

        long writerTime = System.currentTimeMillis();
        System.out.println("writer file use time : " + (writerTime - readerTime));
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

    private void initialSchemaInfo(String schema_dir, int tableSize) throws IOException {
        // reader from schema.info file.

        this.schemaInfo = new SchemaInfo(tableSize);

        FileInputStream fisSchema = new FileInputStream(schema_dir + File.separator + SCHEMA_FILE_NAME);
        InputStreamReader isrSchema = new InputStreamReader(fisSchema, "UTF-8");
        BufferedReader readerSchema = new BufferedReader(isrSchema);
        String lineOuter = null;

        while ((lineOuter = readerSchema.readLine()) != null) {

            if (lineOuter.startsWith("[DATABASE]")) {

                String[] lineOuterInfo = lineOuter.split(" ");
                schemaInfo.setDatabaseName(lineOuterInfo[1]);
                TableInfo tableInfo = new TableInfo(lineOuterInfo[1], lineOuterInfo[3]);

                String lineInner = readerSchema.readLine();
                if (null != lineInner && lineInner.startsWith("COLUMN NUMBER")) {
                    String[] lineInnerInfo = lineInner.split(" ");
                    int columnNumber = Integer.valueOf(lineInnerInfo[lineInnerInfo.length - 1].trim());

                    List<MyColumn> columns = new ArrayList<MyColumn>();
                    for (int i = 0; i < columnNumber; ++i) {
                        String lineColumn = readerSchema.readLine();
                        MyColumn column = new MyColumn(JSONObject.parseObject(lineColumn));
                        columns.add(column);
                    }

                    tableInfo.setColumns(columns);
                }

                lineInner = readerSchema.readLine();
                if (null != lineInner && lineInner.startsWith("INDEX NUMBER")) {
                    String[] lineInnerInfo = lineInner.split(" ");
                    int indexNumber = Integer.valueOf(lineInnerInfo[lineInnerInfo.length - 1].trim());

                    List<MyIndex> indexes = new ArrayList<MyIndex>();
                    for (int i = 0; i < indexNumber; ++i) {
                        String lineIndex = readerSchema.readLine();
                        MyIndex column = new MyIndex(JSONObject.parseObject(lineIndex));
                        indexes.add(column);
                    }

                    tableInfo.setIndexes(indexes);
                }

                lineInner = readerSchema.readLine();
                if (null != lineInner && lineInner.startsWith("PRIMARY KEY NUMBER")) {
                    String[] lineInnerInfo = lineInner.split(" ");
                    int indexNumber = Integer.valueOf(lineInnerInfo[lineInnerInfo.length - 1].trim());

                    List<MyIndex> primaries = new ArrayList<MyIndex>();
                    for (int i = 0; i < indexNumber; ++i) {
                        String lineIndex = readerSchema.readLine();
                        MyIndex column = new MyIndex(JSONObject.parseObject(lineIndex));
                        primaries.add(column);
                    }

                    tableInfo.setPrimaryKey(primaries);
                }

                tableInfo.setCharacterColumns(tableInfo.getColumns());
                tableInfo.setDateTimeColumns(tableInfo.getColumns());
                tableInfo.setDecimalColumns(tableInfo.getColumns());
                tableInfo.setNumberColumns(tableInfo.getColumns());
                tableInfo.setPrimaryNames(tableInfo.getIndex());

                schemaInfo.addTableInfo(lineOuterInfo[3], tableInfo);
            }
        }
    }


    private DataRecord serializeRecord(String[] recordValues) {
        DataRecord record = new DataRecord(recordValues[1], recordValues[2], Operation.INSERT);

        TableInfo tableInfo = schemaInfo.getTableInfoMap().get(record.getTable());

        record.setAfterImages(getColumnValue(recordValues, tableInfo));

        return record;
    }

    private Map<String, String> getColumnValue(String[] recordValues, TableInfo tableInfo) {
        Map<String, String> afterImages = new LinkedHashMap<String, String>(3);

        for (int i = 0; i < tableInfo.getColumnNum(); ++i) {
            afterImages.put(tableInfo.getColumns().get(i).getName(), recordValues[3 + i]);
        }

        return afterImages;
    }

    private DataRecord cleanDataRecord(TableInfo tableInfo, DataRecord record) {

        // data clean.
        // 1.duplicate primary key data; 2.exceed char length data; 3.error date time type data; 4.error decimal type data; 5.error data type.

        Map<String, String> after = record.getAfterImages();

        List<MyColumn> charColumns = tableInfo.getCharacterColumns();
        for (MyColumn column : charColumns) {
            if (after.get(column.getName()).length() > column.getLength()) {
                after.put(column.getName(), after.get(column.getName()).substring(0, column.getLength()));
            }
        }

        List<MyColumn> dateTimeColumns = tableInfo.getDateTimeColumns();
        for (MyColumn column : dateTimeColumns) {
            if (!isValidDateTime(after.get(column.getName()))) {
                after.put(column.getName(), "2020-04-01 00:00:00.0");
            }
        }

        List<MyColumn> decimalColumns = tableInfo.getDecimalColumns();
        for (MyColumn column : decimalColumns) {
            if (!isValidDecimal(after.get(column.getName()), column)) {
                after.put(column.getName(), getDecimalValue(after.get(column.getName()), column));
            }
        }

        List<MyColumn> numberColumns = tableInfo.getNumberColumns();
        for (MyColumn column : numberColumns) {
            if (!isValidDataType(after.get(column.getName()))) {
                after.put(column.getName(), "0");
            }
        }

        return record;
    }

    private long getDuplicateValues(TableInfo tableInfo, DataRecord record) {

        long duplicateValues = 0;

        for (MyIndex indexName : tableInfo.getPrimaryKey()) {
            duplicateValues = duplicateValues * 300000 + Long.valueOf(record.getAfterImages().get(indexName.getIndexCols().get(0)));
        }

        return duplicateValues;
    }

    private boolean isValidDateTime(String dateTime) {
        return !(dateTime != null && (dateTime.length() != 21 || dateTime.matches(REGEX_STR)));
    }

    private boolean isValidDecimal(String decimalValues, MyColumn column) {

        if (-1 == decimalValues.indexOf('.')) {
            return true;
        }

        String decimalStr = decimalValues.substring(decimalValues.indexOf('.') + 1);

        if (decimalStr.length() <= 0) {
            return true;
        }

        if (decimalStr.length() > column.getScale()) {
            return false;
        }

        return true;
    }

    private String getDecimalValue(String decimalValues, MyColumn column) {

        String strFormat = String.format("%s%df", "%.", column.getScale());
        return String.format(strFormat, Double.valueOf(decimalValues));

    }

    private boolean isValidDataType(String intValue) {
        for (int i = 0; i < intValue.length(); i++) {
            char c = intValue.charAt(i);
            if (Character.isLowerCase(c) || Character.isUpperCase(c)) {
                return false;
            }
        }
        return true;
    }
}
