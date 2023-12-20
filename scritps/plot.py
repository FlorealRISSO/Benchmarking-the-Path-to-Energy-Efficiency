#!/usr/bin/env python3

# MIT License
# 
# Copyright (c) 2023 Floréal Risso <floreal.risso@univ-tlse3.fr>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys
import pandas as pd
import matplotlib.pyplot as plt

def plot(timestamp):
    filename = f'analyzes/data_{timestamp}.csv'
    try:
        data = pd.read_csv(filename, delimiter=' ')
    except FileNotFoundError:
        print(f'run: ./preprocess.sh {timestamp} before.')
        sys.exit(1)

    fig, ax1 = plt.subplots(figsize=(10, 6))

    # Bar plots with error bars for energy
    ax1.bar(data['file'], data['avg-energy'], yerr=[data['avg-energy'] - data['min-energy'], data['max-energy'] - data['avg-energy']], capsize=5, label='Avg Energy', color='blue', alpha=0.7)

    # Set labels and title for the first y-axis
    ax1.set_xlabel('Algorithm')
    ax1.set_ylabel('Energy (Joule)', color='blue')
    ax1.tick_params(axis='y', labelcolor='blue')
    ax1.legend(loc='upper left')

    # Create a secondary y-axis for time
    ax2 = ax1.twinx()
    ax2.plot(data['file'], data['time'], label='Time', color='green', marker='o')

    # Set labels and title for the second y-axis
    ax2.set_ylabel('Time (second)', color='green')
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
    if len(sys.argv) != 2:
        print(f"Usage: python {sys.argv[0]} <timestamp>")
        sys.exit(1)

    # Get the first command-line argument
    timestamp = sys.argv[1]
    plot(timestamp)


