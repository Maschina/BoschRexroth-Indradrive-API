using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace WpfApplication1
{
    class StatusUpdate
    {
        private LiveTracker m_livetracker;
        private TimeSpan    m_period;

        private Indradrive  m_indradrlib;

        private Timer timerSpeedUpdate;
        private Timer timerDiagUpdate;


        public StatusUpdate(TimeSpan period, ref Indradrive indralib, ref LiveTracker livetracker)
        {
            m_period        = period;
            m_indradrlib    = indralib;
            m_livetracker   = livetracker;
        }


        public void Run()
        {
            timerSpeedUpdate    = new Timer(getSpeed, null, new TimeSpan(0), m_period);
            timerDiagUpdate     = new Timer(getDiagnostic, null, new TimeSpan(0), m_period);
        }


        private void getSpeed(Object state)
        {
            Double speed = 0.0;
            m_indradrlib.get_speed(ref speed);

            // Invoking element since it will be accessed from other than the owner's thread
            m_livetracker.Dispatcher.BeginInvoke((System.Windows.Forms.MethodInvoker)(() =>
            {
                m_livetracker.lblSpeed.Content = speed.ToString();
                m_livetracker.addSpeedgraphValue(speed);
            }));
        }


        private void getDiagnostic(Object state)
        {
            Byte[] diag_raw = new Byte[256];
            
            m_indradrlib.get_diagnostic_msg(diag_raw);

            String diag = Encoding.ASCII.GetString(diag_raw).TrimEnd((Char)0);

            // Invoking element since it will be accessed from other than the owner's thread
            m_livetracker.Dispatcher.BeginInvoke((System.Windows.Forms.MethodInvoker)(() =>
            {
                m_livetracker.lblDiagnostic.Content = diag;
            }));
        }
    }
}
