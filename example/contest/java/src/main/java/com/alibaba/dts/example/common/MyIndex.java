package com.alibaba.dts.example.common;

import com.alibaba.fastjson.JSONObject;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * @author dts
 */
public class MyIndex {

    private String indexName;
    private List<String> indexCols;
    private boolean primary;
    private boolean unique;

    public MyIndex(JSONObject object) {
        this.indexName = object.getString("IndexName");

        String indexColsTemp = object.getString("IndexCols");
        this.indexCols = new ArrayList<String>(Arrays.asList(indexColsTemp.substring(2, indexColsTemp.length() -2)));

        this.primary = object.getBoolean("Primary");
        this.unique = object.getBoolean("Unique");
    }

    public String getIndexName() {
        return this.indexName;
    }

    public List<String> getIndexCols() {
        return this.indexCols;
    }

    public boolean getPrimary() {
        return this.primary;
    }

    public boolean getUnique() {
        return this.unique;
    }
}
