
result = self.command('AT+CMGS="{}"\n'.format(destno),
                      99, 5000, msgtext+'\x1A')


def command(self, cmdstr, lines=1, waitfor=500, msgtext=None):
    # flush input
    # print(cmdstr)
    while self._uart.any():
        self._uart.readchar()
    self._uart.write(cmdstr)
    if msgtext:
        self._uart.write(msgtext)
    if waitfor > 1000:
        pyb.delay(waitfor-1000)
    buf = self._uart.readline()  # discard linefeed etc
    # print(buf)
    buf = self._uart.readline()
    # print(buf)
    if not buf:
        return None
    result = convert_to_string(buf)
    if lines > 1:
        self.savbuf = ''
        for i in range(lines-1):
            buf = self._uart.readline()
            if not buf:
                return result
            # print(buf)
            buf = convert_to_string(buf)
            if not buf == '' and not buf == 'OK':
                self.savbuf += buf+'\n'
    return result
