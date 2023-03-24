import numpy as np
import random
import pandas as pd
import csv

filename = 'walkSaurav1'
filepath = 'data/'+filename+'.csv'
df = pd.read_csv(filepath)

input_seq_length = 30
df_len = df.shape[0]

for i in range(30):
    starting_index = np.random.randint(0, high=df_len-input_seq_length)
    df2 = df.iloc[starting_index: starting_index + input_seq_length]

    # open the file in the write mode
    f = open(f'split/{filename}_{i}.csv', 'w')

    df2.to_csv(f)

    f.close()
