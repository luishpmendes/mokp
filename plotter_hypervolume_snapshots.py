import csv
import matplotlib.pyplot as plt
import os
import statistics as stats
from math import floor, sqrt
from plotter_definitions import *

for version in versions:
    num_rows = floor(sqrt(len(instances)))
    num_cols = int(len(instances)/floor(sqrt(len(instances))))
    fig = plt.figure(figsize = (5 * num_cols, 5 * num_rows), constrained_layout = True)
    figs = fig.subfigures(nrows = num_rows, ncols = num_cols, wspace = 0.05, hspace = 0.05)
    for i in range(len(instances)):
        row = floor(i/num_cols)
        col = i%num_cols
        figs[row][col].suptitle(instances[i], fontsize = "x-large")
        ax = figs[row][col].subplots()
        ax.set_ylabel("Hypervolume x Time", fontsize = "large")
        ax.set_xlabel("Time (s)", fontsize = "large")
        ax.set_ylim(bottom = 0, top = 1)
        for j in range(len(solvers)):
            for k in range(len(decoder_types)):
                filename = "hypervolume_snapshots/" + instances[i] + "_" + solvers[j] + "_" + str(decoder_types[k]) + "_" + version + ".txt"
                if os.path.exists(filename):
                    x = []
                    y = []
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = ",")
                        for row in data:
                            x.append(float(row[1]))
                            y.append(float(row[2]))
                    ax.plot(x, y, label = solver_labels[solvers[j]] + " " + decoder_types_labels[decoder_types[k]], marker = (j * len(decoder_types) + k + 3, 2, 0), color = colors[j * len(decoder_types) + k], alpha=0.80)
        ax.set_xlim(left = 0)
        ax.legend(loc = 'best')
    fig.suptitle("Hypervolume", fontsize = "xx-large")
    plt.savefig("hypervolume_snapshots/hypervolume_snapshots_" + version + ".png", format = "png")
    plt.savefig("hypervolume_snapshots/hypervolume_snapshots_" + version + ".eps", format = "eps")
    plt.close(fig)

hypervolume_per_solver = {}
time_per_solver = {}

for solver in solvers:
    for decoder_type in decoder_types:
        hypervolume_per_solver[solver + " " + str(decoder_type)] = []
        time_per_solver[solver + " " + str(decoder_type)] = []
        for i in range(num_snapshots + 1):
            hypervolume_per_solver[solver + " " + str(decoder_type)].append([])
            time_per_solver[solver + " " + str(decoder_type)].append([])

for instance in instances:
    for solver in solvers:
        for decoder_type in decoder_types:
            for seed in seeds:
                filename = "hypervolume_snapshots/" + instance + "_" + solver + "_" + str(decoder_type) + "_" + str(seed) + ".txt"
                if os.path.exists(filename):
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = ",")
                        i = 0
                        for row in data:
                            time_per_solver[solver + " " + str(decoder_type)][i].append(float(row[1]))
                            hypervolume_per_solver[solver + " " + str(decoder_type)][i].append(float(row[2]))
                            i += 1
                        csv_file.close()

plt.figure(figsize=(5, 5), constrained_layout = True)
plt.title("Hypervolume x Time")
plt.xlabel("Time (s)")
plt.ylabel("Hypervolume")
max_time = 0;
for i in range(len(solvers)):
    for j in range(len(decoder_types)):
        x = []
        y = []
        for k in range(num_snapshots + 1):
            x.append(stats.mean(time_per_solver[solvers[i] + " " + str(decoder_types[j])][k]))
            y.append(stats.mean(hypervolume_per_solver[solvers[i] + " " + str(decoder_types[j])][k]))
            if max_time < max(time_per_solver[solvers[i] + " " + str(decoder_types[j])][k]):
                max_time = max(time_per_solver[solvers[i] + " " + str(decoder_types[j])][k])
        plt.plot(x, y, label = solver_labels[solvers[i]] + " " + decoder_types_labels[decoder_types[j]], marker = (i * len(decoder_types) + j + 3, 2, 0), color = colors[i * len(decoder_types) + j], alpha=0.80)
plt.xlim(left = 0, right = max_time)
plt.ylim(bottom = 0.0, top = 1.0)
plt.legend(loc = 'best')
plt.savefig("hypervolume_snapshots/hypervolume_snapshots.png", format = "png")
plt.savefig("hypervolume_snapshots/hypervolume_snapshots.eps", format = "eps")
plt.close()
