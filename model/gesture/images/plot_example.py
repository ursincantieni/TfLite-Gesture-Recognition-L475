import pandas as pd
import matplotlib.pyplot as plt

exampleDf = pd.read_csv('example.csv')

FROM=600
TO=720

x = [i for i in range(TO-FROM)]

exampleX=exampleDf.iloc[FROM:TO].loc[:,"acc_x"].to_list()
exampleY=exampleDf.iloc[FROM:TO].loc[:,"acc_y"].to_list()
exampleZ=exampleDf.iloc[FROM:TO].loc[:,"acc_z"].to_list()
exampleP=exampleDf.iloc[FROM:TO].loc[:,"prediction"].to_list()
exampleP=[500*i-1000 for i in exampleP]

pred0=[-1000]*(TO-FROM)
pred1=[-500]*(TO-FROM)
#plt.figure(figsize=(19.20, 10.80))
plt.figure()
plt.plot(x, exampleX, label="Acc X")
plt.plot(x, exampleY, label="Acc Y")
#plt.plot(x, exampleZ, label="Acc Z")
plt.plot(x, pred1, label="click", color='xkcd:salmon', linestyle="--")
plt.plot(x, pred0, label="no click", color='xkcd:light pink', linestyle="--")
plt.plot(x, exampleP, label="Prediction", color="red")
plt.title("Example")
plt.xlabel("Time [1/48 s]")
plt.ylabel("Acceleration [m/s^2]")
plt.ylim(-1200, 1000)
plt.legend(loc='best')
plt.savefig('example-plot.png', dpi=1200)
plt.show()

