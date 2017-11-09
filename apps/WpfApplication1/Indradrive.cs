using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Controls;

namespace WpfApplication1
{
    public class Indradrive
    {
        [StructLayout(LayoutKind.Sequential)]
        public unsafe struct ErrHandle
        {
            [MarshalAs(UnmanagedType.U4)]
            public UInt32 code;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2048)]
            public byte[] msg;
        }

        private int idref;
        private const string dllpath = "..\\..\\..\\..\\bin\\x86\\IndradriveAPI.dll";

        private ErrHandle indraerr;
        private ListBox listboxerr;

        public Indradrive(ref ListBox listbox)
        {
            listboxerr = listbox;
            idref = init();
        }


        // Fundamentals

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int init();

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int open(int ID_ref, Byte[] ID_comport, UInt32 ID_combaudrate, ref ErrHandle ID_err);
        public int open(Byte[] ID_comport, UInt32 ID_combaudrate) { return CheckResult(open(idref, ID_comport, ID_combaudrate, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int close(int ID_ref, ref ErrHandle ID_err);
        public int close() { return CheckResult(close(idref, ref indraerr)); }
       
        
        // Speed Control

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int speedcontrol_activate(int ID_ref, ref ErrHandle ID_err);
        public int speedcontrol_activate() { return CheckResult(speedcontrol_activate(idref, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int speedcontrol_init(int ID_ref, Double ID_max_accel, Double ID_max_jerk, ref ErrHandle ID_err);
        public int speedcontrol_init(Double ID_max_accel, Double ID_max_jerk) { return CheckResult(speedcontrol_init(idref, ID_max_accel, ID_max_jerk, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int speedcontrol_write(int ID_ref, Double ID_speed, Double ID_accel, ref ErrHandle ID_err);
        public int speedcontrol_write(Double ID_speed, Double ID_accel) { return CheckResult(speedcontrol_write(idref, ID_speed, ID_accel, ref indraerr)); }


        // Sequencer

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int sequencer_activate(int ID_ref, ref ErrHandle ID_err);
        public int sequencer_activate() { return CheckResult(sequencer_activate(idref, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int sequencer_init(int ID_ref, Double ID_max_accel, Double ID_max_jerk, ref ErrHandle ID_err);
        public int sequencer_init(Double ID_max_accel, Double ID_max_jerk) { return CheckResult(sequencer_init(idref, ID_max_accel, ID_max_jerk, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int sequencer_write(int ID_ref, Double[] ID_speeds, Double[] ID_accels, Double[] ID_jerks, UInt32[] ID_delays, UInt16 ID_set_length, ref ErrHandle ID_err);
        public int sequencer_write(Double[] ID_speeds, Double[] ID_accels, Double[] ID_jerks, UInt32[] ID_delays, UInt16 ID_set_length) { return CheckResult(sequencer_write(idref, ID_speeds, ID_accels, ID_jerks, ID_delays, ID_set_length, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int sequencer_softtrigger(int ID_ref, ref ErrHandle ID_err);
        public int sequencer_softtrigger() { return CheckResult(sequencer_softtrigger(idref, ref indraerr)); }


        // Status

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_drivemode(int ID_ref, ref UInt32 mode, ref ErrHandle ID_err);
        public int get_drivemode(ref UInt32 mode) { return CheckResult(get_drivemode(idref, ref mode, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_opstate(int ID_ref, ref Byte state, ref ErrHandle ID_err);
        public int get_opstate(ref Byte state) { return CheckResult(get_opstate(idref, ref state, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_speed(int ID_ref, ref Double speed, ref ErrHandle ID_err);
        public int get_speed(ref Double speed) { return CheckResult(get_speed(idref, ref speed, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_diagnostic_msg(int ID_ref, Byte[] ID_diagnostic_msg, ref ErrHandle ID_err);
        public int get_diagnostic_msg(Byte[] ID_diagnostic_msg) { return CheckResult(get_diagnostic_msg(idref, ID_diagnostic_msg, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_diagnostic_num(int ID_ref, ref UInt32 ID_diagnostic_num, ref ErrHandle ID_err);
        public int get_diagnostic_num(ref UInt32 ID_diagnostic_num) { return CheckResult(get_diagnostic_num(idref, ref ID_diagnostic_num, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int clear_error(int ID_ref, ref ErrHandle ID_err);
        public int clear_error() { return CheckResult(clear_error(idref, ref indraerr)); }


        // Helpers

        public int CheckResult(int ret)
        {
            if (ret != 0)
            {
                String err = Encoding.ASCII.GetString(indraerr.msg).TrimEnd((Char)0);

                Console.WriteLine(err);
                listboxerr.Dispatcher.BeginInvoke((System.Windows.Forms.MethodInvoker)(() =>
                {
                    listboxerr.Items.Add(err);
                }));
            }

            return ret;
        }
    }
}
