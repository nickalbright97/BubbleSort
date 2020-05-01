import csv
import matplotlib.pyplot as plt
import numpy as np



for i in range (0,278):
    with open('CSVs/arr' + str(i) + '.csv') as csvDataFile:
        csvReader = csv.reader(csvDataFile)
        l = []
        for row in csvReader:
            l.append(row)

        arr = np.array(l)
        arr = arr.astype(int)


        plt.imshow(arr, cmap="gray")
        #plt.show()
        plt.savefig('Images/img' + str(i) + '.png')