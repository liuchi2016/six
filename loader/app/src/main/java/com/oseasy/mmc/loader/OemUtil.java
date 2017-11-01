/*
 * Copyright (C) 2015-2018 Wuhan OS-easy Technology Co.,ltd. All rights reserved.
 */
package com.oseasy.mmc.loader;

import android.text.TextUtils;

/**
 * author: zhaowei
 * date: 2017/4/13
 * email: zhaowei@os-easy.com
 * description:OEM 工具类
 */
public class OemUtil {

    public static final String OEM_TONGFANG = "tongfang";

    public static String mOem = PropertyUtil.getSystemProperty("ro.board.name");

    public static Boolean isOemTongFang() {
        return TextUtils.equals(OEM_TONGFANG, mOem);
    }

}
