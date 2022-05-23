import csv
import matplotlib.pyplot as plt
import os
from plotter_definitions import *

for instance in instances:
    for version in versions:
        plt.figure(figsize = (5, 5))
        plt.title(instance)
        plt.xlabel("Time (s)")
        plt.ylabel("Non-dominated Solutions")
        for i in range(len(solvers)):
            for j in range(len(decoder_types)):
                filename = "num_non_dominated_snapshots/" + instance + "_" + solvers[i] + "_" + str(decoder_types[j]) + "_" + version + ".txt"
                if os.path.exists(filename):
                    x = []
                    y = []
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = " ")
                        for row in data:
                            x.append(float(row[1]))
                            y.append(float(row[2]))
                    plt.plot(x, y, label = solver_labels[solvers[i]] + " " + decoder_types_labels[decoder_types[j]], color = colors[i * len(decoder_types) + j], marker = (i * len(decoder_types) + j + 3, 2, 0), alpha = 0.80)
        plt.xlim(left = 0)
        plt.ylim(bottom = 0)
        plt.legend(loc = 'best')
        plt.savefig("num_non_dominated_snapshots/" + instance + "_" + version + ".png", format = "png")
        plt.close()
