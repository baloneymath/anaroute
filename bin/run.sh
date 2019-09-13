#!/bin/bash

HAO="/home/usr1/haoc"
HAO_WORKSPACE="/home/usr1/haoc/workspace"


./anaroute --tech_lef $HAO_WORKSPACE/packages/tech/tsmc40_vhv.lef \
           --tech_file a \
           --design_type a \
           --design_file a \
           --placement_layout a \
           --iopin a \
           --symnet a \
           --out a
