/*
 * Copyright (C) 2015-2018 Wuhan OS-easy Technology Co.,ltd. All rights reserved.
 */

package com.oseasy.mmc.multiclient;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;


/**
 * author: zhaowei
 * date: 2016/12/26
 * email: zhaowei@os-easy.com
 * description: shell 命令管理工具类
 */
public class ShellUtil {
    //系统是否root未知
    private static final int SYSTEM_NONE = -1;
    //系统未root
    private static final int SYSTEM_NORMAL = 0;
    //系统已root
    private static final int SYSTEM_ROOTED = 1;
    private static int mSystemStatus = SYSTEM_NONE;

    private static final String TEST_FILE_NAME = "data/data/touch.txt";
    private static final int TIMEOUT = 6000;
    private static final String OPTION = "-c";
    private static final String SU = "su";
    private static final String SH = "sh";

    private static final String SHELL_RM = "rm -rf %s";
    private static final String SHELL_TOUCH = "touch %s";
    /**
     * If system rooted return true, else return false
     * @return
     */
    public static boolean isRootAvailable() {

        if (getSystemStatus() == SYSTEM_NORMAL) {
            return false;
        }

        if (getSystemStatus() == SYSTEM_ROOTED) {
            return true;
        }

        File file = new File(TEST_FILE_NAME);
        Boolean isExist = file.exists();
        String command = isExist ? SHELL_RM : SHELL_TOUCH;
        Boolean isOk = exec(SU, String.format(command, TEST_FILE_NAME));

        if (isOk) {
            setSystemStatus(SYSTEM_ROOTED);
            return true;
        } else {
            setSystemStatus(SYSTEM_NORMAL);
            return false;
        }
    }

    /**
     * Execute shell command and return result
     * @param command
     * @param isMultiline
     * @return
     */
    public static String exec(String command, Boolean isMultiline) {
        String user = isRootAvailable() ? SU : SH;
        Process process = execInProcess(user, command, TIMEOUT);
        if (process == null) {
            return null;
        }
        InputStreamReader inputStreamReader = new InputStreamReader(process.getInputStream());
        BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
        String result = null;
        String line = "";
        while (line != null) {
            try {
                line = bufferedReader.readLine();
                if (line != null && !isMultiline)
                    result = line;
                if (line != null && isMultiline)
                    result += ";" + line;
            } catch (IOException e) {
                e.printStackTrace();
                return null;
            }
        }
        process.destroy();
        return result;
    }

    /**
     * Execute shell command
     * @param command
     * @return
     */
    public static Boolean exec(String command) {
        String user = isRootAvailable() ? SU : SH;
        Process process = execInProcess(user, command, TIMEOUT);
        if (process == null) {
            return false;
        }
        process.destroy();
        return true;
    }

    /**
     * Execute shell command as user
     * @param user
     * @param command
     * @return
     */
    public static Boolean exec(String user, String command) {
        return exec(user, command, TIMEOUT);
    }

    /**
     * Execute shell command as user with timeout
     * @param user
     * @param command
     * @param timeout
     * @return
     */
    public static Boolean exec(String user, String command, Integer timeout) {
        Process process = execInProcess(user, command, timeout);
        if (process == null) {
            return false;
        }
        process.destroy();
        return true;
    }

    private static Process execInProcess(String user, String command, Integer timeout) {
        try {
            Process process = Runtime.getRuntime().exec(new String[]{user, OPTION, command});
            Worker worker = new Worker(process);
            worker.start();
            if (timeout == null) {
                worker.join();
            } else {
                worker.join(timeout);
            }
            if (worker.isAlive()) {
                worker.interrupt();
            }
            if (worker.exit == null) {
                return null;
            } else {
            }
            return process;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    private static int getSystemStatus() {
        return mSystemStatus;
    }

    private static void setSystemStatus(int status) {
        ShellUtil.mSystemStatus = status;
    }

    private static class Worker extends Thread {
        private final Process process;
        private Integer exit;

        private Worker(Process process) {
            this.process = process;
        }

        public void run() {
            try {
                exit = process.waitFor();
                if (exit != 0) {
                    process.destroy();
                    exit = null;
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
