package com.alibaba.dts.judge;

import org.apache.commons.lang3.StringUtils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.math.BigInteger;
import java.security.MessageDigest;

/**
 * @author dts, just for demo.
 */
public class ResultJudge {

    private static final String EXPECT_FILE_DIR = "expect_file_dir";
    private static final String SINK_FILE_DIR = "sink_file_dir";
    private static final String EXPECT_FILE_NAME_TEMPLATE = "tianchi_dts_expect_data_%s";
    private static final String SINK_FILE_NAME_TEMPLATE = "tianchi_dts_sink_data_%s";
    private static final String CHECK_TABLE_SETS = "customer,district,item,new_orders,order_line,orders,stock,warehouse";

    private static boolean checkSuccess = true;

    private String expectDirectory;
    private String resultDirectory;

    private String checkFailedTable;

    public static void main(String[] args) throws IOException {
        ResultJudge resultJudge = new ResultJudge();

        for (int i = 0; i< args.length ; ++i) {
            if (args[i].toLowerCase().startsWith("-expectdirectory")
                    || args[i].toLowerCase().startsWith("expectdirectory")) {
                resultJudge.expectDirectory = StringUtils.substringAfterLast(args[i], "=");
            }

            if (args[i].toLowerCase().startsWith("-resultdirectory")
                    || args[i].toLowerCase().startsWith("resultdirectory")) {
                resultJudge.resultDirectory = StringUtils.substringAfterLast(args[i], "=");
            }
        }

        String source_dir = resultJudge.getSourceTargetFileDirectory("source");
        String target_dir = resultJudge.getSourceTargetFileDirectory("target");

        String[] tables = CHECK_TABLE_SETS.split(",");

        for (String table : tables) {
            File expectFile = new File(new File(source_dir), String.format(EXPECT_FILE_NAME_TEMPLATE, table));
            File sinkFile = new File(new File(target_dir), String.format(SINK_FILE_NAME_TEMPLATE, table));

            if (!expectFile.exists() || !sinkFile.exists()) {
                resultJudge.checkFailedTable = table;
                ResultJudge.checkSuccess = false;
                System.out.println("check " + table + " failed.");
                break;
            }

            if (expectFile.length() != sinkFile.length()) {
                resultJudge.checkFailedTable = table;
                ResultJudge.checkSuccess = false;
                System.out.println("check " + table + " failed.");
                break;
            } else {
                String expectMD5 = resultJudge.getFileMD5Value(expectFile);
                String sinkMD5 = resultJudge.getFileMD5Value(sinkFile);

                if (null != expectMD5 && null != sinkMD5 && !expectMD5.equals(sinkMD5)) {
                    resultJudge.checkFailedTable = table;
                    System.out.println("check " + table + " failed.");
                    ResultJudge.checkSuccess = false;
                    break;
                }
            }

            if (resultJudge.checkFileContent(expectFile, sinkFile)) {
                resultJudge.checkFailedTable = table;
                System.out.println("check " + table + " failed.");
                ResultJudge.checkSuccess = false;
                break;
            }

            System.out.println("check table " + table + " success!");
        }

        if (checkSuccess) {
            System.out.println("######################################");
            System.out.println("check all table success!");
            System.out.println("######################################");
        } else {
            System.out.println("######################################");
            System.out.println("check table " + resultJudge.checkFailedTable + " failed!");
            System.out.println("######################################");
        }
    }

    public String getSourceTargetFileDirectory(String point) throws IOException {
        if (null == expectDirectory || expectDirectory.trim().isEmpty()) {
            expectDirectory = System.getProperty("user.dir");
        }

        File fileExpect = new File(this.expectDirectory);
        if (!fileExpect.exists()) {
            throw new IOException("not find dictionary");
        }

        if (null == resultDirectory || resultDirectory.trim().isEmpty()) {
            resultDirectory = System.getProperty("user.dir");
        }

        File fileResult = new File(this.resultDirectory);
        if (!fileResult.exists()) {
            throw new IOException("not find dictionary");
        }

        if ("source".equalsIgnoreCase(point)) {
            return expectDirectory + File.separatorChar + EXPECT_FILE_DIR;
        } else if ("target".equalsIgnoreCase(point)) {
            return resultDirectory + File.separatorChar + SINK_FILE_DIR;
        } else {
            throw new IOException("error point");
        }
    }

    private String getFileMD5Value(File file) throws IOException {
        try {
            MessageDigest messageDigest = MessageDigest.getInstance("MD5");
            InputStream fInput = new FileInputStream(file);
            byte[] buffer = new byte[1024];
            int length = 0;
            while ((length = fInput.read(buffer)) != -1) {
                messageDigest.update(buffer, 0, length);
            }
            fInput.close();
            return new BigInteger(1, messageDigest.digest()).toString(16);
        } catch (Exception ex) {
            System.out.println("calculate MD5 value failed");
            throw new IOException(ex);
        }
    }

    private boolean checkFileContent(File sourceFile, File targetFile) throws IOException {
        FileInputStream fisSource = new FileInputStream(sourceFile);
        InputStreamReader isrSource = new InputStreamReader(fisSource, "UTF-8");
        BufferedReader readerSource = new BufferedReader(isrSource);

        FileInputStream fisTarget = new FileInputStream(sourceFile);
        InputStreamReader isrTarget = new InputStreamReader(fisTarget, "UTF-8");
        BufferedReader readerTarget = new BufferedReader(isrTarget);

        String sourceContent = null;
        String targetContent = null;

        while ((sourceContent = readerSource.readLine()) != null || (targetContent = readerTarget.readLine()) != null) {
            if (null != sourceContent || sourceContent.equals(targetContent)) {
                return false;
            }
        }

        return true;
    }
}
