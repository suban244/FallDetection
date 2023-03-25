class FallDetector:
    def __init__(self):
        self.fall = False
        self.trigger1 = False
        self.trigger2 = False
        self.trigger3 = False
        self.trigger1count = 0
        self.trigger2count = 0
        self.trigger3count = 0
        self.angleChange = 0
        
    def ruleBasedInference(self, accl: Dict[str, float], gyro: Dict[str, float]) -> bool:
        ax, ay, az = accl["x"], accl["y"], accl["z"]
        gx, gy, gz = gyro["x"], gyro["y"], gyro["z"]

        accl_amp = (ax**2 + ay**2 + az**2) ** 0.5
        
        print(accl_amp)

        if accl_amp <= 6 and self.trigger2 == False:
            self.trigger1 = True
            print('trigger1')

        if self.trigger1 == True:
            self.trigger1count += 1
            if accl_amp >= 12:
                self.trigger2 = True
                self.trigger1 = False
                self.trigger1count = 0
                print('trigger2')

        if self.trigger2 == True:
            self.trigger2count += 1
            self.angleChange = (gx**2 + gy**2 + gz**2) ** 0.5
            if self.angleChange >= 30 and self.angleChange <= 400:
                self.trigger3 = True
                self.trigger2 = False
                self.trigger2count = 0
                print('trigger3')

        if self.trigger3 == True:
            self.trigger3count += 1
            if self.trigger3count >= 10 * 3:
                self.angleChange = (gx**2 + gy**2 + gz**2) ** 0.5
                if self.angleChange >= 0 and self.angleChange <= 10:
                    self.fall = True
                    self.trigger3 = False
                    self.trigger3count = 0
                    print('t4')
                else:
                    self.trigger3 = False
                    self.trigger3count = 0

        if self.fall == True:
            # inform the user
            pass

        if self.trigger2count >= 6 *2:
            self.trigger2 = False
            self.trigger2count = 0

        if self.trigger1count >= 6 *2:
            self.trigger1 = False
            self.trigger1count = 0
        
        return self.fall