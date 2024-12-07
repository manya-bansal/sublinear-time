import matplotlib.pyplot as plt
import numpy as np

# Data
x_values = [50000, 100000, 250000, 500000, 750000, 1000000]
y1_values = [0.000327666, 0.00042, 0.000369541, 0.00046025, 0.000446708, 0.000396959]
y2_values = [0.000216458, 0.000372, 0.000964833, 0.00195004, 0.002807, 0.00371263]

# Create the plot
plt.figure(figsize=(10, 6))
plt.plot(x_values, y1_values, marker='o', label='Sub-linear')
plt.plot(x_values, y2_values, marker='s', label='Linear')

# Set labels and title
plt.xlabel('Size of File in words')
plt.ylabel('Runtime (s)')
plt.title('10 20 30 70 80')

# Set x-axis to logarithmic scale
plt.xscale('log')
plt.yscale('log')

# Add legend
plt.legend()

# Add grid
plt.grid(True, which="both", ls="-", alpha=0.2)
plt.savefig('data_plot.pdf', bbox_inches='tight')

# Show plot
plt.tight_layout()
plt.show()