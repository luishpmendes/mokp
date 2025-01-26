import csv
from functools import partial
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter
import numpy as np
import seaborn as sns
import ptitprince as pt
import os
import statistics as stats
from plotter_definitions import *

dirname = os.path.dirname(__file__)

min_hypervolume = 1.0
max_hypervolume = 0.0
for instance in instances:
    for solver in solvers:
        filename = os.path.join(dirname, "hypervolume/" + instance + "_" + solver + ".txt")
        with open(filename) as csv_file:
            data = csv.reader(csv_file)
            for row in data:
                min_hypervolume = min(min_hypervolume, float(row[0]))
                max_hypervolume = max(max_hypervolume, float(row[0]))
delta_hypervolume = max_hypervolume - min_hypervolume
min_hypervolume = max(min_hypervolume - round(0.025 * delta_hypervolume), 0.00)
max_hypervolume = min(max_hypervolume + round(0.025 * delta_hypervolume), 1.00)

for instance in instances:
    plt.figure(figsize = (11, 11))
    plt.title(instance, fontsize = "xx-large")
    plt.xlabel("Hypervolume Ratio", fontsize = "x-large")
    xs = []
    for solver in solvers:
        filename = os.path.join(dirname, "hypervolume/" + instance + "_" + solver + ".txt")
        x = []
        with open(filename) as csv_file:
            data = csv.reader(csv_file)
            for row in data:
                x.append(float(row[0]))
        xs.append(x)
    pt.half_violinplot(data = xs, palette = colors, orient = "h", width = 0.6, cut = 0.0, inner = None)
    sns.stripplot(data = xs, palette = colors, orient = "h", size = 2, zorder = 0)
    sns.boxplot(data = xs, orient = "h", width = 0.20, color = "black", zorder = 10, showcaps = True, boxprops = {'facecolor' : 'none', "zorder" : 10}, showfliers = True, whiskerprops = {'linewidth' : 2, "zorder" : 10}, flierprops = {'markersize' : 2})
    plt.yticks(ticks = list(range(len(solvers))), labels = [solver_labels[solver] for solver in solvers], fontsize = "large")
    filename = os.path.join(dirname, "hypervolume/" + instance + ".png")
    plt.savefig(filename, format = "png")
    plt.close()

hypervolume = []

for solver in solvers:
    hypervolume.append([])

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "hypervolume/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        hypervolume[i].append(float(row[0]))
                    csv_file.close()

plt.figure()
plt.xlabel(fontsize="large", xlabel="Hypervolume Ratio")
plt.tick_params(axis="x", which="both", labelsize="large")
plt.grid(alpha=0.5, color="gray", linestyle="dashed", linewidth=0.5, which="both")
pt.half_violinplot(data = hypervolume, palette = colors, orient = "h", width = 0.6, cut = 0.0, inner = None)
sns.stripplot(data = hypervolume, palette = colors, orient = "h", size = 2, zorder = 0)
sns.boxplot(data = hypervolume, orient = "h", width = 0.20, color = "black", zorder = 10, showcaps = True, boxprops = {'facecolor' : 'none', "zorder" : 10}, showfliers = True, whiskerprops = {'linewidth' : 2, "zorder" : 10}, flierprops = {'markersize' : 2}, saturation = 1)
plt.yticks(fontsize="large", ticks=list(range(len(solvers))), labels=[solver_labels[solver] for solver in solvers])
plt.tight_layout()
filename = os.path.join(dirname, "hypervolume/hypervolume.png")
plt.savefig(bbox_inches='tight', fname=filename, format="png")
filename = os.path.join(dirname, "hypervolume/hypervolume.pdf")
plt.savefig(bbox_inches='tight', fname=filename, format="pdf")
plt.close()

hypervolume_per_m = {}

for solver in solvers:
    hypervolume_per_m[solver] = {}
    for m in ms:
        hypervolume_per_m[solver][m] = []

for m in ms:
    for instance in instances_per_m[m]:
        for solver in solvers:
            for seed in seeds:
                filename = os.path.join(dirname, "hypervolume/" + instance + "_" + solver + "_" + str(seed) + ".txt")
                if os.path.exists(filename):
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = ",")
                        for row in data:
                            hypervolume_per_m[solver][m].append(float(row[0]))
                        csv_file.close()

plt.figure()
plt.xlabel(fontsize="large", xlabel="Number of Knapsacks")
plt.ylabel(fontsize="large", ylabel="Hypervolume Ratio")
plt.tick_params(axis="both", which="both", labelsize="large")
plt.xticks(fontsize="large", ticks=ms)
plt.grid(alpha=0.5, color='gray', linestyle='dashed', linewidth=0.5, which='both')
for i in range(len(solvers)):
    y = []
    for m in ms:
        y.append(stats.mean(hypervolume_per_m[solvers[i]][m]))
    plt.plot(ms, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.80)
