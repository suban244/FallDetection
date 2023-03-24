import csv
import pandas as pd
import random
import numpy as np

# Provide file name with path for example: "C:\Users\xxxxx\flights.csv"
split_source_file = "downstairs.csv"

# find number of lines using Pandas
df = pd.read_csv("downstairs0.csv")

input_seq_length = 30

df_len = df.shape[0]
print(df_len)
print(df_len-input_seq_length)

starting_index = np.random.randint(0, high=df_len-input_seq_length)

df2 = df.iloc[starting_index: starting_index + input_seq_length]

# open the file in the write mode
f = open(f'1.csv', 'w')

df2.to_csv(f)

f.close()
