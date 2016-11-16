using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Text.RegularExpressions;
using System.ComponentModel;
using System.Threading;

namespace WpfApplication1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Indradrive indralib;
        StatusUpdate status;

        /// <summary>
        /// Boolean to indicate a successful done connection
        /// </summary>
        private Boolean connected = false;

        /// <summary>
        /// Type for available and supported operation modes
        /// </summary>
        private enum opmode
        {
            unknown,
            speed,
            sequence
        }

        /// <summary>
        /// Indicating current active operation mode
        /// </summary>
        opmode operationmode = opmode.unknown;


        public MainWindow()
        {
            InitializeComponent();
            
            indralib = new Indradrive(ref listErrors);

            oLiveTracker.SetIndradriveLib(indralib);

            dataSequence.ItemsSource = new List<DataSequenceItem>();            
        }
        

        private void IntegerValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9.]{1}");
            e.Handled = regex.IsMatch(e.Text);
        }

        private void btnOpen_Click(object sender, RoutedEventArgs e)
        {
            byte[] port = Encoding.ASCII.GetBytes(cboxComPorts.Text);            
           
            // Open the COM port
            int ret = indralib.open(port, 19200);
            if (ret == 0) connected = true;


            if (!connected) return;

            get_opmode();

            status = new StatusUpdate(
                new TimeSpan(0, 0, 0, 0, 500), 
                ref indralib, 
                ref oLiveTracker
                );
            status.Run();
        }

        
        private void btnAddSequence_Click(object sender, RoutedEventArgs e)
        {
            List<DataSequenceItem> sequence = dataSequence.ItemsSource as List<DataSequenceItem>;

            // Empty line that consists of default 0s
            sequence.Add(new DataSequenceItem(0.0, 0.0, 0.0, 0));

            dataSequence.ItemsSource = sequence;
            dataSequence.Items.Refresh();
        }


        private void dataSequence_CellEditEnding(object sender, DataGridCellEditEndingEventArgs e)
        {
            /// Get current position and edited text
            string text = (e.EditingElement as TextBox).Text;     
            int row = e.Row.GetIndex();
            int col = e.Column.DisplayIndex;

            List<DataSequenceItem> sequence = dataSequence.ItemsSource as List<DataSequenceItem>;

            /// Insert the new text into sequence (error correction is done by the class DataSequenceItem) ...
            DataSequenceItem datarow = sequence[row];
            datarow.Update(col, text);

            sequence.RemoveAt(row);
            sequence.Insert(row, datarow);

            /// ... and write it back to the datagrid
            dataSequence.ItemsSource = sequence;
            //dataSequence.Items.Refresh();

            Console.WriteLine("Updated Cell[{0}:{1}] to {2} (Raw input was: {3})", row, col, sequence[row].Get(col), text);
        }


        private void btnSetSpeed_Click(object sender, RoutedEventArgs e)
        {
            if (!connected) return;
            if (operationmode != opmode.speed) return;

            /// Make sure that RELEASE is given before doing any rotation
            Byte state = 0;
            indralib.get_opstate(ref state);

            if ((state & 3) != 3)
            {
                MessageBox.Show("Please RELEASE the drive before starting!");
                return;
            }

            indralib.speedcontrol_init(1000, 1000);

            /// Perform set point control
            UInt32 speed = 0;
            Double accel = 0;
            if (Helpers.ConvertString2UInt(txtSpeed.Text, out speed) && Helpers.ConvertString2Double(txtAccel.Text, out accel))
            {
                indralib.speedcontrol_write((Double)speed, accel);
            }

            txtSpeed.SelectAll();
            txtSpeed.Focus();
        }


        private void Window_Closing(object sender, CancelEventArgs e)
        {
            indralib.close();
        }


        private void btnSpeedModeActivate_Click(object sender, RoutedEventArgs e)
        {
            if (!connected) return;

            indralib.speedcontrol_activate();

            get_opmode();
        }


        private void get_opmode()
        {
            if (!connected) return;

            UInt32 mode = 0;
            indralib.get_drivemode(ref mode);
            
            string mode_label = "Unknown";
            switch (mode)
            {
                case 1:
                    mode_label = "Sequence Control";
                    operationmode = opmode.sequence;
                    break;
                case 2:
                    mode_label = "Speed Control";
                    operationmode = opmode.speed;
                    break;
                default:
                    operationmode = opmode.unknown;
                    break;
            }

            oLiveTracker.lblMode.Content = mode_label;
        }


        private void btnRunSequence_Click(object sender, RoutedEventArgs e)
        {
            if (!connected) return;
            if (operationmode != opmode.sequence) return;
            
            /// Make sure that RELEASE is given before doing any rotation
            Byte state = 0;
            indralib.get_opstate(ref state);

            if ((state & 3) != 3)
            {
                MessageBox.Show("Please RELEASE the drive before starting!");
                return;
            }

            indralib.sequencer_init(1000, 1000);

            /// Read out control values from sequence
            
            List<Double> speeds = new List<Double>();
            List<Double> accels = new List<Double>();
            List<Double> jerks  = new List<Double>();
            List<UInt32> delays = new List<UInt32>();
            
            List<DataSequenceItem> sequence = dataSequence.ItemsSource as List<DataSequenceItem>;

            foreach (DataSequenceItem item in sequence)
            {
                speeds.Add(item.Speed);
                accels.Add(item.AccelDecel);
                jerks.Add(item.Jerk);
                delays.Add(item.Delay);
            }

           indralib.sequencer_write(
                speeds.ToArray(), 
                accels.ToArray(), 
                jerks.ToArray(), 
                delays.ToArray(), 
                (UInt16)sequence.Count
                );
        }

        private void btnSequencerModeActivate_Click(object sender, RoutedEventArgs e)
        {
            if (!connected) return;

            indralib.sequencer_activate();

            get_opmode();
        }

        private void txtSpeed_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter) btnSetSpeed_Click(sender, e);
        }

        private void txtAccel_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                txtSpeed.Focus();
                txtSpeed.SelectAll();
            }
        }
    }
}
