﻿using System;
using System.Runtime.InteropServices;

class Program
{
    private const string dllName = "QCloudMusicApi.dll";
    [DllImport(dllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int invoke(string memberName, string value);
    [DllImport(dllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr get_result(int key);
    [DllImport(dllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void free_result(int key);
    [DllImport(dllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr memberName(int i);
    [DllImport(dllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int memberCount();

    static string api_invoke(string memberName, string value)
    {
        int result_id = invoke(memberName, value);

        // 因为invoke返回的是char*，在C#中对应为IntPtr，所以需要进行转换
        IntPtr resultPtr = get_result(result_id);

        // 将IntPtr转换为C#中的string
        string result = Marshal.PtrToStringUTF8(resultPtr);

        // 释放资源
        free_result(result_id);
        return result;
    }

    static List<string> api_memberList()
    {
        List<string> list = [];
        var count = memberCount();
        for (int i = 0; i < count; i++)
        {
            var member = memberName(i);
            list.Add(Marshal.PtrToStringAnsi(member));
        }
        return list;
    }

    static void Main()
    {
        foreach (var member in api_memberList()) {
            Console.WriteLine(member);
        }

        Console.WriteLine("调用结果: " + api_invoke("lyric_new", "{\"id\": \"2058263032\"}"));
    }
};
