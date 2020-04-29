#!/bin/bash

HAO="/home/local/eda06/haoc"
HAO_WORKSPACE="$HAO/workspace"
ANAROUTE_DIR="$HAO/projects/anaroute"
BENCH_DIR="/home/local/eda06/haoc/projects/anaroute/bench_iccad20/"
OUT_DIR="./out"
#GDB="gdb --args"
GDB=""

#comp2 on grid power
$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_hvh_tapeout.lef \
          --tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          --design_file $BENCH_DIR/comp/COMPARATOR_PRE_AMP_2018_Modify_test_flow.gr \
          --placement_layout $BENCH_DIR/comp/COMPARATOR_PRE_AMP_2018_Modify_test_flow.place.gds \
          --iopin $BENCH_DIR/comp/COMPARATOR_PRE_AMP_2018_Modify_test_flow.iopin \
          --out $OUT_DIR/gds/comp.route.gds

# Core_test_flow on grid power
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_hvh_tapeout.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $BENCH_DIR/Core_test_flow/Core_test_flow.gr \
          #--placement_layout $BENCH_DIR/Core_test_flow/Core_test_flow.place.gds \
          #--iopin $BENCH_DIR/Core_test_flow/Core_test_flow.iopin \
          #--spec $BENCH_DIR/Core_test_flow/Core_test_flow.gr.spec \
          #--out $OUT_DIR/gds/Core_test_flow.route.gds

# OTA_XT_MAGICAL on grid power
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_hvh_tapeout.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $BENCH_DIR/OTA_XT/OTA_XT_MAGICAL_2.gr \
          #--placement_layout $BENCH_DIR/OTA_XT/OTA_XT_MAGICAL_2.place.gds \
          #--iopin $BENCH_DIR/OTA_XT/OTA_XT_MAGICAL_2.iopin \
          #--spec $BENCH_DIR/OTA_XT/OTA_XT_MAGICAL_2.gr.spec \
          #--out $OUT_DIR/gds/OTA_XT_MAGICAL.route.gds

# Telescopic on grid power
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_hvh_tapeout.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $BENCH_DIR/Telescopic/Telescopic_Three_stage_flow.gr \
          #--placement_layout $BENCH_DIR/Telescopic/Telescopic_Three_stage_flow.place.gds \
          #--iopin $BENCH_DIR/Telescopic/Telescopic_Three_stage_flow.iopin \
          #--spec $BENCH_DIR/Telescopic/Telescopic_Three_stage_flow.gr.spec \
          #--out $OUT_DIR/gds/Telescopic_Three_stage_flow.route.gds
