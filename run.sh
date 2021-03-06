#!/bin/bash

instances=(zlt_100_2 zlt_250_2 zlt_500_2 zlt_750_2 zlt_100_3 zlt_250_3 zlt_500_3 zlt_750_3 zlt_100_4 zlt_250_4 zlt_500_4 zlt_750_4)
solvers=(nsga2 nspso moead mhaco ihs nsbrkga)
decoder_types=(1)
seeds=(319086417 293920772 121513023 317048708 746557472)
versions=(best median)

declare -A population_size
population_size[zlt_100_2]=152
population_size[zlt_100_3]=200
population_size[zlt_100_4]=252
population_size[zlt_250_2]=152
population_size[zlt_250_3]=200
population_size[zlt_250_4]=252
population_size[zlt_500_2]=200
population_size[zlt_500_3]=252
population_size[zlt_500_4]=300
population_size[zlt_750_2]=252
population_size[zlt_750_3]=300
population_size[zlt_750_4]=352

num_processes=6

time_limit=3600
max_num_solutions=500
max_num_snapshots=60

mkdir -p statistics
mkdir -p solutions
mkdir -p pareto
mkdir -p best_solutions_snapshots
mkdir -p num_non_dominated_snapshots
mkdir -p num_fronts_snapshots
mkdir -p populations_snapshots
mkdir -p num_elites_snapshots
mkdir -p hypervolume
mkdir -p hypervolume_snapshots

commands=()

for ((i=0;i<num_processes;i++))
do
    commands[$i]="("
done

i=0

for instance in ${instances[@]}
do
    for solver in ${solvers[@]}
    do
        for decoder_type in ${decoder_types[@]}
        do
            for seed in ${seeds[@]}
            do
                command="./bin/exec/${solver}_solver_exec "
                command+="--instance instances/${instance}.txt "
                command+="--decoder-type ${decoder_type} "
                command+="--seed ${seed} "
                command+="--time-limit ${time_limit} "
                command+="--max-num-solutions ${max_num_solutions} "
                command+="--max-num-snapshots ${max_num_snapshots} "
                command+="--population-size ${population_size[${instance}]} "
                command+="--statistics statistics/${instance}_${solver}_${decoder_type}_${seed}.txt "
                command+="--solutions solutions/${instance}_${solver}_${decoder_type}_${seed}_ "
                command+="--pareto pareto/${instance}_${solver}_${decoder_type}_${seed}.txt "
                command+="--best-solutions-snapshots best_solutions_snapshots/${instance}_${solver}_${decoder_type}_${seed}_ "
                command+="--num-non-dominated-snapshots num_non_dominated_snapshots/${instance}_${solver}_${decoder_type}_${seed}.txt "
                command+="--num-fronts-snapshots num_fronts_snapshots/${instance}_${solver}_${decoder_type}_${seed}.txt "
                command+="--populations-snapshots populations_snapshots/${instance}_${solver}_${decoder_type}_${seed}_ "
                if [ $solver = "nspso" ]
                then
                    command+="--memory "
                fi
                if [ $solver = "moead" ]
                then
                    command+="--preserve-diversity "
                fi
                if [ $solver = "mhaco" ]
                then
                    command+="--memory "
                fi
                if [ $solver = "nsbrkga" ]
                then
                    command+="--num-elites-snapshots num_elites_snapshots/${instance}_${solver}_${decoder_type}_${seed}.txt "
                fi
                if [ $i -lt $num_processes ]
                then
                    commands[$i]+="$command"
                else
                    commands[$((i%num_processes))]+=" && $command"
                fi
                i=$((i+1))
            done
        done
    done
done

for ((i=0;i<num_processes;i++))
do
    commands[$i]+=") &> log_${i}.txt"
done

final_command=""

for ((i=0;i<num_processes;i++))
do
    command=${commands[$i]}
    final_command+="$command & "
done

eval $final_command

wait

solvers=(nsga2 nspso moead mhaco ihs nsbrkga)

commands=()

for ((i=0;i<num_processes;i++))
do
    commands[$i]="("
done

i=0

for instance in ${instances[@]}
do
    command="./bin/exec/hypervolume_calculator_exec "
    command+="--instance instances/${instance}.txt "
    j=0;
    for solver in ${solvers[@]}
    do
        for decoder_type in ${decoder_types[@]}
        do
            for seed in ${seeds[@]}
            do
                command+="--pareto-${j} pareto/${instance}_${solver}_${decoder_type}_${seed}.txt "
                command+="--best-solutions-snapshots-${j} best_solutions_snapshots/${instance}_${solver}_${decoder_type}_${seed}_ "
                command+="--hypervolume-${j} hypervolume/${instance}_${solver}_${decoder_type}_${seed}.txt "
                command+="--hypervolume-snapshots-${j} hypervolume_snapshots/${instance}_${solver}_${decoder_type}_${seed}.txt "
                j=$((j+1))
            done
        done
    done
    if [ $i -lt $num_processes ]
    then
        commands[$i]+="$command"
    else
        commands[$((i%num_processes))]+=" && $command"
    fi
    i=$((i+1))
