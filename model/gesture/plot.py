import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('../data/try2/dataset.csv')

df.iloc[1700:2000].plot(y=['acc_x'])
plt.legend(loc='best')
plt.show()
