package com.alibaba.dts.demo.common;

import java.util.ArrayList;
import java.util.List;

/**
 * @author dts, just for demo.
 */
public class TableInfo {

    private String databaseName;
    private String tableName;
    private List<DatabaseColumn> columns;
    private List<DatabaseIndex> indexes;
    private List<DatabaseIndex> primaryKey;

    public TableInfo(String databaseName, String tableName) {
        this.databaseName = databaseName;
        this.tableName = tableName;
        this.columns = new ArrayList<DatabaseColumn>(3);
        this.indexes = new ArrayList<DatabaseIndex>(3);
    }

    public void clear() {
        columns.clear();
        indexes.clear();
    }

    public void setColumns(List<DatabaseColumn> columns) {
        this.columns.addAll(columns);
    }

    public void setIndexes(List<DatabaseIndex> indexes) {
        this.indexes.addAll(indexes);
    }

    public List<DatabaseColumn> getColumns() {
        return columns;
    }

    public void setPrimaryKey(List<DatabaseIndex> index) {
        this.primaryKey = index;
    }

    public List<DatabaseIndex> getPrimaryKey() {
        return primaryKey;
    }

    public List<DatabaseIndex> getIndex() {
        return indexes;
    }

    public int getColumnNum() {
        return columns.size();
    }

    public int getIndexNum() {
        return indexes.size();
    }

    public int getPrimaryKeyNum() {
        return primaryKey.size();
    }

    @Override
    public String toString() {

        // add.

        return null;
    }
}
