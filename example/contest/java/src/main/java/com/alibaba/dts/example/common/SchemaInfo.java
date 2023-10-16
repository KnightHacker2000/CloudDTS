package com.alibaba.dts.example.common;

import java.util.HashMap;
import java.util.Map;

/**
 * @author dts
 */
public class SchemaInfo {

    private String databaseName;
    private Map<String, TableInfo> tableInfoMap;

    public SchemaInfo(int size) {
        tableInfoMap = new HashMap<String, TableInfo>(size);
    }

    public void addTableInfo(String table, TableInfo tableInfo) {
        tableInfoMap.put(table, tableInfo);
    }

    public void addAllTableInfo(Map<String, TableInfo> tableInfos) {
        tableInfoMap.putAll(tableInfos);
    }

    public Map<String, TableInfo> getTableInfoMap() {
        return tableInfoMap;
    }

    public void setDatabaseName(String databaseName) {
        this.databaseName = databaseName;
    }
}
