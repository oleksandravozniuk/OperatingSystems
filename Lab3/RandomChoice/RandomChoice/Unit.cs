using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RandomChoice
{
    class Unit
    {
        public int TimeOfAppearence { get; set; }//время появления в системе

        public int TimeOfExecution { get; set; }//время выполения

        public int TimeOfCompletion//время выхода

        {
            get
            {
                return TimeOfAppearence + TimeOfExecution;
            }
        }

    }
}
