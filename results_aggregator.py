import csv
import os
from plotter_definitions import *
import pandas as pd

dirname = os.path.dirname(__file__)

metrics_labels = ["Hypervolume Ratio", "Modified Inverted Generational Distance", "Multiplicative Epsilon Indicator"]
metrics_labels_to_code = {"Hypervolume Ratio": "hypervolume", "Modified Inverted Generational Distance": "igd_plus", "Multiplicative Epsilon Indicator": "multiplicative_epsilon"}

metrics = []

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "hypervolume/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics.append(["MOMDKP", instance, m_per_instance[instance], size_per_instance[instance], solver_labels[solvers[i]], seed, metrics_labels[0], float(row[0])])
                    csv_file.close()

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "igd_plus/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics.append(["MOMDKP", instance, m_per_instance[instance], size_per_instance[instance], solver_labels[solvers[i]], seed, metrics_labels[1], float(row[0])])
                    csv_file.close()

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "multiplicative_epsilon/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics.append(["MOMDKP", instance, m_per_instance[instance], size_per_instance[instance], solver_labels[solvers[i]], seed, metrics_labels[2], float(row[0])])
                    csv_file.close()

df_metrics = pd.DataFrame(metrics, columns=["problem", "instance", "number of objectives", "chromosome size", "solver", "seed", "metric name", "metric value"])
df_metrics.to_csv(os.path.join(dirname, "metrics.csv"), index=False)

df_metrics_grouped = df_metrics.groupby(['solver', 'metric name'])['metric value'].agg(['mean', 'std']).reset_index()

for metric in df_metrics_grouped['metric name'].unique():
    df_metric = df_metrics_grouped[df_metrics_grouped['metric name'] == metric].drop(columns=['metric name'])

    if metrics_labels_to_code[metric] == "igd_plus":
        df_metric['rank'] = df_metric['mean'].rank(method='min', ascending=True)
    else:
        df_metric['rank'] = df_metric['mean'].rank(method='min', ascending=False)

    df_metric.to_csv(os.path.join(dirname, metrics_labels_to_code[metric] + "_stats.csv"), index=False)

df_metrics_grouped_by_size = df_metrics.groupby(['chromosome size', 'solver', 'metric name'])['metric value'].agg(['mean', 'std']).reset_index()

for metric in df_metrics_grouped_by_size['metric name'].unique():
    df_metric_by_size = df_metrics_grouped_by_size[df_metrics_grouped_by_size['metric name'] == metric].drop(columns=['metric name'])

    if metrics_labels_to_code[metric] == "igd_plus":
        df_metric_by_size['rank'] = df_metric_by_size.groupby('chromosome size')['mean'].rank(method='min', ascending=True)
    else:
        df_metric_by_size['rank'] = df_metric_by_size.groupby('chromosome size')['mean'].rank(method='min', ascending=False)

    df_metric_by_size.to_csv(os.path.join(dirname, metrics_labels_to_code[metric] + "_by_size_stats.csv"), index=False)

df_metrics_grouped_by_objectives = df_metrics.groupby(['number of objectives', 'solver', 'metric name'])['metric value'].agg(['mean', 'std']).reset_index()

for metric in df_metrics_grouped_by_objectives['metric name'].unique():
    df_metric_by_objectives = df_metrics_grouped_by_objectives[df_metrics_grouped_by_objectives['metric name'] == metric].drop(columns=['metric name'])

    if metrics_labels_to_code[metric] == "igd_plus":
        df_metric_by_objectives['rank'] = df_metric_by_objectives.groupby('number of objectives')['mean'].rank(method='min', ascending=True)
    else:
        df_metric_by_objectives['rank'] = df_metric_by_objectives.groupby('number of objectives')['mean'].rank(method='min', ascending=False)

    df_metric_by_objectives.to_csv(os.path.join(dirname, metrics_labels_to_code[metric] + "_by_objectives_stats.csv"), index=False)

metrics_snapshots = []

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "hypervolume_snapshots/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics_snapshots.append(["MOMDKP", instance, m_per_instance[instance], size_per_instance[instance], solver_labels[solvers[i]], seed, metrics_labels[0], float(row[1]), float(row[2])])
                    csv_file.close()

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "igd_plus_snapshots/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics_snapshots.append(["MOMDKP", instance, m_per_instance[instance], size_per_instance[instance], solver_labels[solvers[i]], seed, metrics_labels[1], float(row[1]), float(row[2])])
                    csv_file.close()

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "multiplicative_epsilon_snapshots/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics_snapshots.append(["MOMDKP", instance, m_per_instance[instance], size_per_instance[instance], solver_labels[solvers[i]], seed, metrics_labels[2], float(row[1]), float(row[2])])
                    csv_file.close()

df_metrics_snapshots = pd.DataFrame(metrics_snapshots, columns=["problem", "instance", "number of objectives", "chromosome size", "solver", "seed", "metric name", "snapshot time", "metric value"])
df_metrics_snapshots.to_csv(os.path.join(dirname, "metrics_snapshots.csv"), index=False)
