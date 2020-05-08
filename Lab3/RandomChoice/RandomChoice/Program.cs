using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;

namespace RandomChoice
{
    class Program
    {

        public static List<Unit> allUnits = CreateUnits(50);
        public static List<Unit> queue = new List<Unit>();
        public static double secondsGone = 0;
        public static float CameUnits = 0;
        public static int timeOfOccupation = 0;
        public static int numberOfUnits = 50;
        public static int numOfUncompleted = 50;
        public static int timeOfAppearence = 0;

        private static Timer aTimer;
       
        static void Main(string[] args)
        {
            // Create a timer and set a two second interval.
            aTimer = new Timer
            {
                Interval = 1000
            };

            // Hook up the Elapsed event for the timer. 
            aTimer.Elapsed += OnTimedEvent;

            // Have the timer fire repeated events (true is the default)
            aTimer.AutoReset = true;

            // Start the timer
            aTimer.Enabled = true;

            Console.WriteLine("Press the Enter key to exit the program at any time... ");
            Console.ReadLine();
        }

        public static List<Unit> CreateUnits(int num)
        {
            List<Unit> units = new List<Unit>();

            Random randForAppearence = new Random();
            Random randForExecution = new Random();

            for(int i = 0;i<num;i++)
            {
                timeOfAppearence++;
                timeOfAppearence++;
               
       
                int timeOfExecution = randForExecution.Next(9);
               
                units.Add(new Unit() { TimeOfAppearence = timeOfAppearence, TimeOfExecution = timeOfExecution });
            }

            return units;
        }

        private static void OnTimedEvent(Object source, ElapsedEventArgs e)
        {
           
                //Console.WriteLine("The Elapsed event was raised at {0}", e.SignalTime);
                secondsGone++;
            Console.WriteLine("CameUnits: " + CameUnits);
            Console.WriteLine("Seconds gone: "+secondsGone);
            Console.WriteLine(allUnits.Count);
            int average = CalculateAverageWaiting();
            Console.WriteLine("Average waiting: " + CalculateAverageWaiting());
            float intensity = 0;
           
            float setback = 0;
            float uncomp = numOfUncompleted;
            float num = numberOfUnits;
            if(numOfUncompleted!=0)
            {
                setback = uncomp / num;

            }

            Console.WriteLine(setback);
            
            for(int i = 0;i<allUnits.Count;i++)
            {
                if(allUnits[i].TimeOfAppearence==secondsGone)
                {
                    queue.Add(allUnits[i]);
                    Console.WriteLine("Unit time appearence: " + allUnits[i].TimeOfAppearence);
                    CameUnits++;
                    intensity++;
                }
            }
            Console.WriteLine("Intensity: " + intensity);
            if (timeOfOccupation != 0)
                timeOfOccupation--;
            else
            {
                if(queue.Count!=0)
                {
                    int count = queue.Count;
                    Random rand = new Random();
                    int randInt = rand.Next(count - 1);
                    timeOfOccupation = queue[randInt].TimeOfExecution;
                    queue.RemoveAt(randInt);
                    if(numOfUncompleted!=0)
                    {
                        numOfUncompleted--;
                    }
                    
                }
              
            }
            
            Console.WriteLine("Queue count: " + queue.Count);
            Console.WriteLine("------------------------------------------");
         

            using (UnitContext db = new UnitContext())
            {
                IntencityAverageTime intencityAverageTime = new IntencityAverageTime()
                {
                    Intencity = intensity,
                    AverageTime = average
                
                };

                SetbackIntensity setbackIntensity = new SetbackIntensity()
                {
                    Intencity = intensity,
                    Setback = setback
                };

                NumWait numWait = new NumWait()
                {
                    UnitNum = queue.Count,
                    AverageTime = average
                };


                db.IntencityAverage.Add(intencityAverageTime);
                db.SetbackIntensity.Add(setbackIntensity);
                db.NumWait.Add(numWait);
                db.SaveChanges();
                
            }
        }

        public static int CalculateAverageWaiting()
        {
            if (queue.Count == 0)
                return 0;
            else
            {
                List<int> averageTimes = new List<int>();


                for (int i = 0; i < queue.Count; i++)
                {
                    int sum = 0;
                    for (int j = 0; j < queue.Count; j++)
                    {
                        if (i != j)
                        {
                            sum = sum + queue[j].TimeOfExecution;
                        }
                        if (j == queue.Count - 1)
                        {
                            averageTimes.Add(sum);
                        }

                    }
                }
                int average = 0;
                for (int i = 0; i < averageTimes.Count; i++)
                {
                    average = average + averageTimes[i];
                }

                return average / averageTimes.Count;
            }
          
        }


    }
}
