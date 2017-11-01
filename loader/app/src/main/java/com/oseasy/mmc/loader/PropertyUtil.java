/*
 * Copyright (C) 2015-2018 Wuhan OS-easy Technology Co.,ltd. All rights reserved.
 */

package com.oseasy.mmc.loader;
import android.content.ContentResolver;
import android.content.Context;
import android.provider.Settings;
import java.lang.reflect.Method;
import java.lang.reflect.InvocationTargetException;

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

    public static String getSystemProperty(String key) {
        String result = null;
        try {
            Class onwClass = Class.forName("android.os.SystemProperties");
            Object object = onwClass.newInstance();
            Method get = onwClass.getMethod("get", new Class[]{ String.class });
            get.setAccessible(true);
            Object [] arguments = new Object[] { key };
            result = (String) get.invoke(object, arguments);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        return result;
    }
}