done

for ((i=0;i<num_processes;i++))
do
    commands[$i]+=") &>> log_${i}.txt"
done

final_command=""

for ((i=0;i<num_processes;i++))
do
    command=${commands[$i]}
    final_command+="$command & "
done

eval $final_command

wait

commands=()

for ((i=0;i<num_processes;i++))
do
    commands[$i]="("
done

i=0

for instance in ${instances[@]}
do
    for solver in ${solvers[@]}
    do
        for decoder_type in ${decoder_types[@]}
        do
            command="./bin/exec/results_aggregator_exec "
            command+="--hypervolumes hypervolume/${instance}_${solver}_${decoder_type}.txt "
            command+="--hypervolume-statistics hypervolume/${instance}_${solver}_${decoder_type}_stats.txt "
            command+="--statistics-best statistics/${instance}_${solver}_${decoder_type}_best.txt "
            command+="--statistics-median statistics/${instance}_${solver}_${decoder_type}_median.txt "
            command+="--pareto-best pareto/${instance}_${solver}_${decoder_type}_best.txt "
            command+="--pareto-median pareto/${instance}_${solver}_${decoder_type}_median.txt "
            command+="--hypervolume-snapshots-best hypervolume_snapshots/${instance}_${solver}_${decoder_type}_best.txt "
            command+="--hypervolume-snapshots-median hypervolume_snapshots/${instance}_${solver}_${decoder_type}_median.txt "
            command+="--best-solutions-snapshots-best best_solutions_snapshots/${instance}_${solver}_${decoder_type}_best_ "
            command+="--best-solutions-snapshots-median best_solutions_snapshots/${instance}_${solver}_${decoder_type}_median_ "
            command+="--num-non-dominated-snapshots-best num_non_dominated_snapshots/${instance}_${solver}_${decoder_type}_best.txt "
            command+="--num-non-dominated-snapshots-median num_non_dominated_snapshots/${instance}_${solver}_${decoder_type}_median.txt "
            command+="--populations-snapshots-best populations_snapshots/${instance}_${solver}_${decoder_type}_best_ "
            command+="--populations-snapshots-median populations_snapshots/${instance}_${solver}_${decoder_type}_median_ "
            command+="--num-fronts-snapshots-best num_fronts_snapshots/${instance}_${solver}_${decoder_type}_best.txt "
            command+="--num-fronts-snapshots-median num_fronts_snapshots/${instance}_${solver}_${decoder_type}_median.txt "
            if [ $solver = "nsbrkga" ]
            then
                command+="--num-elites-snapshots-best num_elites_snapshots/${instance}_${solver}_${decoder_type}_best.txt "
                command+="--num-elites-snapshots-median num_elites_snapshots/${instance}_${solver}_${decoder_type}_median.txt "
            fi
            j=0;
            for seed in ${seeds[@]}
            do
                command+="--statistics-${j} statistics/${instance}_${solver}_${decoder_type}_${seed}.txt "
                command+="--pareto-${j} pareto/${instance}_${solver}_${decoder_type}_${seed}.txt "
                command+="--hypervolume-${j} hypervolume/${instance}_${solver}_${decoder_type}_${seed}.txt "
                command+="--hypervolume-snapshots-${j} hypervolume_snapshots/${instance}_${solver}_${decoder_type}_${seed}.txt "
                command+="--best-solutions-snapshots-${j} best_solutions_snapshots/${instance}_${solver}_${decoder_type}_${seed}_ "
                command+="--num-non-dominated-snapshots-${j} num_non_dominated_snapshots/${instance}_${solver}_${decoder_type}_${seed}.txt "
                command+="--populations-snapshots-${j} populations_snapshots/${instance}_${solver}_${decoder_type}_${seed}_ "
                command+="--num-fronts-snapshots-${j} num_fronts_snapshots/${instance}_${solver}_${decoder_type}_${seed}.txt "
                if [ $solver = "nsbrkga" ]
                then
                    command+="--num-elites-snapshots-${j} num_elites_snapshots/${instance}_${solver}_${decoder_type}_${seed}.txt "
                fi
                j=$((j+1))
            done
            if [ $i -lt $num_processes ]
            then
                commands[$i]+="$command"
            else
                commands[$((i%num_processes))]+=" && $command"
            fi
            i=$((i+1))
        done
    done
done

for ((i=0;i<num_processes;i++))
do
    commands[$i]+=") &>> log_${i}.txt"
done

final_command=""

for ((i=0;i<num_processes;i++))
do
    command=${commands[$i]}
    final_command+="$command & "
done

eval $final_command

wait

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
