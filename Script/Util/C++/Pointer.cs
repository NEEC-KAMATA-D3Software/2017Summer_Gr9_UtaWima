using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Script
{
    public class Pointer
    {

        /// <summary>
        /// ポインターからデータにキャスト
        /// </summary>
        /// <typeparam name="Type">キャストするデータ型</typeparam>
        /// <param name="pointer">キャストするポインター</param>
        /// <returns>データ</returns>
        public static Type PointerCast<Type>(int pointer)
        {
            System.IntPtr ptr = new System.IntPtr(pointer);
            Type data = (Type)Marshal.PtrToStructure(ptr, typeof(Type));

            return data;
        }

        /// <summary>
        /// ポイターにデータを上書き
        /// </summary>
        /// <typeparam name="Type">上書きするデータの型</typeparam>
        /// <param name="pointer">上書きするポインタ</param>
        /// <param name="data">上書きするデータ</param>
        public static void Override<Type>(int pointer,Type data)
        {
            System.IntPtr ptr = new System.IntPtr(pointer);

            Marshal.StructureToPtr(data, ptr, true);
        }


    }
}
