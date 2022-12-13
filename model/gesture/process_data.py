import pandas as pd

SRCFILE = 'data/moving.csv'
TARFILE = 'data/moving-processed.csv'

df = pd.read_csv(SRCFILE)

# Validate
print(df.iloc[0:15])

# Replace empty cells with previous value
df = df.fillna(method="ffill")

# reset indices
df = df.reset_index(drop=True)

if True:
    def divide_by_32(value):
        return value / 32

    df = df.applymap(divide_by_32)
    print(df)

if True:
    NUM_VALUES = 48
    axes = ['acc_x_', 'acc_y_', 'acc_z_']
    columns = [axes[i // NUM_VALUES] + str(i % NUM_VALUES) for i in range(3 * NUM_VALUES)] + ['val']

    new_df = pd.DataFrame(columns=columns)

    for row in range(NUM_VALUES, len(df)):
        if (row % 1000 == 0):
            print("running... %d/%d" % (row // 1000, len(df) // 1000))
        
        new_row = dict()
        try:
            for i in range(NUM_VALUES):
                new_row[columns[i]] = df.loc[row - NUM_VALUES + 1 + i, 'acc_x']

            for i in range(NUM_VALUES, 2 * NUM_VALUES):
                new_row[columns[i]] = df.loc[row - 2 * NUM_VALUES + 1 + i, 'acc_y']
            
            for i in range(2 * NUM_VALUES, 3 * NUM_VALUES):
                new_row[columns[i]] = df.loc[row - 3 * NUM_VALUES + 1 + i, 'acc_z']

            new_row['val'] = 0

            new_df = pd.concat([new_df, pd.Series(new_row).to_frame().T], ignore_index=True)
        except:
            print("Error, line", row)
        

    print("\Done\n")
    print(new_df.iloc[1:2, :])
    print(len(new_df))

    # Save data
    new_df.to_csv(TARFILE, index=False)
