# x axis: mass -10 to + 10 -> 0.5 - 5
# y axis: dist -> 0 to 30

def translate(value, leftMin, leftMax, rightMin, rightMax):
    # Figure out how 'wide' each range is
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin

    # Convert the left range into a 0-1 range (float)
    valueScaled = float(value - leftMin) / float(leftSpan)

    # Convert the 0-1 range into a value in the right range.
    return rightMin + (valueScaled * rightSpan)


def converter(accel: dict):
    mass = translate(accel['x'], -10, 10, 0.5, 5)
    dist = translate(accel['y'], -10, 10, 0, 30)
    print(f"{mass},{dist},{0.0},{0.0}")
