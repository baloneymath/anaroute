#!/bin/bash

HAO="/home/local/eda06/haoc"
HAO_WORKSPACE="$HAO/workspace"
ANAROUTE_DIR="$HAO/projects/anaroute"
BENCH_DIR="/home/local/eda06/haoc/projects/anaroute/bench_grid"
OUT_DIR="./out"
#GDB="gdb --args"
GDB=""

#$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_hvh_tapeout.lef \
          #--tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          #--design_file $BENCH_DIR/temp/SAR_ADC.gr \
          #--placement_layout $BENCH_DIR/temp/SAR_ADC.place.gds \
          #--out $OUT_DIR/gds/SAR_ADC.route.gds

$GDB ./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_hvh_tapeout.lef \
          --tech_file $HAO_WORKSPACE/packages/tech/tsmc40_techfile \
          --design_file $BENCH_DIR/temp/comp.gr \
          --placement_layout $BENCH_DIR/temp/comp.place.gds \
          --out $OUT_DIR/gds/comp.route.gds