plt.yscale("function", functions=(partial(np.power, 10.0), np.log10))
plt.legend(fontsize="large", loc="lower left")
plt.gca().xaxis.set_major_formatter(FormatStrFormatter('%d'))
plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.2f'))
plt.gca().yaxis.set_minor_formatter(FormatStrFormatter('%.2f'))
plt.tight_layout()
filename = os.path.join(dirname, "hypervolume/hypervolume_mean_per_m.png")
plt.savefig(bbox_inches='tight', fname=filename, format="png")
filename = os.path.join(dirname, "hypervolume/hypervolume_mean_per_m.pdf")
plt.savefig(bbox_inches='tight', fname=filename, format="pdf")
plt.close()

plt.figure()
plt.xlabel(fontsize="large", xlabel="Number of Knapsacks")
plt.ylabel(fontsize="large", ylabel="Hypervolume Ratio")
plt.tick_params(axis="both", which="both", labelsize="large")
plt.xticks(fontsize="large", ticks=ms)
plt.grid(alpha=0.5, color='gray', linestyle='dashed', linewidth=0.5, which='both')
for i in range(len(solvers)):
    y0 = []
    y2 = []
    for m in ms:
        quantiles = stats.quantiles(hypervolume_per_m[solvers[i]][m])
        y0.append(quantiles[0])
        y2.append(quantiles[2])
    plt.fill_between(ms, y0, y2, color = colors[i], alpha = 0.25)
for i in range(len(solvers)):
    y1 = []
    for m in ms:
        quantiles = stats.quantiles(hypervolume_per_m[solvers[i]][m])
        y1.append(quantiles[1])
    plt.plot(ms, y1, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.75)
plt.yscale("function", functions=(partial(np.power, 10.0), np.log10))
plt.legend(fontsize="large", loc="best")
plt.gca().xaxis.set_major_formatter(FormatStrFormatter('%d'))
plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.2f'))
plt.gca().yaxis.set_minor_formatter(FormatStrFormatter('%.2f'))
plt.tight_layout()
filename = os.path.join(dirname, "hypervolume/hypervolume_quartiles_per_m.png")
plt.savefig(bbox_inches='tight', fname=filename, format="png")
filename = os.path.join(dirname, "hypervolume/hypervolume_quartiles_per_m.pdf")
plt.savefig(bbox_inches='tight', fname=filename, format="pdf")
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
                filename = os.path.join(dirname, "hypervolume/" + instance + "_" + solver + "_" + str(seed) + ".txt")
                if os.path.exists(filename):
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = ",")
                        for row in data:
                            hypervolume_per_size[solver][size].append(float(row[0]))
                        csv_file.close()

plt.figure()
plt.xlabel(fontsize="large", xlabel="Number of Items")
plt.ylabel(fontsize="large", ylabel="Hypervolume Ratio")
plt.tick_params(axis="both", which="both", labelsize="large")
plt.grid(alpha=0.5, color='gray', linestyle='dashed', linewidth=0.5, which='both')
for i in range(len(solvers)):
    y = []
    for size in sizes:
        y.append(stats.mean(hypervolume_per_size[solvers[i]][size]))
    plt.plot(sizes, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.80)
plt.xscale("log")
plt.yscale("function", functions=(partial(np.power, 10.0), np.log10))
plt.legend(fontsize="large", loc="best")
plt.gca().xaxis.set_major_formatter(FormatStrFormatter('%d'))
plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.2f'))
plt.tight_layout()
filename = os.path.join(dirname, "hypervolume/hypervolume_mean_per_size.png")
plt.savefig(bbox_inches='tight', fname=filename, format="png")
filename = os.path.join(dirname, "hypervolume/hypervolume_mean_per_size.pdf")
plt.savefig(bbox_inches='tight', fname=filename, format="pdf")
plt.close()

plt.figure()
plt.xlabel(fontsize="large", xlabel="Number of Items")
plt.ylabel(fontsize="large", ylabel="Hypervolume Ratio")
plt.tick_params(axis="both", which="both", labelsize="large")
plt.grid(alpha=0.5, color='gray', linestyle='dashed', linewidth=0.5, which='both')
for i in range(len(solvers)):
    y0 = []
    y2 = []
    for size in sizes:
        quantiles = stats.quantiles(hypervolume_per_size[solvers[i]][size])
        y0.append(quantiles[0])
        y2.append(quantiles[2])
    plt.fill_between(sizes, y0, y2, color = colors[i], alpha = 0.25)
for i in range(len(solvers)):
    y1 = []
    for size in sizes:
        quantiles = stats.quantiles(hypervolume_per_size[solvers[i]][size])
        y1.append(quantiles[1])
    plt.plot(sizes, y1, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.75)
plt.xscale("log")
plt.yscale("function", functions=(partial(np.power, 10.0), np.log10))
plt.legend(fontsize="large", loc="best")
plt.gca().xaxis.set_major_formatter(FormatStrFormatter('%d'))
plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.1f'))
plt.tight_layout()
filename = os.path.join(dirname, "hypervolume/hypervolume_quartiles_per_size.png")
plt.savefig(bbox_inches='tight', fname=filename, format="png")
filename = os.path.join(dirname, "hypervolume/hypervolume_quartiles_per_size.pdf")
plt.savefig(bbox_inches='tight', fname=filename, format="pdf")
plt.close()
