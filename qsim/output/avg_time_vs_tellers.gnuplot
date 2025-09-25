set title "Average Time in Bank vs Number of Tellers"
set xlabel "Number of Tellers"
set ylabel "Average Time in Bank (minutes)"
set grid
plot "output/avg_time_vs_tellers.dat" using 1:2 with linespoints title "Avg Time"
pause -1 "Press Enter to exit..."
