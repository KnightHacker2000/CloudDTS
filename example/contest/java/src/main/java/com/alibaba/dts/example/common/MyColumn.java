package com.alibaba.dts.example.common;

import com.alibaba.fastjson.JSONObject;

/**
 * @author dts
 */
public class MyColumn {

    private String name;
    private Integer ordinal;
    private Boolean unsigned;
    private String charSet;
    private String defType;
    private String columnDef;
    private Integer length;
    private Integer precision;
    private Integer scale;

    public MyColumn(JSONObject object) {
        this.name = object.getString("Name");
        this.ordinal = object.getInteger("Ordinal");
        this.unsigned = object.getBoolean("Unsigned");
        this.charSet = object.getString("CharSet");
        this.defType = object.getString("DefType");
        this.columnDef = object.getString("ColumnDef");
        this.length = object.getInteger("Length");
        this.precision = object.getInteger("Precision");
        this.scale = object.getInteger("Scale");
    }

    public String getName() {
        return this.name;
    }

    public Integer getOrdinal() {
        return this.ordinal;
    }

    public Boolean getUnsigned() {
        return this.unsigned;
    }

    public String getCharSet() {
        return this.charSet;
    }

    public String getDefType() {
        return this.defType;
    }

    public String getColumnDef() {
        return this.columnDef;
    }

    public Integer getLength() {
        return this.length;
    }

    public Integer getPrecision() {
        return this.precision;
    }

    public Integer getScale() {
        return this.scale;
    }
}

