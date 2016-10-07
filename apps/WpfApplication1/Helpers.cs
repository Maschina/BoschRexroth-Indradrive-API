using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfApplication1
{
    public static class Helpers
    {
        public static Boolean ConvertString2Double(String stringVal, out Double doubleVal)
        {
            try
            {
                stringVal = stringVal.Replace(".", ",");
                doubleVal = System.Convert.ToDouble(stringVal);
                return true;
            }
            catch (System.OverflowException)
            {
                System.Console.WriteLine("Conversion from string-to-double overflowed.");
                doubleVal = 0.0;
                return false;
            }
            catch (System.FormatException)
            {
                System.Console.WriteLine("The string was not formatted as a double.");
                doubleVal = 0.0;
                return false;
            }
            catch (System.ArgumentException)
            {
                System.Console.WriteLine("The string pointed to null.");
                doubleVal = 0.0;
                return false;
            }
        }

        public static Boolean ConvertString2UInt(String stringVal, out UInt32 intVal)
        {
            try
            {
                intVal = System.Convert.ToUInt32(stringVal);
                return true;
            }
            catch (System.OverflowException)
            {
                System.Console.WriteLine("Conversion from string-to-UInt32 overflowed.");
                intVal = 0;
                return false;
            }
            catch (System.FormatException)
            {
                System.Console.WriteLine("The string was not formatted as a UInt32.");
                intVal = 0;
                return false;
            }
            catch (System.ArgumentException)
            {
                System.Console.WriteLine("The string pointed to null.");
                intVal = 0;
                return false;
            }
        }
    }
}
