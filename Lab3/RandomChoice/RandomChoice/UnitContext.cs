using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RandomChoice
{
    public class UnitContext : DbContext
    {
        public DbSet<IntencityAverageTime> IntencityAverage { get; set; }

        public DbSet<SetbackIntensity> SetbackIntensity { get; set; }

        public DbSet<NumWait> NumWait { get; set; }


        static UnitContext()
        {
            Database.SetInitializer(new StoreDbInitializer());
        }
        public UnitContext()
            : base("Server=(local)\\sqlexpress; Database=Units; Trusted_Connection=True; MultipleActiveResultSets=True;")
        {
        }
    }

    public class StoreDbInitializer : DropCreateDatabaseIfModelChanges<UnitContext>
    {
        protected override void Seed(UnitContext db)
        {

           

        }
    }
}
