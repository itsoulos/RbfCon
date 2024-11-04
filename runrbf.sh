#!/bin/sh
PROGRAM=./RbfCon
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
GENOMES=200
GENERATIONS=500
DATAFILE=$1
SEED=$2
$PROGRAM --trainfile=$DATAPATH/$DATAFILE.train --chromosome_count=$GENOMES --generations=$GENERATIONS   --testfile=$DATAPATH/$DATAFILE.test
