import csv
import matplotlib.pyplot as plt

data = []

with open("support_data.csv", "r") as file:
    reader = csv.DictReader(file)

    for row in reader:
        data.append(row)

# get freq itemsets' lvls
levels = sorted(set(int(row["Level"]) for row in data))

# for each lvl, draw chart
for level in levels:

    current = [
        row for row in data
        if int(row["Level"]) == level
    ]

    itemsets = [row["Itemset"] for row in current]
    supports = [float(row["Support"]) for row in current]
    counts = [int(row["SupportCount"]) for row in current]
    
    plt.figure(figsize=(10, 5))

    supports_percent = [s * 100 for s in supports]

    bars = plt.bar(itemsets, supports_percent)

    plt.title(f"Frequent Itemsets Support - L{level}", pad = 15)
    plt.xlabel("Itemsets")
    plt.ylabel("Support (%)")
    plt.ylim(0, 100)
    plt.yticks(range(0, 101, 20))

    # display supp at top of each  bar
    for bar, count in zip(bars, counts):

        plt.text(
            bar.get_x() + bar.get_width() / 2,
            bar.get_height(),
            str(count),
            ha="center",
            va="bottom"
        )

    plt.xticks(rotation=45)
    plt.tight_layout()

    # save
    plt.savefig(f"support_L{level}.png")

    # display
    plt.show()

print("All charts generated successfully!")