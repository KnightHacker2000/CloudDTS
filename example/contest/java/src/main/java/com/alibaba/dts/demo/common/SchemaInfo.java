package com.alibaba.dts.demo.common;

import java.util.HashMap;
import java.util.Map;

/**
 * @author dts, just for demo.
 */
public class SchemaInfo {
    public Map<String, TableInfo> tableInfoMap;

    public SchemaInfo(int size) {
        tableInfoMap = new HashMap<String, TableInfo>(size);
    }

    public void addAllTableInfo(Map<String, TableInfo> tableInfos) {
        tableInfoMap.putAll(tableInfos);
    }

    public Map<String, TableInfo> getTableInfoMap() {
        return tableInfoMap;
    }
}
