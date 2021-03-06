instances = ["zlt_100_2", "zlt_250_2", "zlt_500_2", "zlt_750_2", "zlt_100_3", "zlt_250_3", "zlt_500_3", "zlt_750_3", "zlt_100_4", "zlt_250_4", "zlt_500_4", "zlt_750_4"]
solvers = ["nsga2", "nspso", "moead", "mhaco", "ihs", "nsbrkga"]
solver_labels = {"nsga2": "NSGA-II", "nspso": "NSPSO", "moead": "MOEA/D-DE", "mhaco": "MHACO", "ihs": "IHS", "nsbrkga": "NS-BRKGA"}
decoder_types = [1]
decoder_types_labels = {0: "", 1: ""}
seeds = [319086417, 293920772, 121513023, 317048708, 746557472]
versions = ["best", "median"]
colors = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd", "#8c564b", "#e377c2", "#7f7f7f", "#bcbd22", "#17becf", "#8c7e6e", "#738191"]
colors2 = ["#103c5a", "#804007", "#165016", "#6b1414", "#4a345f", "#462b26", "#723c61", "#404040", "#5e5f11", "#0c5f68", "#463f37", "#3a4149"]
ms = [2, 3, 4]
instances_per_m = {2 : ["zlt_100_2", "zlt_250_2", "zlt_500_2", "zlt_750_2"], 3 : ["zlt_100_3", "zlt_250_3", "zlt_500_3", "zlt_750_3"], 4 : ["zlt_100_4", "zlt_250_4", "zlt_500_4", "zlt_750_4"]}
sizes = [100, 250, 500, 750]
instances_per_size = {100 : ["zlt_100_2", "zlt_100_3", "zlt_100_4"], 250 : ["zlt_250_2", "zlt_250_3", "zlt_250_4"], 500 : ["zlt_500_2", "zlt_500_3", "zlt_500_4"], 750 : ["zlt_750_2", "zlt_750_3", "zlt_750_4"]}
num_snapshots = 60
