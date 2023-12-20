#!/usr/bin/env python3

import sys
import pandas as pd
import matplotlib.pyplot as plt

def plot(data, timestamp):
    data = pd.read_csv(data, delimiter=' ')
    fig, ax1 = plt.subplots(figsize=(10, 6))

    # Bar plots with error bars for energy
    ax1.bar(data['file'], data['avg-energy'], yerr=[data['avg-energy'] - data['min-energy'], data['max-energy'] - data['avg-energy']], capsize=5, label='Avg Energy', color='blue', alpha=0.7)

    # Set labels and title for the first y-axis
    ax1.set_xlabel('File')
    ax1.set_ylabel('Energy', color='blue')
    ax1.tick_params(axis='y', labelcolor='blue')
    ax1.legend(loc='upper left')

    # Create a secondary y-axis for time
    ax2 = ax1.twinx()
    ax2.plot(data['file'], data['time'], label='Time', color='green', marker='o')

    # Set labels and title for the second y-axis
    ax2.set_ylabel('Time', color='green')
    ax2.tick_params(axis='y', labelcolor='green')
    ax2.legend(loc='upper right')

    # Rotate x-axis labels for better readability
    plt.xticks(rotation=45, ha='right')

    # Set overall title
    plt.title('Energy and Time Analysis')

    # Save the plot to a file
    plt.savefig(f'analyzes/energy_time_{timestamp}.png')

if __name__ == "__main__":
    # Check if at least one command-line argument is provided
    if len(sys.argv) < 2:
        print(f"Usage: python {sys.argv[0]} <data.csv>")
        sys.exit(1)

    # Get the first command-line argument
    data = sys.argv[1]
    timestamp = data[-19:][:-4]
    plot(data, timestamp)


