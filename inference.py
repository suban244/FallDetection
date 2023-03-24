import sys
import csv
import serial
import time
from torch import nn
import torch
import torch.nn.functional as F
# Open the serial port with PySerial
ser = serial.Serial('/dev/ttyACM0', 9600)  # IMU data

start = time.time()
queue = []
count = 0


class FallDetector(nn.Module):

    def __init__(self):
        super(FallDetector, self,).__init__()
        self.lstm = nn.LSTM(6, 20, num_layers=1)
        self.hidden2tag = nn.Linear(20, 4)

    def forward(self, seq):
        output, (h_n, c_n) = self.lstm(seq.view(len(seq), 1, -1))
        tag_space = self.hidden2tag(c_n.view(1, -1))
        # print(tag_space.data)

        return tag_space
        self.hidden2tag = nn.Linear(20, 4)

    # def forward(self, seq):
    #     output, (h_n, c_n) = self.lstm(seq.view(len(seq), 1, -1))
    #     tag_space = self.hidden2tag(c_n.view(1, -1))
    #     # print(tag_space.data)

    #     return tag_space

# class CNNNetwork(nn.Module):
#     def __init__(self):
#         super(CNNNetwork, self,).__init__()
#         self.cnn1 = nn.Conv1d(6, 12, 3)
#         self.cnn2 = nn.Conv1d(12, 24, 3)
#         self.cnn3 = nn.Conv1d(24, 48, 3)
#         self.MP = nn.MaxPool1d(3)

#         self.lstm = nn.LSTM(48, 24, num_layers=1)
#         self.hidden2tag = nn.Linear(24, 4)

#     def forward(self, seq):
#         seq = seq.reshape((1, 6, 40))
#         seq = self.cnn1(seq)
#         seq = F.relu(seq)

#         seq = self.cnn2(seq)
#         seq = F.relu(seq)

#         seq = self.cnn3(seq)
#         seq = F.relu(seq)
#         seq = self.MP(seq)

#         seq = seq.reshape(-1, 48)
#         output, (h_n, c_n) = self.lstm(seq)
#         tag_space = self.hidden2tag(c_n.view(1, -1))
#         return tag_space


model = FallDetector()
# model = CNNNetwork()

model.load_state_dict(torch.load("model.pt"))
model.eval()


def model_time():
    # print(queue)
    t = torch.tensor(queue, dtype=torch.float)
    global count
    # print("about to eval")
    result = model(t)
    index = torch.argmax(result)
    print("index ", index)


counter = 0

while True:
    try:
        line = ser.readline().decode()
        if line:
            # Split the line into a timestamp and data
            timestamp = line.strip().split(',')
            if (len(timestamp) == 6):
                varlist = [timestamp[0], timestamp[1],
                           timestamp[2], timestamp[3], timestamp[4], timestamp[5]]

                for i in range(6):
                    varlist[i] = float(varlist[i])

                if (len(queue) < 40):
                    queue.append(varlist)
                    print(varlist)
                else:
                    # print([timestamp[0],
                    #    timestamp[1], timestamp[2], timestamp[3], timestamp[4], timestamp[5

                    queue.pop(0)
                    queue.append(varlist)
                    if counter == 5:

                        model_time()
                        counter = 0
                    else:
                        counter += 1

    except:
        continue
