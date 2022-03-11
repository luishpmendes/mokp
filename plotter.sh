#!/bin/bash

instances=(zlt_100_2 zlt_250_2 zlt_500_2 zlt_750_2 zlt_100_3 zlt_250_3 zlt_500_3 zlt_750_3 zlt_100_4 zlt_250_4 zlt_500_4 zlt_750_4)
versions=(best median)
solvers=(nsga2 nspso moead mhaco ihs nsbrkga)

python3 plotter_hypervolume.py &
python3 plotter_hypervolume_snapshots.py &
python3 plotter_num_non_dominated_snapshots.py &
python3 plotter_num_fronts_snapshots.py &
python3 plotter_num_elites_snapshots.py &
python3 plotter_pareto.py &
python3 plotter_best_solutions_snapshots.py &
python3 plotter_populations_snapshots.py

wait

for instance in ${instances[@]}
do
    for version in ${versions[@]}
    do
        echo "Instance ${instance}"
        echo "Version ${version}"

        ffmpeg -y -r 5 -i best_solutions_snapshots/${instance}_${version}_%d.png -c:v libx264 -vf fps=60 -pix_fmt yuv420p best_solutions_snapshots/${instance}_${version}.mp4 &
        ffmpeg -y -r 5 -i populations_snapshots/${instance}_${version}_%d.png -c:v libx264 -vf fps=60 -pix_fmt yuv420p populations_snapshots/${instance}_${version}.mp4

        wait

        rm best_solutions_snapshots/${instance}_${version}_*.png &
        rm populations_snapshots/${instance}_${version}_*.png

        wait
    done
done
