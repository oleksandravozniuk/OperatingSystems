using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab5
{
    class Program
    {
        static void Main(string[] args)
        {
            DateTime moment1 = DateTime.Now;//first time

            //-----------------my function-------------------

            int[] C = new int[2];
            int res = 0;

            int incr = C[0];

            for (int j = 500000000; j > 0; j--)
            {
                //start variant
                //C[0]++;
                //C[0]++;

                //first modification
                //C[0]=C[0]+2

                //Second modification
                incr += 2;
               
            }
            C[0] = incr;
            C[1] = C[0];
            Console.WriteLine(C[0]);

            //---------------------------------------------------------------

            DateTime moment2 = DateTime.Now;//time after running
            TimeSpan diff = moment2.Subtract(moment1);//diff between first and second times
            
            Console.WriteLine("Time of execution in seconds:" + diff.TotalSeconds);//result in seconds
            Console.Read();
        }
    }
}
