using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Script.Animation
{
    public struct AnimData
    {
        public AnimData(string animeName,bool isLoop)
        {
            this.animeName = animeName;
            this.isLoop = isLoop;
        }
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string animeName;
        public bool isLoop;
    }
}
