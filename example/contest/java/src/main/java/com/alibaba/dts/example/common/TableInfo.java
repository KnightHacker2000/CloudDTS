package com.alibaba.dts.example.common;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * @author dts
 */
public class TableInfo {

    private static final Set<String> NUMBER_SETS = new HashSet<String>(3);
    private static final Set<String> CHARACTER_SETS = new HashSet<String>(3);
    private static final Set<String> DATE_TIME_SETS = new HashSet<String>(3);
    private static final Set<String> DECIMAL_SETS = new HashSet<String>(3);

    static {
        NUMBER_SETS.add("int(11)");NUMBER_SETS.add("tinyint(4)");NUMBER_SETS.add("smallint(6)");NUMBER_SETS.add("bigint(20)");
        NUMBER_SETS.add("decimal(4,2)");NUMBER_SETS.add("decimal(12,2)");NUMBER_SETS.add("decimal(5,2)");NUMBER_SETS.add("decimal(6,2)");
        NUMBER_SETS.add("decimal(8,0)");


        CHARACTER_SETS.add("char(24)");CHARACTER_SETS.add("varchar(50)");CHARACTER_SETS.add("varchar(24)");
        CHARACTER_SETS.add("varchar(10)");CHARACTER_SETS.add("char(2)");CHARACTER_SETS.add("char(9)");
        CHARACTER_SETS.add("char(16)");CHARACTER_SETS.add("varchar(20)");CHARACTER_SETS.add("varchar(16)");

        DATE_TIME_SETS.add("datetime");

        DECIMAL_SETS.add("decimal(5,2)");DECIMAL_SETS.add("decimal(6,2)");DECIMAL_SETS.add("decimal(12,2)");
        DECIMAL_SETS.add("decimal(4,2)");DECIMAL_SETS.add("decimal(8,0)");
    }

    private String databaseName;
    private String tableName;
    private List<MyColumn> columns;

    private List<MyColumn> numberColumns;
    private List<MyColumn> characterColumns;
    private List<MyColumn> dateTimeColumns;
    private List<MyColumn> decimalColumns;

    private List<MyIndex> indexes;
    private List<MyIndex> primaryKey;
    private Set<String> primaryNames;

    public TableInfo(String databaseName, String tableName) {
        this.databaseName = databaseName;
        this.tableName = tableName;
        this.columns = new ArrayList<MyColumn>(3);
        this.indexes = new ArrayList<MyIndex>(3);
        this.primaryKey = new ArrayList<MyIndex>(3);
        this.numberColumns = new ArrayList<MyColumn>(3);
        this.dateTimeColumns = new ArrayList<MyColumn>(3);
        this.characterColumns = new ArrayList<MyColumn>(3);
        this.decimalColumns = new ArrayList<MyColumn>(3);
        this.primaryNames = new HashSet<String>(3);
    }

    public void clear() {
        columns.clear();
        indexes.clear();
        primaryKey.clear();
        numberColumns.clear();
        dateTimeColumns.clear();
        characterColumns.clear();
        decimalColumns.clear();
    }

    public void setColumns(List<MyColumn> columns) {
        this.columns.addAll(columns);
    }

    public List<MyColumn> getColumns() {
        return columns;
    }

    public int getColumnNum() {
        return columns.size();
    }

    public void setNumberColumns(List<MyColumn> columns) {
        for (MyColumn column : columns) {
            if (NUMBER_SETS.contains(column.getColumnDef().toLowerCase())) {
                this.numberColumns.add(column);
            }
        }
    }

    public List<MyColumn> getNumberColumns() {
        return this.numberColumns;
    }

    public void setCharacterColumns(List<MyColumn> columns) {
        for (MyColumn column : columns) {
            if (CHARACTER_SETS.contains(column.getColumnDef().toLowerCase()) && null != column.getLength()) {
                this.characterColumns.add(column);
            }
        }
    }

    public List<MyColumn> getCharacterColumns() {
        return this.characterColumns;
    }

    public void setDecimalColumns(List<MyColumn> columns) {
        for (MyColumn column : columns) {
            if (DECIMAL_SETS.contains(column.getColumnDef().toLowerCase())) {
                this.decimalColumns.add(column);
            }
        }
    }

    public List<MyColumn> getDecimalColumns() {
        return this.decimalColumns;
    }

    public void setIndexes(List<MyIndex> indexes) {
        this.indexes.addAll(indexes);
    }

    public List<MyIndex> getIndex() {
        return indexes;
    }

    public int getIndexNum() {
        return indexes.size();
    }

    public void setPrimaryKey(List<MyIndex> indexes) {
        for (MyIndex index : indexes) {
            if("PRIMARY".equalsIgnoreCase(index.getIndexName()) && index.getPrimary()) {
                primaryKey.add(index);
            }
        }
    }

    public List<MyIndex> getPrimaryKey() {
        return primaryKey;
    }

    public void setPrimaryNames(List<MyIndex> indexes) {
        for (MyIndex index : indexes) {
            if("PRIMARY".equalsIgnoreCase(index.getIndexName()) && index.getPrimary()) {
                primaryNames.add(index.getIndexCols().get(0));
            }
        }
    }

    public Set<String> getPrimaryNames() {
        return this.primaryNames;
    }

    public int getPrimaryKeyNum() {
        return primaryKey.size();
    }

    public void setDateTimeColumns(List<MyColumn> columns) {
        for (MyColumn column : columns) {
            if (DATE_TIME_SETS.contains(column.getColumnDef().toLowerCase())) {
                this.dateTimeColumns.add(column);
            }
        }
    }

    public List<MyColumn> getDateTimeColumns() {
        return this.dateTimeColumns;
    }

    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder();
        builder.append("MySQLTableMetaInfo: ").append(this.databaseName);
        builder.append(".").append(this.tableName);

        builder.append("\nColumn Info:");
        for (MyColumn column : this.columns) {
            builder.append("\n\t").append(column.toString());
        }

        builder.append("\nIndex Info:");
        for (MyIndex index : indexes) {
            builder.append("\n\t").append(index.getIndexName()).append("[");
            for (String indexColumn : index.getIndexCols()) {
                builder.append(indexColumn).append(",");
            }
            builder.setLength(builder.length() - 1);
            builder.append("]");
        }

        if (null != primaryKey && !primaryKey.isEmpty()) {
            builder.append("\nPrimary Key Info:");
            builder.append("\n\t").append(primaryKey.get(0).getIndexName()).append("[");
            for (MyIndex indexColumn : primaryKey) {
                builder.append(indexColumn.getIndexCols()).append(",");
            }
            builder.setLength(builder.length() - 1);
            builder.append("]");
        }

        return builder.toString();
    }
}
