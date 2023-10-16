package com.alibaba.dts.example.common;

import java.util.LinkedHashMap;
import java.util.Map;

/**
 * @author dts
 */
public class DataRecord {

    private String database;
    private String table;
    private Map<String, String> beforeImages;
    private Map<String, String> afterImages;
    private Operation operation;

    public DataRecord(String database, String table, Operation operation) {
        this.database = database;
        this.table = table;
        this.beforeImages = new LinkedHashMap<String, String>(3);
        this.afterImages = new LinkedHashMap<String, String>(3);
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

    public void setOperation(Operation operation) {
        this.operation = operation;
    }

    public Operation getOperation() {
        return this.operation;
    }

    @Override
    public String toString() {
        StringBuilder build  = new StringBuilder();

        build.append(operation.getType()).append("\t");

        build.append(this.getDatabase()).append("\t");

        build.append(this.getTable()).append("\t");

        for(Map.Entry<String, String> image : beforeImages.entrySet()) {
            build.append(image.getValue()).append("\t");
        }

        for(Map.Entry<String, String> image : afterImages.entrySet()) {
            build.append(image.getValue()).append("\t");
        }

        build.setLength(build.length() - 1);

        return build.toString();
    }

    @Override
    public DataRecord clone() {
        DataRecord record = new DataRecord(this.database, this.table, this.operation);
        record.setOperation(this.operation);

        record.getBeforeImages().putAll(this.beforeImages);
        record.getAfterImages().putAll(this.afterImages);

        return record;
    }

    public String myToString() {
        StringBuilder build  = new StringBuilder();

        for(Map.Entry<String, String> image : beforeImages.entrySet()) {
            build.append(image.getValue()).append("\t");
        }

        for(Map.Entry<String, String> image : afterImages.entrySet()) {
            build.append(image.getValue()).append("\t");
        }

        build.setLength(build.length() - 1);

        return build.toString();
    }
}
