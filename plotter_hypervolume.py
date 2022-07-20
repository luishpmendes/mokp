import csv
import matplotlib.pyplot as plt
import os
import statistics as stats
import itertools
from math import floor, sqrt
from plotter_definitions import *

num_rows = floor(sqrt(len(instances)))
num_cols = int(len(instances)/floor(sqrt(len(instances))))
fig = plt.figure(figsize = (5 * num_cols, 5 * num_rows), constrained_layout = True)
figs = fig.subfigures(nrows = num_rows, ncols = num_cols, wspace = 0.05, hspace = 0.05)
for i in range(len(instances)):
    row = floor(i/num_cols)
    col = i%num_cols
    figs[row][col].suptitle(instances[i], fontsize = "x-large")
    ax = figs[row][col].subplots()
    ax.set_ylabel("Hypervolume", fontsize = "large")
    ax.set_xlabel("Solver", fontsize = "large")
    ax.set_ylim(bottom = 0, top = 1)
    xs = []
    for solver in solvers:
        for decoder_type in decoder_types:
            filename = "hypervolume/" + instances[i] + "_" + solver + "_" + str(decoder_type) + ".txt"
            x = []
            with open(filename) as csv_file:
                data = csv.reader(csv_file)
                for row in data:
                    x.append(float(row[0]))
            xs.append(x)
    ax.boxplot(xs, labels = [' '.join(map(str, x)) for x in list(itertools.product([solver_labels[solver] for solver in solvers], [str(decoder_type) for decoder_type in decoder_types]))])
    ax.set_xticklabels([' '.join(map(str, x)) for x in list(itertools.product([solver_labels[solver] for solver in solvers], [str(decoder_type) for decoder_type in decoder_types]))], fontsize = "small", rotation = 0, rotation_mode = "default")
fig.suptitle("Hypervolume", fontsize = "xx-large")
plt.savefig("hypervolume/hypervolume.png", format = "png")
plt.savefig("hypervolume/hypervolume.eps", format = "eps")
plt.close(fig)

hypervolume_per_m = {}

for solver in solvers:
    for decoder_type in decoder_types:
        hypervolume_per_m[solver + " " + str(decoder_type)] = {}
        for m in ms:
            hypervolume_per_m[solver + " " + str(decoder_type)][m] = []

for m in ms:
    for instance in instances_per_m[m]:
        for solver in solvers:
            for decoder_type in decoder_types:
                for seed in seeds:
                    filename = "hypervolume/" + instance + "_" + solver + "_" + str(decoder_type) + "_" + str(seed) + ".txt"
                    if os.path.exists(filename):
                        with open(filename) as csv_file:
                            data = csv.reader(csv_file, delimiter = ",")
                            for row in data:
                                hypervolume_per_m[solver + " " + str(decoder_type)][m].append(float(row[0]))
                            csv_file.close()

plt.figure()
plt.title("Hypervolume x Number of Objectives")
plt.xlabel("Number of Objectives")
plt.ylabel("Hypervolume")
plt.xticks(ms)
for i in range(len(solvers)):
    for j in range(len(decoder_types)):
        y = []
        for m in ms:
            y.append(stats.mean(hypervolume_per_m[solvers[i] + " " + str(decoder_types[j])][m]))
        plt.plot(ms, y, label = solver_labels[solvers[i]] + " " + decoder_types_labels[decoder_types[j]], marker = (i * len(decoder_types) + j + 3, 2, 0), color = colors[i * len(decoder_types) + j], alpha=0.8)
plt.xlim(left = min(ms), right = max(ms))
plt.ylim(bottom = 0.0, top = 1.0)
plt.legend(loc = 'best')
plt.savefig("hypervolume/hypervolume_per_m.png", format = "png")
plt.savefig("hypervolume/hypervolume_per_m.eps", format = "eps")
plt.close()

hypervolume_per_size = {}

for solver in solvers:
    for decoder_type in decoder_types:
        hypervolume_per_size[solver + " " + str(decoder_type)] = {}
        for size in sizes:
            hypervolume_per_size[solver + " " + str(decoder_type)][size] = []

for size in sizes:
    for instance in instances_per_size[size]:
        for solver in solvers:
            for decoder_type in decoder_types:
                for seed in seeds:
                    filename = "hypervolume/" + instance + "_" + solver + "_" + str(decoder_type) + "_" + str(seed) + ".txt"
                    if os.path.exists(filename):
                        with open(filename) as csv_file:
                            data = csv.reader(csv_file, delimiter = ",")
                            for row in data:
                                hypervolume_per_size[solver + " " + str(decoder_type)][size].append(float(row[0]))
                            csv_file.close()

plt.figure()
plt.title("Hypervolume x Number of Items")
plt.xlabel("Number of Items")
plt.ylabel("Hypervolume")
plt.xticks(sizes)
for i in range(len(solvers)):
    for j in range(len(decoder_types)):
        y = []
        for size in sizes:
            y.append(stats.mean(hypervolume_per_size[solvers[i] + " " + str(decoder_types[j])][size]))
        plt.plot(sizes, y, label = solver_labels[solvers[i]] + " " + decoder_types_labels[decoder_types[j]], marker = (i * len(decoder_types) + j + 3, 2, 0), color = colors[i * len(decoder_types) + j], alpha=0.8)
plt.xlim(left = min(sizes), right = max(sizes))
plt.ylim(bottom = 0.0, top = 1.0)
plt.legend(loc = 'best')
plt.savefig("hypervolume/hypervolume_per_size.png", format = "png")
plt.savefig("hypervolume/hypervolume_per_size.eps", format = "eps")
plt.close()
