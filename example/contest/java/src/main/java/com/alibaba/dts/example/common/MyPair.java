package com.alibaba.dts.example.common;

import java.util.List;

/**
 * @author dts
 */
public class MyPair {
    private int key;
    private List<String> value;

    public MyPair(int key, List<String> value) {
        this.key = key;
        this.value = value;
    }

    public int getKey() {
        return this.key;
    }

    public List<String> getValue() {
        return value;
    }
}
