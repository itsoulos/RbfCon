#!/bin/sh
PROGRAM=./RbfCon
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
CHROMOSOMES=500
GENERATIONS=500
DATAFILE=$1
SEED=$2
LOCALMETHOD=lbfgs
 $PROGRAM --trainfile=$DATAPATH/$DATAFILE.train --chromosome_count=$CHROMOSOMES --generations=$GENERATIONS   --testfile=$DATAPATH/$DATAFILE.test --iterations=10 --local_method=$LOCALMETHOD
