using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Script.Animation
{
    public class Nail
    {
        List<AnimData> dataList;

        public Nail()
        {
            dataList = new List<AnimData>();

            dataList.Add(new AnimData("Nail_Move",true));
            dataList.Add(new AnimData("Nail_Normal", true));
            dataList.Add(new AnimData("Nail_Attack", true));
        }
    }
}
