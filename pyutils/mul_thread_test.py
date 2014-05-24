import threading
import time
class timer(threading.Thread):
    def __init__(self,no,lis):
        #super(timer,self).__init__(self)
        threading.Thread.__init__(self)
        self.no=no
        self.lis=lis
    def run(self):
        no=self.no
        lis=self.lis
        for i in range(len(lis)):
            print '#',str(no),lis[i]
            time.sleep(1)
    #thread.exit_thread()

a=timer(1,['a','b'])
b=timer(2,['aa','bb'])
a.start()
b.start()
