import pandas as pd
import matplotlib.pyplot as plt

shakingDf = pd.read_csv('data/shaking.csv')
holdingDf = pd.read_csv('data/holding.csv')
movingDf = pd.read_csv('data/moving.csv')

FROM=200
TO=250

x = [i for i in range(TO-FROM)]

shakingX=shakingDf.iloc[FROM:TO].loc[:,"acc_x"].to_list()
shakingY=shakingDf.iloc[FROM:TO].loc[:,"acc_y"].to_list()
shakingZ=shakingDf.iloc[FROM:TO].loc[:,"acc_z"].to_list()
movingX=movingDf.iloc[FROM:TO].loc[:,"acc_x"].to_list()
movingY=movingDf.iloc[FROM:TO].loc[:,"acc_y"].to_list()
movingZ=movingDf.iloc[FROM:TO].loc[:,"acc_z"].to_list()
holdingX=holdingDf.iloc[FROM:TO].loc[:,"acc_x"].to_list()
holdingY=holdingDf.iloc[FROM:TO].loc[:,"acc_y"].to_list()
holdingZ=holdingDf.iloc[FROM:TO].loc[:,"acc_z"].to_list()

#Plot shaking
#plt.figure(figsize=(19.20, 10.80))
plt.figure()
plt.plot(x, shakingX, label="Acc X")
plt.plot(x, shakingY, label="Acc Y")
plt.plot(x, shakingZ, label="Acc Z")
plt.title("Accelerometer data during shaking")
plt.xlabel("Time [1/48 s]")
plt.ylabel("Acceleration [m/s^2]")
plt.ylim(-1000, 2800)
plt.legend(loc='best')
plt.savefig('images/shaking-plot.png', dpi=1200)
plt.show()

#Plot moving
plt.figure()
plt.plot(x, movingX, label="Acc X")
plt.plot(x, movingY, label="Acc Y")
plt.plot(x, movingZ, label="Acc Z")
plt.title("Accelerometer data during moving")
plt.xlabel("Time [1/48 s]")
plt.ylabel("Acceleration [m/s^2]")
plt.legend(loc='best')
plt.ylim(-1000, 2800)
plt.savefig('images/moving-plot.png', dpi=1200)
plt.show()

#Plot holding
plt.figure()
plt.plot(x, holdingX, label="Acc X")
plt.plot(x, holdingY, label="Acc Y")
plt.plot(x, holdingZ, label="Acc Z")
plt.title("Accelerometer data during holding")
plt.xlabel("Time [1/48 s]")
plt.ylabel("Acceleration [m/s^2]")
plt.ylim(-1000, 2800)
plt.legend(loc='best')
plt.savefig('images/holding-plot.png', dpi=1200)
plt.show()

