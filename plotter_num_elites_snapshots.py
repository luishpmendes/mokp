import csv
import matplotlib.pyplot as plt
import os
from plotter_definitions import *

for instance in instances:
    for version in versions:
        plt.figure(figsize = (5, 5))
        plt.title(instance)
        plt.xlabel("Time (s)")
        plt.ylabel("Number of elites")
        for solver in solvers:
            if solver.startswith("nsbrkga"):
                for decoder_type in decoder_types:
                    filename = "num_elites_snapshots/" + instance + "_" + solver + "_" + str(decoder_type) + "_" + version + ".txt"
                    if os.path.exists(filename):
                        x = []
                        y = []
                        with open(filename) as csv_file:
                            data = csv.reader(csv_file, delimiter = " ")
                            for row in data:
                                x.append(float(row[1]))
                                y.append(float(row[2]))
                        plt.plot(x, y, label = solver_labels[solver] + " " + decoder_types_labels[decoder_type])
        plt.xlim(left = 0)
        plt.ylim(bottom = 0)
        plt.legend(loc = 'best')
        plt.savefig("num_elites_snapshots/" + instance + "_" + version + ".png", format = "png")
        plt.close()
