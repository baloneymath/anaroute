#!/bin/bash

HAO="/home/local/eda06/haoc"
HAO_WORKSPACE="$HAO/workspace"
ANAROUTE_DIR="$HAO/projects/anaroute"
BENCH_DIR="../bench_new"
OUT_DIR="./out"
GDB="gdb --args"
#GDB=""

# Comparator
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/COMPARATOR_PRE_AMP_2018_Modify_test_flow/ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/COMPARATOR_PRE_AMP_2018_Modify_test_flow/COMPARATOR_PRE_AMP_2018_Modify_test_flow.wellgen.gds \
           #--symnet $BENCH_DIR/COMPARATOR_PRE_AMP_2018_Modify_test_flow/COMPARATOR_PRE_AMP_2018_Modify_test_flow.symnet \
           #--iopin $BENCH_DIR/COMPARATOR_PRE_AMP_2018_Modify_test_flow/COMPARATOR_PRE_AMP_2018_Modify_test_flow.iopin \
           #--out $OUT_DIR/gds/COMPARATOR_PRE_AMP_2018_Modify_test_flow.route.gds \
           #--out_guide $OUT_DIR/guide/COMPARATOR_PRE_AMP_2018_Modify_test_flow.guide

# OTA_FF_2s_v3_org
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/OTA_FF_2s_v3_org/ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/OTA_FF_2s_v3_org/OTA_FF_2s_v3_org.wellgen.gds \
           #--symnet $BENCH_DIR/OTA_FF_2s_v3_org/OTA_FF_2s_v3_org.symnet \
           #--iopin $BENCH_DIR/OTA_FF_2s_v3_org/OTA_FF_2s_v3_org.iopin \
           #--out $OUT_DIR/gds/OTA_FF_2s_v3_org.route.gds \
           #--out_guide $OUT_DIR/guide/OTA_FF_2s_v3_org.guide \


# Telescopic_Three_stage_flow
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/Telescopic_Three_stage_flow/ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/Telescopic_Three_stage_flow/Telescopic_Three_stage_flow.wellgen.gds  \
           #--iopin $BENCH_DIR/Telescopic_Three_stage_flow/Telescopic_Three_stage_flow.iopin \
           #--symnet $BENCH_DIR/Telescopic_Three_stage_flow/Telescopic_Three_stage_flow.symnet \
           #--out $OUT_DIR/gds/Telescopic_Three_stage_flow.gds \
           #--out_guide $OUT_DIR/guide/Telescopic_Three_stage_flow.guide \

# Telescopic_Three_stage_flow_2
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/Telescopic_Three_stage_flow_2/Telescopic_Three_stage_flow.gr \
           #--placement_layout $BENCH_DIR/Telescopic_Three_stage_flow_2/Telescopic_Three_stage_flow.place.gds  \
           #--iopin $BENCH_DIR/Telescopic_Three_stage_flow_2/Telescopic_Three_stage_flow.iopin \
           #--symnet $BENCH_DIR/Telescopic_Three_stage_flow_2/Telescopic_Three_stage_flow.symnet \
           #--out $OUT_DIR/gds/Telescopic_Three_stage_flow_2.gds \
           #--out_guide $OUT_DIR/guide/Telescopic_Three_stage_flow_2.guide \

# Core_test_flow
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/Core_test_flow/ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/Core_test_flow/Core_test_flow.wellgen.gds  \
           #--symnet $BENCH_DIR/Core_test_flow/Core_test_flow.symnet \
           #--out $OUT_DIR/gds/Core_test_flow.gds \
           #--iopin $BENCH_DIR/Core_test_flow/Core_test_flow.iopin \
           #--out_guide $OUT_DIR/guide/Core_test_flow.guide \

# Core_hao
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/Core_hao/ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/Core_hao/Core_test_flow.wellgen.gds  \
           #--symnet $BENCH_DIR/Core_hao/Core_test_flow.symnet \
           #--out $OUT_DIR/gds/Core_test_flow.gds \
           #--iopin $BENCH_DIR/Core_test_flow/Core_test_flow.iopin \
           #--out_guide $OUT_DIR/guide/Core_test_flow.guide \

