#!/bin/bash
#================= OPTIONS (s'applique à chaque job du tableau) =========================
#SBATCH --ntasks=30
#SBATCH --cpus-per-task=1
#SBATCH --nodelist=node34
#SBATCH --mem-per-cpu=10000
#SBATCH --output=simulationParis-30repli-MT-fullmap
#========================== TASKS ================================
make clean
make
./replicationSimulation.sh 30 configVaccin configDpt75
