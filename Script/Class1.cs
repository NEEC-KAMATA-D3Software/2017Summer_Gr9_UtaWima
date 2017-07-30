using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Script
{
    //[ClassInterface(ClassInterfaceType.AutoDual)]
    public class Class1
    {
        public struct Data
        {
            public int n1;
            public int n2;
            public int n3;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string name;
        }

        private Data data = new Data();

        public Class1()
        {
            data.n1 = 100;
            data.n2 = 1;
        }

        //[MethodImpl(MethodImplOptions.Synchronized)]
        public int Sum(int a, int b)
        {
            return 100;
        }

        public int Test()
        {
            return 1000;
        }

        public void Test2(int structPointer)
        {
            var data = Pointer.PointerCast<Data>(structPointer);
            data = this.data;

            Pointer.Override<Data>(structPointer,data);
        }
    }
}