# OTA_4
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/OTA_4/ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/OTA_4/OTA_4.wellgen.gds  \
           #--symnet $BENCH_DIR/OTA_4/OTA_4.symnet \
           #--out $OUT_DIR/gds/OTA_4.gds \
           #--iopin $BENCH_DIR/OTA_4/OTA_4.iopin \
           #--out_guide $OUT_DIR/guide/OTA_4.guide \

# SW_VCM
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/SW_VCM/ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/SW_VCM/SW_VCM.wellgen.gds  \
           #--symnet $BENCH_DIR/SW_VCM/SW_VCM.symnet \
           #--out $OUT_DIR/gds/SW_VCM.gds \
           #--iopin $BENCH_DIR/SW_VCM/SW_VCM.iopin \
           #--out_guide $OUT_DIR/guide/SW_VCM.guide \

# SR_Latch_LVT
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/SR_Latch_LVT/SR_Latch_LVT.ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/SR_Latch_LVT/SR_Latch_LVT.wellgen.gds  \
           #--symnet $BENCH_DIR/SR_Latch_LVT/SR_Latch_LVT.symnet \
           #--out $OUT_DIR/gds/SR_Latch_LVT.gds \
           #--out_guide $OUT_DIR/guide/SR_Latch_LVT.guide \

# Comparator
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/COMPARATOR/COMPARATOR_schematic.ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/COMPARATOR/COMPARATOR_schematic.wellgen.gds \
           #--symnet $BENCH_DIR/COMPARATOR/COMPARATOR_schematic.symnet \
           #--out $OUT_DIR/gds/COMPARATOR.route.gds \
           #--out_guide $OUT_DIR/guide/COMPARATOR.guide

# CTDSM
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/CTDSM_TOP_hao/CTDSM_TOP.ROUTING_INPUT.gr \
           #--placement_layout $BENCH_DIR/CTDSM_TOP_hao/CTDSM_TOP.wellgen.gds \
           #--symnet $BENCH_DIR/CTDSM_TOP_hao/CTDSM_TOP.symnet \
           #--out $OUT_DIR/gds/CTDSM_TOP.route.gds \
           #--out_guide $OUT_DIR/guide/CTDSM_TOP.guide

# CTDSM2
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/CTDSM_TOP_hao_2/COMPARATOR_schematic.gr \
           #--placement_layout $BENCH_DIR/CTDSM_TOP_hao_2/COMPARATOR_schematic.place.gds \
           #--symnet $BENCH_DIR/CTDSM_TOP_hao_2/COMPARATOR_schematic.symnet \
           #--out $OUT_DIR/gds/CTDSM_TOP_2.route.gds \
           #--out_guide $OUT_DIR/guide/CTDSM_TOP_2.guide

# CTDSM3
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/CTDSM_TOP_hao_3/BUFFD4BWP_LVT_schematic.gr \
           #--placement_layout $BENCH_DIR/CTDSM_TOP_hao_3/BUFFD4BWP_LVT_schematic.place.gds \
           #--symnet $BENCH_DIR/CTDSM_TOP_hao_3/BUFFD4BWP_LVT_schematic.symnet \
           #--out $OUT_DIR/gds/CTDSM_TOP_3.route.gds \
           #--out_guide $OUT_DIR/guide/CTDSM_TOP_3.guide

# CTDSM4
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/CTDSM_TOP_hao_4/CTDSM_TOP.gr \
           #--placement_layout $BENCH_DIR/CTDSM_TOP_hao_4/CTDSM_TOP.place.gds \
           #--symnet $BENCH_DIR/CTDSM_TOP_hao_4/CTDSM_TOP.symnet \
           #--out $OUT_DIR/gds/CTDSM_TOP_4.route.gds \
           #--out_guide $OUT_DIR/guide/CTDSM_TOP_4.guide

# COMPARATOR_TEST_FLOW
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $BENCH_DIR/COMPARATOR_PRE_AMP_test_flow/COMPARATOR_PRE_AMP.gr \
           #--placement_layout $BENCH_DIR/COMPARATOR_PRE_AMP_test_flow/COMPARATOR_PRE_AMP.place.gds \
           #--symnet $BENCH_DIR/COMPARATOR_PRE_AMP_test_flow/COMPARATOR_PRE_AMP.symnet \
           #--out $OUT_DIR/gds/COMPARATOR_PRE_AMP.route.gds \
           #--out_guide $OUT_DIR/guide/COMPARATOR_PRE_AMP.guide

