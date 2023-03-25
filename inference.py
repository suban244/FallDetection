import turtle
import sys
import csv
import serial
import time
from torch import nn
import torch
import torch.nn.functional as F
# Open the serial port with PySerial
ser = serial.Serial('/dev/ttyACM1', 9600)  # IMU data

start = time.time()
queue = []
count = 0


scr = turtle.Screen()
pen = turtle.Turtle()
font = 'Arial'
fontsize = 60
style = 'bold'  # You can use 'bold', 'italic', 'overstrike' or 'underline'
align = 'center'
message = "text"


class FallDetector(nn.Module):

    def __init__(self):
        super(FallDetector, self,).__init__()
        self.lstm = nn.LSTM(6, 20, num_layers=1)
        self.hidden2tag = nn.Linear(20, 3)

    def forward(self, seq):
        output, (h_n, c_n) = self.lstm(seq.view(len(seq), 1, -1))
        tag_space = self.hidden2tag(c_n.view(1, -1))
        # print(tag_space.data)

        return tag_space


class CNNNetwork(nn.Module):
    def __init__(self):
        super(CNNNetwork, self,).__init__()
        self.cnn1 = nn.Conv1d(6, 12, 3)
        self.cnn2 = nn.Conv1d(12, 24, 3)
        self.cnn3 = nn.Conv1d(24, 48, 3)
        self.MP = nn.MaxPool1d(3)

        self.lstm = nn.LSTM(48, 24, num_layers=1)
        self.hidden2tag = nn.Linear(24, 3)

    def forward(self, seq):
        seq = seq.reshape(1, 6, 30)
        seq = self.cnn1(seq)
        seq = F.relu(seq)

        seq = self.cnn2(seq)
        seq = F.relu(seq)

        seq = self.cnn3(seq)
        seq = F.relu(seq)
        seq = self.MP(seq)

        seq = seq.reshape(-1, 48)
        output, (h_n, c_n) = self.lstm(seq)
        tag_space = self.hidden2tag(c_n.view(1, -1))
        return tag_space


# model = FallDetector()
model = CNNNetwork()

model.load_state_dict(torch.load("model3.pt"))
model.eval()

index_map = {
    0: 'Fall',
    1: 'Idle',
    2: "Walk"
}


def model_time():
    # print(queue)
    t = torch.tensor(queue, dtype=torch.float)
    global count
    # print("about to eval")
    result = model(t)
    print(result)
    index = torch.argmax(result)
    # if index == 2:
    #     if (torch.max(torch.abs(t[:, 0:3]))) > 20:
    #         index = 0
    pen.clear()
    pen.write(index_map[int(index)], font=(font, fontsize, style), align=align)
    pen.hideturtle()


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

                if (len(queue) < 30):
                    queue.append(varlist)
                    print(varlist)
                else:
                    # print([timestamp[0],
                    #    timestamp[1], timestamp[2], timestamp[3], timestamp[4], timestamp[5

                    # queue.pop(0)
                    # queue.append(varlist)
                    queue.pop(-1)
                    queue.insert(0, varlist)
                    if counter == 10:

                        model_time()
                        counter = 0
                    else:
                        counter += 1

    except:
        continue
