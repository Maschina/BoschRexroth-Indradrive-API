using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfApplication1
{
    class ComPorts : ObservableCollection<String>
    {
        public ComPorts()
        {
            String[] ports = SerialPort.GetPortNames();
            foreach (String port in ports)
                Add(port);
        }
    }
}
