// ======================================================================
// SimpleSmartIO.v generated from TopDesign.cysch
// 03/06/2016 at 06:11
// This file is auto generated. ANY EDITS YOU MAKE MAY BE LOST WHEN THIS FILE IS REGENERATED!!!
// ======================================================================

`define CYDEV_CHIP_FAMILY_UNKNOWN 0
`define CYDEV_CHIP_MEMBER_UNKNOWN 0
`define CYDEV_CHIP_FAMILY_PSOC3 1
`define CYDEV_CHIP_MEMBER_3A 1
`define CYDEV_CHIP_REVISION_3A_PRODUCTION 3
`define CYDEV_CHIP_REVISION_3A_ES3 3
`define CYDEV_CHIP_REVISION_3A_ES2 1
`define CYDEV_CHIP_REVISION_3A_ES1 0
`define CYDEV_CHIP_FAMILY_PSOC4 2
`define CYDEV_CHIP_MEMBER_4G 2
`define CYDEV_CHIP_REVISION_4G_PRODUCTION 17
`define CYDEV_CHIP_REVISION_4G_ES 17
`define CYDEV_CHIP_REVISION_4G_ES2 33
`define CYDEV_CHIP_MEMBER_4U 3
`define CYDEV_CHIP_REVISION_4U_PRODUCTION 0
`define CYDEV_CHIP_MEMBER_4E 4
`define CYDEV_CHIP_REVISION_4E_PRODUCTION 0
`define CYDEV_CHIP_MEMBER_4O 5
`define CYDEV_CHIP_REVISION_4O_PRODUCTION 0
`define CYDEV_CHIP_MEMBER_4N 6
`define CYDEV_CHIP_REVISION_4N_PRODUCTION 0
`define CYDEV_CHIP_MEMBER_4D 7
`define CYDEV_CHIP_REVISION_4D_PRODUCTION 0
`define CYDEV_CHIP_MEMBER_4J 8
`define CYDEV_CHIP_REVISION_4J_PRODUCTION 0
`define CYDEV_CHIP_MEMBER_4K 9
`define CYDEV_CHIP_REVISION_4K_PRODUCTION 0
`define CYDEV_CHIP_MEMBER_4H 10
`define CYDEV_CHIP_REVISION_4H_PRODUCTION 0
`define CYDEV_CHIP_MEMBER_4A 11
`define CYDEV_CHIP_REVISION_4A_PRODUCTION 17
`define CYDEV_CHIP_REVISION_4A_ES0 17
`define CYDEV_CHIP_MEMBER_4F 12
`define CYDEV_CHIP_REVISION_4F_PRODUCTION 0
`define CYDEV_CHIP_REVISION_4F_PRODUCTION_256K 0
`define CYDEV_CHIP_REVISION_4F_PRODUCTION_256DMA 0
`define CYDEV_CHIP_MEMBER_4F 13
`define CYDEV_CHIP_REVISION_4F_PRODUCTION 0
`define CYDEV_CHIP_REVISION_4F_PRODUCTION_256K 0
`define CYDEV_CHIP_REVISION_4F_PRODUCTION_256DMA 0
`define CYDEV_CHIP_MEMBER_4M 14
`define CYDEV_CHIP_REVISION_4M_PRODUCTION 0
`define CYDEV_CHIP_MEMBER_4L 15
`define CYDEV_CHIP_REVISION_4L_PRODUCTION 0
`define CYDEV_CHIP_MEMBER_4I 16
`define CYDEV_CHIP_REVISION_4I_PRODUCTION 0
`define CYDEV_CHIP_MEMBER_4C 17
`define CYDEV_CHIP_REVISION_4C_PRODUCTION 0
`define CYDEV_CHIP_FAMILY_PSOC5 3
`define CYDEV_CHIP_MEMBER_5B 18
`define CYDEV_CHIP_REVISION_5B_PRODUCTION 0
`define CYDEV_CHIP_REVISION_5B_ES0 0
`define CYDEV_CHIP_MEMBER_5A 19
`define CYDEV_CHIP_REVISION_5A_PRODUCTION 1
`define CYDEV_CHIP_REVISION_5A_ES1 1
`define CYDEV_CHIP_REVISION_5A_ES0 0
`define CYDEV_CHIP_FAMILY_USED 2
`define CYDEV_CHIP_MEMBER_USED 8
`define CYDEV_CHIP_REVISION_USED 0
// Component: cy_virtualmux_v1_0
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "C:\Program Files (x86)\Cypress\PSoC Creator\3.3\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0"
`include "C:\Program Files (x86)\Cypress\PSoC Creator\3.3\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0\cy_virtualmux_v1_0.v"
`else
`define CY_BLK_DIR "C:\Program Files (x86)\Cypress\PSoC Creator\3.3\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0"
`include "C:\Program Files (x86)\Cypress\PSoC Creator\3.3\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0\cy_virtualmux_v1_0.v"
`endif

