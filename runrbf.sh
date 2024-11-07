#!/bin/sh
PROGRAM=./RbfCon
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
CHROMOSOMES=200
GENERATIONS=200
DATAFILE=$1
SEED=$2
LOCALMETHOD=bfgs
$PROGRAM --trainfile=$DATAPATH/$DATAFILE.train --chromosome_count=$CHROMOSOMES --generations=$GENERATIONS   --testfile=$DATAPATH/$DATAFILE.test --iterations=10 --local_method=$LOCALMETHOD
