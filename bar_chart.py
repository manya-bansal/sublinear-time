import matplotlib.pyplot as plt
from collections import Counter

# Data
data = [0.979999, 0.984, 0.975999, 0.975999, 0.98, 0.975999, 0.975999, 0.979999, 0.984, 0.979999, 
        0.98, 0.975999, 0.975999, 0.975999, 0.979999, 0.98, 0.975999, 0.975999, 0.98, 0.979999, 
        0.975999, 0.995999, 1.004, 0.975999, 0.975999, 0.975999, 0.995999, 0.975999, 0.975999, 
        0.975999, 0.975999, 0.975999, 0.984, 0.98, 0.975999, 1, 0.979999, 0.98, 0.975999, 0.979999, 
        0.975999, 0.98, 0.975999, 0.98, 0.975999, 0.975999, 0.979999, 0.979999, 0.979999, 0.984, 
        0.975999, 0.984, 0.98, 0.98, 0.98, 0.975999, 0.984, 0.975999, 0.979999, 0.975999, 0.984, 
        0.999999, 0.98, 0.98, 0.975999, 0.984, 0.979999, 0.984, 0.984, 0.979999, 0.984, 0.975999, 
        0.984, 0.975999, 0.98, 1.004, 0.975999, 0.98, 0.975999, 0.975999, 0.975999, 0.975999, 
        0.975999, 0.98, 0.98, 0.975999, 0.975999, 0.975999, 0.975999, 0.984, 0.975999, 0.98, 
        0.975999, 0.975999, 0.975999, 0.975999, 0.975999, 0.98, 0.979999, 0.975999, 1, 0.98, 
        0.975999, 0.979999, 0.979999, 0.975999, 0.999999, 0.975999, 0.975999, 0.98, 0.98, 0.98, 
        0.996, 0.98, 0.975999, 0.975999, 0.984, 0.979999, 0.98, 0.975999, 0.984, 0.975999, 0.979999, 
        0.98, 0.979999, 0.975999, 0.979999]

# Convert to string labels
def format_value(val):
    # Round to 3 decimal places and convert to string
    return f"{val:.3f}"

# Count the frequency of each unique value
value_counts = Counter(format_value(x) for x in data)

# Create the plot
plt.figure(figsize=(12, 6))

# Sort values by frequency in descending order
sorted_counts = sorted(value_counts.items(), key=lambda x: x[1], reverse=True)

# Unpack the sorted data
values = [x[0] for x in sorted_counts]
frequencies = [x[1] for x in sorted_counts]

# Create bar plot
plt.bar(range(len(values)), frequencies, color='blue', alpha=0.7)

# Customize the plot
plt.title('Frequency of Values', fontsize=16)
plt.xlabel('Storage Used', fontsize=12)
plt.ylabel('Frequency', fontsize=12)

# Set x-axis labels
plt.xticks(range(len(values)), values, rotation=45, ha='right')

# Add value labels on top of each bar
for i, v in enumerate(frequencies):
    plt.text(i, v, str(v), ha='center', va='bottom')

# Adjust layout
plt.tight_layout()

# Save the figure
plt.savefig('frequency_chart.png', dpi=300)
plt.savefig('frequency_chart.pdf', bbox_inches='tight')

# Show the plot
plt.show()

# Print detailed frequency information
print("Detailed Frequency Breakdown:")
for value, count in sorted_counts:
    print(f"{value}: {count} times")