// SmartIO_v1_0(Clock_select=31, data0_direction=0, data0_src=Undefined, data0_sync=0, data1_direction=0, data1_src=Undefined, data1_sync=0, data2_direction=0, data2_src=Undefined, data2_sync=0, data3_direction=0, data3_src=Undefined, data3_sync=0, data4_direction=1, data4_src=M0S8LCD[0].com[20], data4_sync=0, data5_direction=4, data5_src=Undefined, data5_sync=0, data6_direction=0, data6_src=Undefined, data6_sync=0, data7_direction=0, data7_src=Undefined, data7_sync=0, Fabric_bypass0=1, Fabric_bypass1=1, Fabric_bypass2=1, Fabric_bypass3=1, Fabric_bypass4=0, Fabric_bypass5=0, Fabric_bypass6=1, Fabric_bypass7=1, gpio0_direction=0, gpio0_sync=0, gpio1_direction=0, gpio1_sync=0, gpio2_direction=0, gpio2_sync=0, gpio3_direction=0, gpio3_sync=0, gpio4_direction=4, gpio4_sync=0, gpio5_direction=2, gpio5_sync=0, gpio6_direction=0, gpio6_sync=0, gpio7_direction=0, gpio7_sync=0, HsiomConnection0=Undefined, HsiomConnection1=Undefined, HsiomConnection2=Undefined, HsiomConnection3=Undefined, HsiomConnection4=M0S8LCD[0].com[20], HsiomConnection5=Undefined, HsiomConnection6=Undefined, HsiomConnection7=Undefined, lut0_in0=17, lut0_in1=17, lut0_in2=17, lut0_map=0, lut0_mode=0, lut1_in0=17, lut1_in1=17, lut1_in2=17, lut1_map=0, lut1_mode=0, lut2_in0=17, lut2_in1=17, lut2_in2=17, lut2_map=0, lut2_mode=0, lut3_in0=17, lut3_in1=17, lut3_in2=17, lut3_map=0, lut3_mode=0, lut4_in0=17, lut4_in1=17, lut4_in2=17, lut4_map=0, lut4_mode=0, lut5_in0=8, lut5_in1=8, lut5_in2=8, lut5_map=128, lut5_mode=0, lut6_in0=17, lut6_in1=17, lut6_in2=17, lut6_map=0, lut6_mode=0, lut7_in0=17, lut7_in1=17, lut7_in2=17, lut7_map=0, lut7_mode=0, Target_port=2, UseHldOvr=false, CY_API_CALLBACK_HEADER_INCLUDE=#include "cyapicallbacks.h", CY_COMPONENT_NAME=SmartIO_v1_0, CY_CONTROL_FILE=<:default:>, CY_DATASHEET_FILE=<:default:>, CY_FITTER_NAME=SmartIO, CY_INSTANCE_SHORT_NAME=SmartIO, CY_MAJOR_VERSION=1, CY_MINOR_VERSION=0, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=PSoC Creator  3.3 SP2.3, INSTANCE_NAME=SmartIO, )
module SmartIO_v1_0_0 (
    data4_in,
    gpio3_inout,
    data3_inout,
    gpio3_in,
    data3_in,
    gpio2_inout,
    data2_inout,
    gpio2_in,
    data2_in,
    gpio1_inout,
    data1_inout,
    gpio1_in,
    data1_in,
    gpio0_inout,
    data0_inout,
    gpio0_in,
    data0_in,
    gpio4_in,
    data4_inout,
    gpio4_inout,
    data5_in,
    gpio5_in,
    data5_inout,
    gpio5_inout,
    data6_in,
    gpio6_in,
    data6_inout,
    gpio6_inout,
    data7_in,
    gpio7_in,
    data7_inout,
    gpio7_inout,
    data0_out,
    gpio0_out,
    data1_out,
    gpio1_out,
    data2_out,
    gpio2_out,
    data3_out,
    gpio3_out,
    data4_out,
    gpio4_out,
    data5_out,
    gpio5_out,
    data6_out,
    gpio6_out,
    data7_out,
    gpio7_out,
    clock);
    input       data4_in;
    inout       gpio3_inout;
    inout       data3_inout;
    input       gpio3_in;
    input       data3_in;
    inout       gpio2_inout;
    inout       data2_inout;
    input       gpio2_in;
    input       data2_in;
    inout       gpio1_inout;
    inout       data1_inout;
    input       gpio1_in;
    input       data1_in;
    inout       gpio0_inout;
    inout       data0_inout;
    input       gpio0_in;
    input       data0_in;
    input       gpio4_in;
    inout       data4_inout;
    inout       gpio4_inout;
    input       data5_in;
    input       gpio5_in;
    inout       data5_inout;
    inout       gpio5_inout;
    input       data6_in;
    input       gpio6_in;
    inout       data6_inout;
    inout       gpio6_inout;
    input       data7_in;
    input       gpio7_in;
    inout       data7_inout;
    inout       gpio7_inout;
    output      data0_out;
    output      gpio0_out;
    output      data1_out;
    output      gpio1_out;
    output      data2_out;
    output      gpio2_out;
    output      data3_out;
    output      gpio3_out;
    output      data4_out;
    output      gpio4_out;
    output      data5_out;
    output      gpio5_out;
    output      data6_out;
    output      gpio6_out;
    output      data7_out;
    output      gpio7_out;
    input       clock;


          wire  Net_654;
          wire  Net_766;
          wire  Net_776;
          wire  Net_797;
          wire  Net_798;
          wire  Net_799;
          wire  Net_800;
          wire  Net_801;
          wire  Net_761;
          wire  Net_796;
          wire  Net_794;
          wire  Net_792;
          wire  Net_790;
          wire  Net_788;
          wire  Net_775;
          wire  Net_772;

    cy_m0s8_prgio_v1_0 cy_m0s8_prgio (
        .clock(clock),
        .data0_i(data0_in),
        .data0_o(data0_out),
        .data0_io(data0_inout),
        .data1_i(data1_in),
        .data1_o(data1_out),
        .data1_io(data1_inout),
        .data2_i(data2_in),
        .data2_o(data2_out),
        .data2_io(data2_inout),
        .data3_i(data3_in),
        .data3_o(data3_out),
        .data3_io(data3_inout),
        .data4_i(data4_in),
        .data4_o(data4_out),
        .data4_io(data4_inout),
        .data5_i(data5_in),
        .data5_o(data5_out),
        .data5_io(data5_inout),
        .data6_i(data6_in),
        .data6_o(data6_out),
        .data6_io(data6_inout),
        .data7_i(data7_in),
        .data7_o(data7_out),
        .data7_io(data7_inout),
        .gpio0_i(Net_654),
        .gpio0_o(gpio0_out),
        .gpio0_io(gpio0_inout),
        .gpio1_i(Net_766),
        .gpio1_o(gpio1_out),
        .gpio1_io(gpio1_inout),
        .gpio2_i(Net_776),
        .gpio2_o(gpio2_out),
        .gpio2_io(gpio2_inout),
        .gpio3_i(Net_797),
        .gpio3_o(gpio3_out),
        .gpio3_io(gpio3_inout),
        .gpio4_i(Net_798),
        .gpio4_o(gpio4_out),
        .gpio4_io(gpio4_inout),
        .gpio5_i(Net_799),
        .gpio5_o(gpio5_out),
        .gpio5_io(gpio5_inout),
        .gpio6_i(Net_800),
        .gpio6_o(gpio6_out),
        .gpio6_io(gpio6_inout),
        .gpio7_i(Net_801),
        .gpio7_o(gpio7_out),
        .gpio7_io(gpio7_inout));
    defparam cy_m0s8_prgio.port_id = 2;

	// VirtualMux_0 (cy_virtualmux_v1_0)
	assign Net_654 = gpio0_in;

	// VirtualMux_1 (cy_virtualmux_v1_0)
	assign Net_766 = gpio1_in;

	// VirtualMux_2 (cy_virtualmux_v1_0)
	assign Net_776 = gpio2_in;

	wire [0:0] tmpOE__internalPin_o4_net;
	wire [0:0] tmpFB_0__internalPin_o4_net;
	wire [0:0] tmpIO_0__internalPin_o4_net;
	wire [0:0] tmpINTERRUPT_0__internalPin_o4_net;
	electrical [0:0] tmpSIOVREF__internalPin_o4_net;

	cy_psoc3_pins_v1_10
		#(.id("f4c66737-3803-4460-8bff-ab29c84994ee/e61d2b3d-761f-4f3b-8222-3ed8eedbdb41"),
		  .drive_mode(3'b000),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b1),
		  .input_clk_en(0),
		  .input_sync(1'b0),
		  .input_sync_mode(1'b0),
		  .intr_mode(2'b00),
		  .invert_in_clock(0),
		  .invert_in_clock_en(0),
		  .invert_in_reset(0),
		  .invert_out_clock(0),
		  .invert_out_clock_en(0),
		  .invert_out_reset(0),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .oe_reset(0),
		  .oe_sync(1'b0),
		  .output_clk_en(0),
		  .output_clock_mode(1'b0),
		  .output_conn(1'b1),
		  .output_mode(1'b0),
		  .output_reset(0),
		  .output_sync(1'b0),
		  .pa_in_clock(-1),
		  .pa_in_clock_en(-1),
		  .pa_in_reset(-1),
		  .pa_out_clock(-1),
		  .pa_out_clock_en(-1),
		  .pa_out_reset(-1),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b1),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .sio_hifreq(""),
		  .sio_vohsel(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .use_annotation(1'b0),
		  .vtrip(2'b10),
		  .width(1),
		  .ovt_hyst_trim(1'b0),
		  .ovt_needed(1'b0),
		  .ovt_slew_control(2'b00),
		  .input_buffer_sel(2'b00))
		internalPin_o4
		 (.oe(tmpOE__internalPin_o4_net),
		  .y({gpio4_out}),
		  .fb({tmpFB_0__internalPin_o4_net[0:0]}),
		  .io({tmpIO_0__internalPin_o4_net[0:0]}),
		  .siovref(tmpSIOVREF__internalPin_o4_net),
		  .interrupt({tmpINTERRUPT_0__internalPin_o4_net[0:0]}),
		  .in_clock({1'b0}),
		  .in_clock_en({1'b1}),
		  .in_reset({1'b0}),
		  .out_clock({1'b0}),
		  .out_clock_en({1'b1}),
		  .out_reset({1'b0}));

	assign tmpOE__internalPin_o4_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

	// VirtualMux_3 (cy_virtualmux_v1_0)
	assign Net_797 = gpio3_in;

	// VirtualMux_4 (cy_virtualmux_v1_0)
	assign Net_798 = gpio4_in;

	// VirtualMux_5 (cy_virtualmux_v1_0)
	assign Net_799 = gpio5_in;

	// VirtualMux_6 (cy_virtualmux_v1_0)
	assign Net_800 = gpio6_in;

	// VirtualMux_7 (cy_virtualmux_v1_0)
	assign Net_801 = gpio7_in;



endmodule

// TCPWM_P4_v2_10(Clock_CheckTolerance=true, Clock_desired_freq=12, Clock_desired_freq_unit=6, Clock_divisor=1, Clock_FractDividerDenominator=0, Clock_FractDividerNumerator=0, Clock_FractDividerUsed=false, Clock_is_direct=false, Clock_is_divider=false, Clock_is_freq=true, Clock_minus_tolerance=5, Clock_ph_align_clock_id=, Clock_ph_align_clock_name=, Clock_plus_tolerance=5, Clock_source_clock_id=, Clock_source_clock_name=, PinVisibility_index=false, PinVisibility_kill=false, PinVisibility_phiA=false, PinVisibility_phiB=false, PinVisibility_switch=false, PWMCompare=5000, PWMCompareBuf=65535, PWMCompareSwap=0, PWMCountMode=3, PWMCountPresent=false, PWMDeadTimeCycle=0, PWMInterruptMask=1, PWMKillEvent=0, PWMLinenSignal=0, PWMLineSignal=0, PWMMode=4, PWMPeriod=10000, PWMPeriodBuf=65535, PWMPeriodSwap=0, PWMPrescaler=0, PWMReloadMode=0, PWMReloadPresent=false, PWMRunMode=0, PWMSetAlign=0, PWMStartMode=0, PWMStartPresent=false, PWMStopEvent=0, PWMStopMode=0, PWMStopPresent=false, PWMSwitchMode=0, PWMSwitchPresent=false, QuadAutoStart=true, QuadEncodingModes=0, QuadIndexMode=0, QuadIndexPresent=false, QuadInterruptMask=1, QuadPhiAMode=3, QuadPhiBMode=3, QuadStopMode=0, QuadStopPresent=false, TCCaptureMode=0, TCCapturePresent=false, TCCompare=65535, TCCompareBuf=65535, TCCompareSwap=0, TCCompCapMode=2, TCCountingModes=0, TCCountMode=3, TCCountPresent=false, TCInterruptMask=1, TCPeriod=65535, TCPrescaler=0, TCPWMCapturePresent=false, TCPWMConfig=7, TCPWMCountPresent=false, TCPWMReloadPresent=false, TCPWMStartPresent=false, TCPWMStopPresent=false, TCReloadMode=0, TCReloadPresent=false, TCRunMode=0, TCStartMode=0, TCStartPresent=false, TCStopMode=0, TCStopPresent=false, TermMode_capture=0, TermMode_cc=0, TermMode_clock=0, TermMode_count=0, TermMode_interrupt=0, TermMode_line=0, TermMode_line_n=0, TermMode_ov=0, TermMode_reload=0, TermMode_start=0, TermMode_stop=0, TermMode_un=0, TermModeStates=, TermVisibility_capture=false, TermVisibility_cc=true, TermVisibility_clock=true, TermVisibility_count=false, TermVisibility_interrupt=true, TermVisibility_line=true, TermVisibility_line_n=true, TermVisibility_ov=true, TermVisibility_reload=false, TermVisibility_start=false, TermVisibility_stop=false, TermVisibility_un=true, CY_API_CALLBACK_HEADER_INCLUDE=#include "cyapicallbacks.h", CY_COMPONENT_NAME=TCPWM_P4_v2_10, CY_CONTROL_FILE=<:default:>, CY_DATASHEET_FILE=<:default:>, CY_FITTER_NAME=PWM, CY_INSTANCE_SHORT_NAME=PWM, CY_MAJOR_VERSION=2, CY_MINOR_VERSION=10, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=PSoC Creator  3.3 SP2.3, INSTANCE_NAME=PWM, )
module TCPWM_P4_v2_10_1 (
    stop,
    count,
    reload,
    start,
    capture,
    interrupt,
    ov,
    un,
    cc,
    clock,
    line,
    line_n);
    input       stop;
    input       count;
    input       reload;
    input       start;
    input       capture;
    output      interrupt;
    output      ov;
    output      un;
    output      cc;
    input       clock;
    output      line;
    output      line_n;


          wire  Net_64;
          wire  Net_63;
          wire  Net_85;
          wire  Net_62;
          wire  Net_110;
          wire  Net_95;
          wire  Net_106;
          wire  Net_104;
          wire  Net_109;
          wire  Net_98;
          wire  Net_108;
          wire  Net_101;
          wire  Net_66;
          wire  Net_81;
          wire  Net_75;
          wire  Net_69;
          wire  Net_82;
          wire  Net_72;

    cy_m0s8_tcpwm_v1_0 cy_m0s8_tcpwm_1 (
        .capture(Net_75),
        .underflow(un),
        .overflow(ov),
        .line_out_compl(line_n),
        .line_out(line),
        .compare_match(cc),
        .interrupt(interrupt),
        .count(Net_69),
        .reload(Net_66),
        .stop(Net_82),
        .start(Net_72),
        .clock(Net_81));

	// VMux_reload (cy_virtualmux_v1_0)
	assign Net_66 = reload;

	// VMux_count (cy_virtualmux_v1_0)
	assign Net_69 = count;

	// VMux_start (cy_virtualmux_v1_0)
	assign Net_72 = start;

	// VMux_capture (cy_virtualmux_v1_0)
	assign Net_75 = capture;

	// VMux_stop (cy_virtualmux_v1_0)
	assign Net_82 = stop;

	// VMux_clock (cy_virtualmux_v1_0)
	assign Net_81 = clock;



endmodule

// top
module top ;

          wire  Net_125;
          wire  Net_135;
          wire  Net_134;
          wire  Net_133;
          wire  Net_132;
          wire  Net_131;
          wire  Net_130;
          wire  Net_129;
          wire  Net_128;
          wire  Net_127;
          wire  Net_126;
          wire  Net_281;
          wire  Net_280;
          wire  Net_279;
          wire  Net_278;
          wire  Net_277;
          wire  Net_276;
          wire  Net_275;
          wire  Net_274;
          wire  Net_273;
          wire  Net_272;
          wire  Net_271;
          wire  Net_270;
          wire  Net_269;
          wire  Net_268;
          wire  Net_267;
          wire  Net_266;
          wire  Net_265;
          wire  Net_264;
          wire  Net_263;
          wire  Net_262;
          wire  Net_261;
          wire  Net_260;
          wire  Net_259;
          wire  Net_258;
          wire  Net_257;
          wire  Net_256;
          wire  Net_255;
          wire  Net_254;
          wire  Net_253;
          wire  Net_252;
          wire  Net_251;
          wire  Net_250;
          wire  Net_249;
          wire  Net_248;
          wire  Net_247;
          wire  Net_246;
          wire  Net_245;
          wire  Net_244;
          wire  Net_243;
          wire  Net_242;
          wire  Net_241;
          wire  Net_240;
          wire  Net_239;
          wire  Net_238;
          wire  Net_237;
          wire  Net_236;
          wire  Net_235;
          wire  Net_183;
          wire  Net_156;
          wire  Net_122;

    SmartIO_v1_0_0 SmartIO (
        .clock(1'b0),
        .data0_in(1'b0),
        .gpio0_in(1'b0),
        .data0_inout(Net_238),
        .gpio0_inout(Net_239),
        .data1_in(1'b0),
        .gpio1_in(1'b0),
        .data1_inout(Net_242),
        .gpio1_inout(Net_243),
        .data2_in(1'b0),
        .gpio2_in(1'b0),
        .data2_inout(Net_246),
        .gpio2_inout(Net_247),
        .data3_in(1'b0),
        .gpio3_in(1'b0),
        .data3_inout(Net_250),
        .gpio3_inout(Net_251),
        .data4_in(Net_156),
        .gpio4_in(1'b0),
        .data4_inout(Net_253),
        .gpio4_inout(Net_254),
        .data5_in(1'b0),
        .gpio5_in(1'b0),
        .data5_inout(Net_257),
        .gpio5_inout(Net_258),
        .data6_in(1'b0),
        .gpio6_in(1'b0),
        .data6_inout(Net_261),
        .gpio6_inout(Net_262),
        .data7_in(1'b0),
        .gpio7_in(1'b0),
        .data7_inout(Net_265),
        .gpio7_inout(Net_266),
        .data0_out(Net_267),
        .gpio0_out(Net_268),
        .data1_out(Net_269),
        .gpio1_out(Net_270),
        .data2_out(Net_271),
        .gpio2_out(Net_272),
        .data3_out(Net_273),
        .gpio3_out(Net_274),
        .data4_out(Net_275),
        .gpio4_out(Net_276),
        .data5_out(Net_277),
        .gpio5_out(Net_183),
        .data6_out(Net_278),
        .gpio6_out(Net_279),
        .data7_out(Net_280),
        .gpio7_out(Net_281));

    TCPWM_P4_v2_10_1 PWM (
        .stop(1'b0),
        .reload(1'b0),
        .start(1'b0),
        .count(1'b1),
        .capture(1'b0),
        .interrupt(Net_131),
        .ov(Net_132),
        .un(Net_133),
        .cc(Net_134),
        .line(Net_156),
        .line_n(Net_135),
        .clock(Net_122));

	wire [0:0] tmpOE__BlueLED_net;
	wire [0:0] tmpFB_0__BlueLED_net;
	wire [0:0] tmpIO_0__BlueLED_net;
	wire [0:0] tmpINTERRUPT_0__BlueLED_net;
	electrical [0:0] tmpSIOVREF__BlueLED_net;

	cy_psoc3_pins_v1_10
		#(.id("e851a3b9-efb8-48be-bbb8-b303b216c393"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b1),
		  .input_clk_en(0),
		  .input_sync(1'b0),
		  .input_sync_mode(1'b0),
		  .intr_mode(2'b00),
		  .invert_in_clock(0),
		  .invert_in_clock_en(0),
		  .invert_in_reset(0),
		  .invert_out_clock(0),
		  .invert_out_clock_en(0),
		  .invert_out_reset(0),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .oe_reset(0),
		  .oe_sync(1'b0),
		  .output_clk_en(0),
		  .output_clock_mode(1'b0),
		  .output_conn(1'b1),
		  .output_mode(1'b0),
		  .output_reset(0),
		  .output_sync(1'b0),
		  .pa_in_clock(-1),
		  .pa_in_clock_en(-1),
		  .pa_in_reset(-1),
		  .pa_out_clock(-1),
		  .pa_out_clock_en(-1),
		  .pa_out_reset(-1),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b1),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .sio_hifreq(""),
		  .sio_vohsel(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .use_annotation(1'b0),
		  .vtrip(2'b10),
		  .width(1),
		  .ovt_hyst_trim(1'b0),
		  .ovt_needed(1'b0),
		  .ovt_slew_control(2'b00),
		  .input_buffer_sel(2'b00))
		BlueLED
		 (.oe(tmpOE__BlueLED_net),
		  .y({Net_183}),
		  .fb({tmpFB_0__BlueLED_net[0:0]}),
		  .io({tmpIO_0__BlueLED_net[0:0]}),
		  .siovref(tmpSIOVREF__BlueLED_net),
		  .interrupt({tmpINTERRUPT_0__BlueLED_net[0:0]}),
		  .in_clock({1'b0}),
		  .in_clock_en({1'b1}),
		  .in_reset({1'b0}),
		  .out_clock({1'b0}),
		  .out_clock_en({1'b1}),
		  .out_reset({1'b0}));

	assign tmpOE__BlueLED_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};


	cy_clock_v1_0
		#(.id("80aa1c6d-ccad-41e6-9c7a-c2a83c989856"),
		  .source_clock_id(""),
		  .divisor(0),
		  .period("100000000000"),
		  .is_direct(0),
		  .is_digital(0))
		Clock_1
		 (.clock_out(Net_122));




endmodule

