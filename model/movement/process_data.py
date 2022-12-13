import pandas as pd

SRCFILE = 'data/dataset1.csv'
TARFILE = 'data/dataset1-processed3.csv'

df = pd.read_csv(SRCFILE)

# Validate
print(df.iloc[0:15])

# Replace empty cells with previous value
df = df.fillna(method="ffill")

# drop lines where dir is not as wished (not sure why)
df = df.drop(
    df[
        ((df['ud_dir'] != 255) & (df['ud_dir'] != 0)) |
        ((df['lr_dir'] != 255) & (df['lr_dir'] != 0))
    ].index)

# Shift values around 0. Positive -> right/down, negative -> left/up
df.loc[df['lr_dir'] == 255, 'lr_vel'] -= 256
df.loc[df['ud_dir'] == 255, 'ud_vel'] -= 256

# delete 'dir' columns

df = df.drop(
    columns=['lr_dir', 'ud_dir']
)

# reset indices
df = df.reset_index(drop=True)


# 'normalize data': subtract 1000 from acc_z, divide all inputs by 10

if True:
    def subtract_1000(value):
        return value - 1000
    def divide_by_32(value):
        return value / 32
    def round_to_int(value):
        return int(round(value, 0))

    #print(df)
    df['acc_z'] = df['acc_z'].apply(subtract_1000)  
    #print(df)
    #df[['acc_x', 'acc_y', 'acc_z']] = df[['acc_x', 'acc_y', 'acc_z']].apply(divide_by_10)
    df = df.applymap(divide_by_32)
    #print(df)
    df = df.applymap(round_to_int)
    #print(df)


if True:
    """ 
    columns = ["acc_x", "acc_y", "acc_z"]
    d = {
        columns[i] : [j for j in range(100)]
        for i in range(3)
    }
    d['lr_vel'] = ["LR" + str(i) for i in range(100)]
    d['ud_vel'] = ["UD" + str(i) for i in range(100)]

    df = pd.DataFrame(data=d)
    print(df) """




    NUM_VALUES = 48
    axes = ['acc_x_', 'acc_y_', 'acc_z_']
    columns = [axes[i // NUM_VALUES] + str(i % NUM_VALUES) for i in range(3 * NUM_VALUES)] + ['x_vel', 'y_vel']

    new_df = pd.DataFrame(columns=columns)

    for row in range(NUM_VALUES, len(df) - 1):
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

            new_row[columns[3 * NUM_VALUES]] = df.loc[row,'lr_vel']
            new_row[columns[3 * NUM_VALUES + 1]] = df.loc[row,'ud_vel']

            new_df = pd.concat([new_df, pd.Series(new_row).to_frame().T], ignore_index=True)
        except:
            print("Error, line", row)
        

    print("\Done\n")
    print(new_df.iloc[1:2, :])
    print(len(new_df))

    # Save data
    new_df.to_csv(TARFILE, index=False)
