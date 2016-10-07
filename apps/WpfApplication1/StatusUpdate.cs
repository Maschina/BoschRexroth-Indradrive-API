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
        private Label       m_speedlabel;
        private Label       m_diaglabel;
        private TimeSpan    m_period;

        private Indradrive  m_indradrlib;

        private Timer timerSpeedUpdate;
        private Timer timerDiagUpdate;


        public StatusUpdate(TimeSpan period, ref Indradrive indralib, ref Label speedlabel, ref Label diaglabel)
        {
            m_period        = period;
            m_indradrlib    = indralib;
            m_speedlabel    = speedlabel;
            m_diaglabel     = diaglabel;
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

            m_speedlabel.Content = speed.ToString();
        }


        private void getDiagnostic(Object state)
        {
            Byte[] diag = new Byte[256];
            m_indradrlib.get_diagnostic_msg(ref diag);

            m_diaglabel.Content = diag.ToString();
        }
    }
}