# comp2 manual
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
           #--design_file $ANAROUTE_DIR/bench_grid/comp2/comp2_grid_v1.pin \
           #--placement_layout $ANAROUTE_DIR/bench_grid/comp2/comp2_grid_v1.place.gds \
           #--symnet $ANAROUTE_DIR/bench_grid/comp2/comp2.symnet \
           #--out $OUT_DIR/gds/comp2.route.gds 

# comp2 magical on grid
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/comp2_mag/comp2.pin \
          #--placement_layout $ANAROUTE_DIR/bench_grid/comp2_mag/comp2.place.gds \
          #--symnet $ANAROUTE_DIR/bench_grid/comp2_mag/comp2.symnet \
          #--out $OUT_DIR/gds/comp2_mag.route.gds 

# comp2 magical on grid 2
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/comp2_mag2/comp2.pin \
          #--placement_layout $ANAROUTE_DIR/bench_grid/comp2_mag2/comp2.place.gds \
          #--symnet $ANAROUTE_DIR/bench_grid/comp2_mag2/comp2.symnet \
          #--out $OUT_DIR/gds/comp2_mag2.route.gds 

# comp2 magical on grid 3
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/comp2_mag3/comp2_mag3.pin \
          #--placement_layout $ANAROUTE_DIR/bench_grid/comp2_mag3/comp2_mag3.place.gds \
          #--symnet $ANAROUTE_DIR/bench_grid/comp2_mag3/comp2.symnet \
          #--out $OUT_DIR/gds/comp2_mag3.route.gds 

# comp2 magical on grid 4
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/comp2_mag4/comp2_mag4.pin \
          #--placement_layout $ANAROUTE_DIR/bench_grid/comp2_mag4/comp2_mag4.place.gds \
          #--symnet $ANAROUTE_DIR/bench_grid/comp2_mag4/comp2.symnet \
          #--out $OUT_DIR/gds/comp2_mag4.route.gds 

# Core_test_flow on grid
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/Core_test_flow/Core_test_flow.pin \
          #--placement_layout $ANAROUTE_DIR/bench_grid/Core_test_flow/Core_test_flow.place.gds \
          #--symnet $ANAROUTE_DIR/bench_grid/Core_test_flow/Core_test_flow.symnet \
          #--out $OUT_DIR/gds/Core_test_flow.route.gds 

# comp2 magical on grid 5
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/comp2_mag5/comp2.pin \
          #--placement_layout $ANAROUTE_DIR/bench_grid/comp2_mag5/comp2.place.gds \
          #--symnet $ANAROUTE_DIR/bench_grid/comp2_mag5/comp2.symnet \
          #--iopin $ANAROUTE_DIR/bench_grid/com2_mag5/comp2.iopin \
          #--out $OUT_DIR/gds/comp2_mag5.route.gds 

# Core_test_flow on grid
$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          --tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          --design_file $ANAROUTE_DIR/bench_grid/Core_test_flow_2/Core_test_flow.pin \
          --placement_layout $ANAROUTE_DIR/bench_grid/Core_test_flow_2/Core_test_flow.place.gds \
          --symnet $ANAROUTE_DIR/bench_grid/Core_test_flow_2/Core_test_flow.symnet \
          --iopin $ANAROUTE_DIR/bench_grid/Core_test_flow_2/Core_test_flow.iopin \
          --out $OUT_DIR/gds/Core_test_flow_2.route.gds 

# OTA_XT_MAGICAL_2 on grid
#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $ANAROUTE_DIR/bench_grid/OTA_XT_MAGICAL/OTA_XT_MAGICAL_2.pin \
          #--placement_layout $ANAROUTE_DIR/bench_grid/OTA_XT_MAGICAL/OTA_XT_MAGICAL_2.place.gds \
          #--symnet $ANAROUTE_DIR/bench_grid/OTA_XT_MAGICAL/OTA_XT_MAGICAL_2.symnet \
          #--out $OUT_DIR/gds/OTA_XT_MAGICAL_2.route.gds 
