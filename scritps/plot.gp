set title "Performance Data"
set xlabel "File"
set ylabel "Time / Energy"
set terminal png
set output 'plot_data.png'
set xtics rotate by -45
set style data histograms
set style histogram clustered
set boxwidth 0.5

plot 'data.csv' using 2:xtic(1) title 'Time' lc rgb 'blue', \
     'data.csv' using 3:xtic(1) title 'Energy' lc rgb 'red' axes x1y2


