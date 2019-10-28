#!/bin/bash

HAO="/home/local/eda06/haoc"
HAO_WORKSPACE="$HAO/workspace"
ANAROUTE_DIR="$HAO/projects/anaroute"
BENCH_DIR="../bench"
OUT_DIR="./out"
GDB="gdb --args"
#GDB=""

# Comparator
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_type a \
           #--design_file $BENCH_DIR/magical/COMPARATOR_PRE_AMP_2018_Modify_test_flow/ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/magical/COMPARATOR_PRE_AMP_2018_Modify_test_flow/COMPARATOR_PRE_AMP_2018_Modify_test_flow.wellgen.gds \
           #--iopin a \
           #--symnet $BENCH_DIR/magical/COMPARATOR_PRE_AMP_2018_Modify_test_flow/COMPARATOR_PRE_AMP_2018_Modify_test_flow.symnet \
           #--out a \
           #--out_guide $OUT_DIR/guide/COMPARATOR_PRE_AMP_2018_Modify_test_flow.guide

# OTA_FF_2s_v3_nodummy
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_type a \
           #--design_file $BENCH_DIR/magical/OTA_FF_2s_v3_nodummy/ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/magical/OTA_FF_2s_v3_nodummy/OTA_FF_2s_v3_nodummy.wellgen.gds \
           #--iopin a \
           #--symnet $BENCH_DIR/magical/OTA_FF_2s_v3_nodummy/OTA_FF_2s_v3_nodummy.symnet \
           #--out a \
           #--out_guide $OUT_DIR/guide/OTA_FF_2s_v3_nodummy.guide \
           #--out_guide_gds $OUT_DIR/guide/OTA_FF_2s_v3_nodummy.guide.gds


# Telescopic_Three_stage_flow
$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           --tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           --design_type a \
           --design_file $BENCH_DIR/Telescopic_Three_stage_flow/ROUTING_INPUT.gr \
           --placement_layout $BENCH_DIR/Telescopic_Three_stage_flow/Telescopic_Three_stage_flow.wellgen.gds  \
           --iopin a \
           --symnet $BENCH_DIR/Telescopic_Three_stage_flow/Telescopic_Three_stage_flow.symnet \
           --out $OUT_DIR/gds/Telescopic_Three_stage_flow.gds \
           --out_guide $OUT_DIR/guide/Telescopic_Three_stage_flow.guide \
           --out_guide_gds $OUT_DIR/guide/Telescopic_Three_stage_flow.guide.gds \
