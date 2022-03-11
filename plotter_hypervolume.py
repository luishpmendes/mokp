import csv
import matplotlib.pyplot as plt
import os
import statistics as stats
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
        filename = "hypervolume/" + instances[i] + "_" + solver + ".txt"
        x = []
        with open(filename) as csv_file:
            data = csv.reader(csv_file)
            for row in data:
                x.append(float(row[0]))
        xs.append(x)
    ax.boxplot(xs, labels = [solver_labels[solver] for solver in solvers])
    ax.set_xticklabels([solver_labels[solver] for solver in solvers], fontsize = "small", rotation = 0, rotation_mode = "default")
fig.suptitle("Hypervolume", fontsize = "xx-large")
plt.savefig("hypervolume/hypervolume.png", format = "png")
plt.close(fig)

hypervolume_per_m = {}

for solver in solvers:
    hypervolume_per_m[solver] = {}
    for m in ms:
        hypervolume_per_m[solver][m] = []

for m in ms:
    for instance in instances_per_m[m]:
        for solver in solvers:
            for seed in seeds:
                filename = "hypervolume/" + instance + "_" + solver + "_" + str(seed) + ".txt"
                if os.path.exists(filename):
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = ",")
                        for row in data:
                            hypervolume_per_m[solver][m].append(float(row[0]))
                        csv_file.close()

plt.figure()
plt.title("Hypervolume x Number of Objectives")
plt.xlabel("Number of Objectives")
plt.ylabel("Hypervolume")
plt.xticks(ms)
for i in range(len(solvers)):
    y = []
    for m in ms:
        y.append(stats.mean(hypervolume_per_m[solvers[i]][m]))
    plt.plot(ms, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i])
plt.xlim(left = min(ms), right = max(ms))
plt.ylim(bottom = 0.0, top = 1.0)
plt.legend(loc = 'best')
plt.savefig("hypervolume/hypervolume_per_m.png", format = "png")
plt.close()

hypervolume_per_size = {}

for solver in solvers:
    hypervolume_per_size[solver] = {}
    for size in sizes:
        hypervolume_per_size[solver][size] = []

for size in sizes:
    for instance in instances_per_size[size]:
        for solver in solvers:
            for seed in seeds:
                filename = "hypervolume/" + instance + "_" + solver + "_" + str(seed) + ".txt"
                if os.path.exists(filename):
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = ",")
                        for row in data:
                            hypervolume_per_size[solver][size].append(float(row[0]))
                        csv_file.close()

plt.figure()
plt.title("Hypervolume x Number of Items")
plt.xlabel("Number of Items")
plt.ylabel("Hypervolume")
plt.xticks(sizes)
for i in range(len(solvers)):
    y = []
    for size in sizes:
        y.append(stats.mean(hypervolume_per_size[solvers[i]][size]))
    plt.plot(sizes, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i])
plt.xlim(left = min(sizes), right = max(sizes))
plt.ylim(bottom = 0.0, top = 1.0)
plt.legend(loc = 'best')
plt.savefig("hypervolume/hypervolume_per_size.png", format = "png")
plt.close()
