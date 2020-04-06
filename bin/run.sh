#!/bin/bash

HAO="/home/local/eda06/haoc"
HAO_WORKSPACE="$HAO/workspace"
ANAROUTE_DIR="$HAO/projects/anaroute"
BENCH_DIR="/home/local/eda06/haoc/projects/MAGICAL_private/execution/tsmc40/results"
OUT_DIR="./out"
GDB="gdb --args"
#GDB=""

# comp2 magical on grid 5
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/comp2_mag5/comp2.pin \
          #--placement_layout $ANAROUTE_DIR/bench_grid/comp2_mag5/comp2.place.gds \
          #--iopin $ANAROUTE_DIR/bench_grid/comp2_mag5/comp2.iopin \
          #--power $ANAROUTE_DIR/bench_grid/comp2_mag5/comp2.power \
          #--out $OUT_DIR/gds/comp2_mag5.route.gds 
          #--symnet $ANAROUTE_DIR/bench_grid/comp2_mag5/comp2.symnet \

# Core_test_flow on grid
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_hvh_tapeout.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/Core_test_flow_2/Core_test_flow.pin \
          #--placement_layout $ANAROUTE_DIR/bench_grid/Core_test_flow_2/Core_test_flow.place.gds \
          #--iopin $ANAROUTE_DIR/bench_grid/Core_test_flow_2/Core_test_flow.iopin \
          #--power $ANAROUTE_DIR/bench_grid/Core_test_flow_2/Core_test_flow.power \
          #--out $OUT_DIR/gds/Core_test_flow_2.route.gds 
          #--symnet $ANAROUTE_DIR/bench_grid/Core_test_flow_2/Core_test_flow.symnet \

# OTA_XT_MAGICAL_2 on grid
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/OTA_XT_MAGICAL/OTA_XT_MAGICAL_2.pin \
          #--placement_layout $ANAROUTE_DIR/bench_grid/OTA_XT_MAGICAL/OTA_XT_MAGICAL_2.place.gds \
          #--iopin $ANAROUTE_DIR/bench_grid/OTA_XT_MAGICAL/OTA_XT_MAGICAL_2.iopin \
          #--out $OUT_DIR/gds/OTA_XT_MAGICAL_2.route.gds 
          #--symnet $ANAROUTE_DIR/bench_grid/OTA_XT_MAGICAL/OTA_XT_MAGICAL_2.symnet \

# OTA_4 on grid
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/OTA_4/OTA_4.pin \
          #--placement_layout $ANAROUTE_DIR/bench_grid/OTA_4/OTA_4.place.gds \
          #--out $OUT_DIR/gds/OTA_4.route.gds 
          #--symnet $ANAROUTE_DIR/bench_grid/OTA_4/OTA_4.symnet \

# BUFF_hao on grid
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/BUFF_hao/BUFF.gr \
          #--placement_layout $ANAROUTE_DIR/bench_grid/BUFF_hao/BUFF.place.gds \
          #--out $OUT_DIR/gds/BUFF.route.gds 

# CTDSM_TOP_hao on grid
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/CTDSM_TOP_hao/DFCND4BWP_LVT.gr \
          #--placement_layout $ANAROUTE_DIR/bench_grid/CTDSM_TOP_hao/DFCND4BWP_LVT.place.gds \
          #--out $OUT_DIR/gds/DFCND4BWP_LVT.route.gds 

# CTDSM_CORE_NEW on grid
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/CTDSM_CORE_NEW/CTDSM_CORE_NEW.gr \
          #--placement_layout $ANAROUTE_DIR/bench_grid/CTDSM_CORE_NEW/CTDSM_CORE_NEW.place.gds \
          #--out $OUT_DIR/gds/CTDSM_CORE_NEW.route.gds 
          #--symnet $ANAROUTE_DIR/bench_grid/CTDSM_CORE_NEW/CTDSM_CORE_NEW.symnet \

# CTDSM_CORE_NEW_2 on grid
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/CTDSM_MAGICAL_2/CTDSM_CORE_NEW.gr \
          #--placement_layout $ANAROUTE_DIR/bench_grid/CTDSM_MAGICAL_2/CTDSM_CORE_NEW.place.gds \
          #--out $OUT_DIR/gds/CTDSM_CORE_NEW.route.gds

# comp2 on grid power
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_hvh_tapeout.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $BENCH_DIR/COMPARATOR_PRE_AMP_2018_Modify_test_flow/COMPARATOR_PRE_AMP_2018_Modify_test_flow.gr \
          #--placement_layout $BENCH_DIR/COMPARATOR_PRE_AMP_2018_Modify_test_flow/COMPARATOR_PRE_AMP_2018_Modify_test_flow.place.gds \
          #--iopin $BENCH_DIR/Core_test_flow/COMPARATOR_PRE_AMP_2018_Modify_test_flow.iopin \
          #--out $OUT_DIR/gds/COMPARATOR_PRE_AMP_2018_Modify_test_flow.route.gds

# Core_test_flow on grid power
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_hvh_tapeout.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $BENCH_DIR/Core_test_flow/Core_test_flow.gr \
          #--placement_layout $BENCH_DIR/Core_test_flow/Core_test_flow.place.gds \
          #--iopin $BENCH_DIR/Core_test_flow/Core_test_flow.iopin \
          #--out $OUT_DIR/gds/Core_test_flow.route.gds

# 2019_CTDSM_MAGICAL_TAPEOUT on grid power
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $BENCH_DIR/2019_CTDSM_MAGICAL_TAPEOUT/C1_BANK.gr \
          #--placement_layout $BENCH_DIR/2019_CTDSM_MAGICAL_TAPEOUT/C1_BANK.place.gds \
          #--iopin $BENCH_DIR/2019_CTDSM_CORE_NEW_TAPEOUT/C1_BANK.iopin \
          #--out $OUT_DIR/gds/C1_BANK.route.gds

# 2019_CTDSM_MAGICAL_TAPEOUT_wrapper_2 on grid power
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $BENCH_DIR/2019_CTDSM_MAGICAL_wrapper_2/CTDSM_CORE_NEW.gr \
          #--placement_layout $BENCH_DIR/2019_CTDSM_MAGICAL_wrapper_2/CTDSM_CORE_NEW.place.gds \
          #--iopin $BENCH_DIR/2019_CTDSM_CORE_NEW_wrapper_2/CTDSM_CORE_NEW.iopin \
          #--out $OUT_DIR/gds/CTDSM_CORE_NEW_wrapper_2.route.gds

# 2019_CTDSM_MAGICAL_TAPEOUT_wrapper_3 on grid power
$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          --tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          --design_file $BENCH_DIR/2019_CTDSM_MAGICAL_wrapper_3/CTDSM_CORE_NEW_schematic.gr \
          --placement_layout $BENCH_DIR/2019_CTDSM_MAGICAL_wrapper_3/CTDSM_CORE_NEW_schematic.place.gds \
          --iopin $BENCH_DIR/2019_CTDSM_CORE_NEW_wrapper_3/CTDSM_CORE_NEW_schematic.iopin \
          --out $OUT_DIR/gds/CTDSM_CORE_NEW_wrapper_3.route.gds
