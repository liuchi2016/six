/*
 * Copyright (C) 2015-2018 Wuhan OS-easy Technology Co.,ltd. All rights reserved.
 */

package com.oseasy.mmc.multiclient;
import android.content.ContentResolver;
import android.content.Context;
import android.provider.Settings;

/**
 * author: zhaowei
 * date: 2016/12/22
 * email: zhaowei@os-easy.com
 * description: 属性设置工具类
 */

public class PropertyUtil {
    public static String getProperty(Context context,String key) {
        ContentResolver contentResolver = context.getContentResolver();
        return Settings.System.getString(contentResolver, key);
    }
}
