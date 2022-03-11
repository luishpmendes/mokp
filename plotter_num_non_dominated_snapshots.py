import csv
import matplotlib.pyplot as plt
import os

instances = ["zlt_100_2", "zlt_250_2", "zlt_500_2", "zlt_750_2", "zlt_100_3", "zlt_250_3", "zlt_500_3", "zlt_750_3", "zlt_100_4", "zlt_250_4", "zlt_500_4", "zlt_750_4"]
solvers = ["nsga2", "nspso", "moead", "mhaco", "ihs", "nsbrkga"]
solver_labels = {"nsga2": "NSGA-II", "nspso": "NSPSO", "moead": "MOEA/D-DE", "mhaco": "MHACO", "ihs": "IHS", "nsbrkga": "NS-BRKGA"}
versions = ["best", "median"]

for instance in instances:
    for version in versions:
        plt.figure(figsize = (5, 5))
        plt.title(instance)
        plt.xlabel("Time (s)")
        plt.ylabel("Non-dominated Solutions")
        for solver in solvers:
            filename = "num_non_dominated_snapshots/" + instance + "_" + solver + "_" + version + ".txt"
            if os.path.exists(filename):
                x = []
                y = []
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = " ")
                    for row in data:
                        x.append(float(row[1]))
                        y.append(float(row[2]))
                plt.plot(x, y, label = solver_labels[solver], alpha = 0.67)
        plt.xlim(left = 0)
        plt.ylim(bottom = 0)
        plt.legend(loc = 'best')
        plt.savefig("num_non_dominated_snapshots/" + instance + "_" + version + ".png", format = "png")
        plt.close()
