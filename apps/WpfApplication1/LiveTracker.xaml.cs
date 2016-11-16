using LiveCharts;
using LiveCharts.Configurations;
using System;
using System.ComponentModel;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;

namespace WpfApplication1
{
    /// <summary>
    /// Interaction logic for ConstantChangesChart.xaml
    /// </summary>
    public partial class LiveTracker : UserControl, INotifyPropertyChanged
    {
        private double _axisMax;
        private double _axisMin;

        private Indradrive m_indradrlib;

        public LiveTracker()
        {
            InitializeComponent();

            //To handle live data easily, in this case we built a specialized type
            //the MeasureModel class, it only contains 2 properties
            //DateTime and Value
            //We need to configure LiveCharts to handle MeasureModel class
            //The next code configures MEasureModel  globally, this means
            //that livecharts learns to plot MeasureModel and will use this config every time
            //a ChartValues instance uses this type.
            //this code ideally should only run once, when application starts is reccomended.
            //you can configure series in many ways, learn more at http://lvcharts.net/App/examples/v1/wpf/Types%20and%20Configuration

            var mapper = Mappers.Xy<SpeedOverTimeModel>()
                .X(model => model.DateTime.Ticks)   //use DateTime.Ticks as X
                .Y(model => model.Value);           //use the value property as Y

            //lets save the mapper globally.
            Charting.For<SpeedOverTimeModel>(mapper);

            //the values property will store our values array
            ChartValues = new ChartValues<SpeedOverTimeModel>();

            //lets set how to display the X Labels
            XFormatter = value => new DateTime((long)value).ToString("mm:ss");

            AxisStep = TimeSpan.FromSeconds(1).Ticks;
            SetAxisLimits(DateTime.Now);

            startTime = DateTime.Now;

            DataContext = this;
        }

        public void SetIndradriveLib(Indradrive _lib) { m_indradrlib = _lib; }

        public event PropertyChangedEventHandler PropertyChanged;

        public double AxisMax
        {
            get { return _axisMax; }
            set
            {
                _axisMax = value;
                OnPropertyChanged("AxisMax");
            }
        }

        public double AxisMin
        {
            get { return _axisMin; }
            set
            {
                _axisMin = value;
                OnPropertyChanged("AxisMin");
            }
        }

        private DateTime startTime;
        public double AxisStep { get; set; }
        public ChartValues<SpeedOverTimeModel> ChartValues { get; set; }
        public Func<double, string> XFormatter { get; set; }
        protected virtual void OnPropertyChanged(string propertyName = null)
        {
            if (PropertyChanged != null)
                PropertyChanged.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private void SetAxisLimits(DateTime now)
        {
            AxisMax = now.Ticks + TimeSpan.FromSeconds(1).Ticks; // lets force the axis to be 100ms ahead
            AxisMin = now.Ticks - TimeSpan.FromSeconds(15).Ticks; //we only care about the last 8 seconds
        }
        

        public void addSpeedgraphValue(Double speed)
        {
            DateTime now = DateTime.Now;

            ChartValues.Add(new SpeedOverTimeModel
            {
                DateTime = now,
                Value = speed
            });

            SetAxisLimits(now);

            //lets only use the last 50 values
            if (ChartValues.Count > 50) ChartValues.RemoveAt(0);
        }

        private void buttonClearError_Click(object sender, RoutedEventArgs e)
        {
            m_indradrlib.clear_error();
        }
    }

    public class SpeedOverTimeModel
    {
        public DateTime DateTime { get; set; }
        public double Value { get; set; }
    }
}