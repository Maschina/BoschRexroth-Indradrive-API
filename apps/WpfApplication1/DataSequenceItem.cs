using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using LiveCharts;
using LiveCharts.Wpf;
using System.Text.RegularExpressions;
using System.IO.Ports;
using System.Collections.ObjectModel;
using System.Runtime.InteropServices;
using System.Collections;
using System.ComponentModel;
using System.Globalization;
using System.Threading;


namespace WpfApplication1
{
    public class DataSequenceItem
    {
        public String SpeedString
        {
            get { return Speed.ToString("F1", CultureInfo.CreateSpecificCulture("en-US")); }

            set
            {
                Double doubleVal = 0.0;
                if (Helpers.ConvertString2Double(value, out doubleVal)) Speed = doubleVal;
            }
        }

        public String AccelDecelString
        {
            get { return AccelDecel.ToString("F3", CultureInfo.CreateSpecificCulture("en-US")); }

            set
            {
                Double doubleVal = 0.0;
                if (Helpers.ConvertString2Double(value, out doubleVal)) AccelDecel = doubleVal;
            }
        }

        public String JerkString
        {
            get { return Jerk.ToString("F3", CultureInfo.CreateSpecificCulture("en-US")); }

            set
            {
                Double doubleVal = 0.0;
                if (Helpers.ConvertString2Double(value, out doubleVal)) Jerk = doubleVal;
            }
        }

        public String DelayString
        {
            get { return Delay.ToString("F0", CultureInfo.CreateSpecificCulture("en-US")); }

            set
            {
                UInt32 intVal = 0;
                if (Helpers.ConvertString2UInt(value, out intVal)) Delay = intVal;
            }
        }

        public Double Speed = 0.0;
        public Double AccelDecel = 0.0;
        public Double Jerk = 0.0;
        public UInt32 Delay = 0;

        public DataSequenceItem(Double _speed, Double _acceldecel, Double _jerk, UInt32 _delay)
        {
            Speed = _speed;
            AccelDecel = _acceldecel;
            Jerk = _jerk;
            Delay = _delay;
        }

        public void Update(int col, string val)
        {
            switch (col)
            {
                case 0:
                    SpeedString = val;
                    break;
                case 1:
                    AccelDecelString = val;
                    break;
                case 2:
                    JerkString = val;
                    break;
                case 3:
                    DelayString = val;
                    break;
                default:
                    break;
            }
        }

        public string Get(int col)
        {
            switch (col)
            {
                case 0:
                    return SpeedString;
                case 1:
                    return AccelDecelString;
                case 2:
                    return JerkString;
                case 3:
                    return DelayString;
                default:
                    return "";
            }
        }
    }
}