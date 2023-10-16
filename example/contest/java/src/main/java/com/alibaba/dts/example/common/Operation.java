package com.alibaba.dts.example.common;

/**
 * @author dts
 */
public enum Operation {
    INSERT("I"),
    DELETE("D");

    private String type;

    Operation(String type) {
        this.type = type;
    }

    public String getType() {
        return this.type;
    }
}