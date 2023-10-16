package com.alibaba.dts.demo.common;

import com.alibaba.fastjson.JSONObject;

import java.util.HashMap;
import java.util.Map;

/**
 * @author dts, just for demo.
 */
public class DataRecord {

    private String database;
    private String table;
    private Map<String, String> beforeImages;
    private Map<String, String> afterImages;
    private String operation;

    public DataRecord(String database, String table, String operation) {
        this.database = database;
        this.table = table;
        this.beforeImages = new HashMap<String, String>(3);
        this.afterImages = new HashMap<String, String>(3);
        this.operation = operation;
    }

    public void setBeforeImages(Map<String, String> images) {
        this.beforeImages = images;
    }

    public Map<String, String> getBeforeImages() {
        return this.beforeImages;
    }

    public void setAfterImages(Map<String, String> images) {
        this.afterImages = images;
    }

    public Map<String, String> getAfterImages() {
        return this.afterImages;
    }

    public String getDatabase() {
        return this.database;
    }

    public String getTable() {
        return this.table;
    }

    public void setOperation(String operation) {
        this.operation = operation;
    }

    public String getOperation() {
        return this.operation;
    }

    @Override
    public String toString() {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("databaseName", getDatabase());
        jsonObject.put("tableName", getTable());
        jsonObject.put("operation", getOperation());

        JSONObject beforeImageJsonObject = new JSONObject();
        for(Map.Entry<String, String> image : beforeImages.entrySet()) {
            beforeImageJsonObject.put(image.getKey(), image.getValue());
        }
        jsonObject.put("beforeImage", beforeImageJsonObject);

        JSONObject afterImageJsonObject = new JSONObject();
        for(Map.Entry<String, String> image : afterImages.entrySet()) {
            afterImageJsonObject.put(image.getKey(), image.getValue());
        }
        jsonObject.put("afterImage", afterImageJsonObject);

        return jsonObject.toString();
    }

    @Override
    public DataRecord clone() {
        DataRecord record = new DataRecord(this.database, this.table, this.operation);

        record.getBeforeImages().putAll(this.beforeImages);
        record.getAfterImages().putAll(this.afterImages);

        return record;
    }
}
