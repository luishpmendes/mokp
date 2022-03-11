import csv
import matplotlib.pyplot as plt
import os
import seaborn as sns
from plotter_definitions import *

for m in ms:
    for instance in instances_per_m[m]:
        for version in versions:
            print("m = " + str(m))
            print("instance = " + instance)
            print("version = " + version)
            min_ys = []
            max_ys = []
            for j in range(m):
                min_ys.append(-1)
                max_ys.append(-1)
            for i in range(len(solvers)):
                filename = "pareto/" + instance + "_" + solvers[i] + "_" + version + ".txt"
                if os.path.exists(filename):
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter=" ")
                        for row in data:
                            for j in range(m):
                                if min_ys[j] == -1 or min_ys[j] > float(row[j]):
                                    min_ys[j] = float(row[j])
                                if max_ys[j] == -1 or max_ys[j] < float(row[j]):
                                    max_ys[j] = float(row[j])
                        csv_file.close()
            for j in range(m):
                delta_y = max_ys[j] - min_ys[j]
                min_ys[j] = min_ys[j] - round(0.025 * delta_y)
                max_ys[j] = max_ys[j] + round(0.025 * delta_y)
            fig, axs = plt.subplots(nrows = m, ncols = m, figsize = (5.0 * m, 5.0 * m), squeeze = False, num = 1, clear = True)
            fig.set_size_inches(5.0 * m, 5.0 * m)
            fig.suptitle(instance, fontsize = "xx-large")
            for i in range(len(solvers)):
                filename = "pareto/" + instance + "_" + solvers[i] + "_" + version + ".txt"
                if os.path.exists(filename):
                    ys = []
                    for j in range(m):
                        ys.append([])
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = " ")
                        for row in data:
                            for j in range(m):
                                ys[j].append(float(row[j]))
                        csv_file.close()
                    for j in range(m):
                        for k in range(m):
                            if j == k:
                                axs[j][k].set_xlim(left = min_ys[k], right = max_ys[k])
                                axs[j][k].set_xlabel(xlabel = "$f_{" + str(k + 1) + "}$", fontsize = "x-large")
                                axs[j][j].set_yticks([])
                                axs[j][k].set_ylabel(ylabel = "Density", fontsize = "x-large")
                                sns.kdeplot(data = ys[k], ax = axs[j][k], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), alpha = 0.80)
                                axs[j][k].legend(loc = "best")
                            else:
                                axs[j][k].set_xlim(left = min_ys[k], right = max_ys[k])
                                axs[j][k].set_ylim(bottom = min_ys[j], top = max_ys[j])
                                axs[j][k].set_xlabel(xlabel = "$f_{" + str(k + 1) + "}$", fontsize = "x-large")
                                axs[j][k].set_ylabel(ylabel = "$f_{" + str(j + 1) + "}$", fontsize = "x-large")
                                axs[j][k].scatter(x = ys[k], y = ys[j], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), alpha = 0.75)
                                axs[j][k].legend(loc = "best")
                    del ys
            plt.subplots_adjust(wspace = 0.15 + 0.05 * m, hspace = 0.15 + 0.05 * m)
            plt.savefig("pareto/" + instance + "_" + version + ".png", format = "png")
