# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'remoteQt.ui'
#
# Created by: PyQt5 UI code generator 5.10.1
#
# WARNING! All changes made in this file will be lost!

import PyQt5
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtCore import QThread, pyqtSignal, QRectF
from time import sleep
import asyncore
import numpy as np
import pickle
from imageThread import ImageThread
import cv2
import time

DEBUG = False

class Ui_MainWindow(object):
    
    def __init__(self):
        self.setupUi(self)

        self.ImageThread = ImageThread()
        self.ImageThread.new_image.connect(self.viewImage)

        self.checkThreadTimer = QtCore.QTimer(self)
#        self.checkThreadTimer.start(1000)
        self.checkThreadTimer.timeout.connect(self.getWiFiRxLevel)
    
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1305, 913)
        MainWindow.setFocusPolicy(QtCore.Qt.StrongFocus)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

#Move the Servo Head
        self.fwButton = QtWidgets.QPushButton(self.centralwidget)
        self.fwButton.setGeometry(QtCore.QRect(840, 700, 51, 41))
        self.fwButton.setAutoRepeat(False)
        self.fwButton.setAutoRepeatInterval(10)
        self.fwButton.setObjectName("fwButton")
        self.bwButton = QtWidgets.QPushButton(self.centralwidget)
        self.bwButton.setGeometry(QtCore.QRect(840, 820, 51, 41))
        self.bwButton.setObjectName("bwButton")
        self.leftButton = QtWidgets.QPushButton(self.centralwidget)
        self.leftButton.setGeometry(QtCore.QRect(770, 760, 51, 41))
        self.leftButton.setObjectName("leftButton")
        self.rightButton = QtWidgets.QPushButton(self.centralwidget)
        self.rightButton.setGeometry(QtCore.QRect(910, 760, 51, 41))
        self.rightButton.setObjectName("rightButton")
        self.streamButton = QtWidgets.QPushButton(self.centralwidget)
        self.streamButton.setGeometry(QtCore.QRect(830, 750, 71, 61))
        self.streamButton.setObjectName("streamButton")
        self.driveposButton = QtWidgets.QPushButton(self.centralwidget)
        self.driveposButton.setGeometry(QtCore.QRect(1030, 650, 71, 61))
        self.driveposButton.setObjectName("driveposButton")



#Move the robot
        self.mvfwButton = QtWidgets.QPushButton(self.centralwidget)
        self.mvfwButton.setGeometry(QtCore.QRect(440, 700, 51, 41))
        self.mvfwButton.setObjectName("mvfwButton")
        self.mvbwButton = QtWidgets.QPushButton(self.centralwidget)
        self.mvbwButton.setGeometry(QtCore.QRect(440, 820, 51, 41))
        self.mvbwButton.setObjectName("mvbwButton")
        self.mvleftButton = QtWidgets.QPushButton(self.centralwidget)
        self.mvleftButton.setGeometry(QtCore.QRect(370, 760, 51, 41))
        self.mvleftButton.setObjectName("mvleftButton")
        self.mvrightButton = QtWidgets.QPushButton(self.centralwidget)
        self.mvrightButton.setGeometry(QtCore.QRect(510, 760, 51, 41))
        self.mvrightButton.setObjectName("mvrightButton")
        self.stopButton = QtWidgets.QPushButton(self.centralwidget)
        self.stopButton.setGeometry(QtCore.QRect(430, 750, 71, 61))
        self.stopButton.setObjectName("stopButton")


        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(10, 10, 1271, 671))
        self.label.setObjectName("label")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1305, 22))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(0, 700, 71, 21))
        self.label_2.setObjectName("label_2")
        self.wifilabel = QtWidgets.QLabel(self.centralwidget)
        self.wifilabel.setGeometry(QtCore.QRect(200, 700, 71, 21))
        self.wifilabel.setObjectName("wifilabel")

#Camera Controls
        self.fwButton.pressed.connect(self.buttonpressed)
        self.fwButton.released.connect(self.buttonreleased)        
        self.bwButton.pressed.connect(self.buttonpressed)
        self.bwButton.released.connect(self.buttonreleased)        
        self.leftButton.pressed.connect(self.buttonpressed)
        self.leftButton.released.connect(self.buttonreleased)        
        self.rightButton.pressed.connect(self.buttonpressed)
        self.rightButton.released.connect(self.buttonreleased)
        self.streamButton.setCheckable(True)
        self.streamButton.toggled.connect(self.buttontoggled)

#Motor controls
        self.mvfwButton.pressed.connect(self.buttonpressed)
        self.mvfwButton.released.connect(self.buttonreleased)        
        self.mvbwButton.pressed.connect(self.buttonpressed)        
        self.mvbwButton.released.connect(self.buttonreleased)
        self.mvleftButton.pressed.connect(self.buttonpressed)        
        self.mvleftButton.released.connect(self.buttonreleased)        
        self.mvrightButton.pressed.connect(self.buttonpressed)        
        self.mvrightButton.released.connect(self.buttonreleased)        
        self.stopButton.setCheckable(True)
        self.stopButton.toggled.connect(self.buttontoggled)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 942, 22))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def getWiFiRxLevel(self):
        rxlevel = self.ImageThread.getWifiTxLevel(True)
        self.wifilabel.setText('FPS: {}'.format(rxlevel))

    def viewImage(self):
      img, fps = self.ImageThread.getImage()
#      cap = cv.VideoCapture("http://192.168.1.217/mjpeg/1")
#      ret, img = cap.read()
#      print("img = {}".format(img))
      if img is not None:
#        print("img is no None")
        img = cv2.resize(img, (self.label.size().width(), self.label.size().height()))
        # img = img[:, :, ::-1]
        height, width, channel = img.shape
        bytesPerLine = 3 * width
        self.qImg = QImage(img.data, width, height, bytesPerLine, QImage.Format_RGB888)
        pixMap = QPixmap.fromImage(self.qImg)
        self.label.setPixmap(pixMap)
        self.label_2.setText('FPS: {}'.format(fps))
        # print('label size {}'.format(self.label.size()))
        # print('image size {}'.format(self.qImg.size()))

        if DEBUG:
          cv2.imwrite('test2.jpg', self.ImageThread.getImage())

    def closeEvent(self, event):
        self.ImageThread.stop_signal.emit()
        self.ImageThread.wait()

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "ControlPanel"))

# Servo motors control
        self.fwButton.setText(_translate("MainWindow", "FW"))
        self.bwButton.setText(_translate("MainWindow", "BW"))
        self.leftButton.setText(_translate("MainWindow", "LEFT"))
        self.rightButton.setText(_translate("MainWindow", "RIGHT"))
        self.streamButton.setText(_translate("MainWindow", "STREAM"))
        self.streamButton.setShortcut(_translate("MainWindow", "P"))

# Motors control
        self.mvfwButton.setText(_translate("MainWindow", "MVFW"))
        self.mvbwButton.setText(_translate("MainWindow", "MVBW"))
        self.mvleftButton.setText(_translate("MainWindow", "MVLEFT"))
        self.mvrightButton.setText(_translate("MainWindow", "MVRIGHT"))
        self.stopButton.setText(_translate("MainWindow", "STOP"))

        self.label.setText(_translate("MainWindow", "<html><head/><body><p align=\"center\">Image View</p></body></html>"))
        self.label_2.setText(_translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt;\">FPS: </span></p></body></html>"))
    
    def buttonpressed(self):
        sender = self.sender()
        if sender.isDown():
            self.statusBar().showMessage(sender.text() + ' is pressed')        
#            print(sender.text() + ' is pressed')
            if sender.text() == 'LEFT':
                self.ImageThread.requestLeft(True)
            if sender.text() == 'RIGHT':
                self.ImageThread.requestRight(True)
            if sender.text() == 'FW':
                self.ImageThread.requestFw(True)
            if sender.text() == 'BW':
                self.ImageThread.requestBw(True)
            if sender.text() == 'MVFW':
                self.ImageThread.requestMotorFW(True)
            if sender.text() == 'MVBW':
                self.ImageThread.requestMotorBW(True)
            if sender.text() == 'MVLEFT':
                self.ImageThread.requestMotorLeft(True)
            if sender.text() == 'MVRIGHT':
                self.ImageThread.requestMotorRight(True)


    def buttonreleased(self):
        sender = self.sender()
        if not sender.isDown():
#            self.statusBar().showMessage(sender.text() + ' is released')
            print(sender.text() + ' is released')
            if sender.text() == 'LEFT':
                self.ImageThread.requestLeft(False)
            if sender.text() == 'RIGHT':
                self.ImageThread.requestRight(False)
            if sender.text() == 'FW':
                self.ImageThread.requestFw(False)
            if sender.text() == 'BW':
                self.ImageThread.requestBw(False)
            if sender.text() == 'MVFW':
                self.ImageThread.requestMotorFW(False)
            if sender.text() == 'MVBW':
                self.ImageThread.requestMotorBW(False)
            if sender.text() == 'MVLEFT':
                self.ImageThread.requestMotorLeft(False)
            if sender.text() == 'MVRIGHT':
                self.ImageThread.requestMotorRight(False)


    def buttontoggled(self):
        sender = self.sender()  # This is what you need
        if sender.isChecked():
          print(sender.text())
          if sender.text() == 'STREAM':
            print('STREAM START')
            self.streamButton.setChecked(True)
            if self.ImageThread.isPause:
              self.ImageThread.resume_signal.emit()
            else:
              self.ImageThread.start()

          self.statusBar().showMessage(sender.text() + ' is pressed')

        else:
          if sender.text() == 'STREAM':
            print('STREAM PAUSE')
            self.streamButton.setChecked(False)
            self.ImageThread.pause_signal.emit()
  
          self.statusBar().showMessage(sender.text() + ' is released')
   
    def keyPressEvent(self, e):
        if e.key() == ord('D') and not e.isAutoRepeat():
            self.ImageThread.requestMotorRight(True)
        if e.key() == ord('A') and not e.isAutoRepeat():
            self.ImageThread.requestMotorLeft(True)
        if e.key() == ord('W') and not e.isAutoRepeat():
            self.ImageThread.requestMotorFW(True)
        if e.key() == ord('S') and not e.isAutoRepeat():
            self.ImageThread.requestMotorBW(True)
        if e.key() == ord('Q') and not e.isAutoRepeat():
            self.ImageThread.requestMotorStop(True)
        if e.key() == ord('L') and not e.isAutoRepeat():
            self.ImageThread.requestRight(True)
        if e.key() == ord('J') and not e.isAutoRepeat():
            self.ImageThread.requestLeft(True)
        if e.key() == ord('K') and not e.isAutoRepeat():
            self.ImageThread.requestBw(True)
        if e.key() == ord('I') and not e.isAutoRepeat():
            self.ImageThread.requestFw(True)
        if e.key() == ord('T') and not e.isAutoRepeat():
            self.ImageThread.getWifiTxLevel(True)

    def keyReleaseEvent(self, e):
        if e.key() == ord('D') and not e.isAutoRepeat():
            self.ImageThread.requestMotorRight(False)
        if e.key() == ord('A') and not e.isAutoRepeat():
            self.ImageThread.requestMotorLeft(False)
        if e.key() == ord('W') and not e.isAutoRepeat():
            self.ImageThread.requestMotorFW(False)
        if e.key() == ord('S') and not e.isAutoRepeat():
            self.ImageThread.requestMotorBW(False)
        if e.key() == ord('Q') and not e.isAutoRepeat():
            self.ImageThread.requestMotorStop(False)
        if e.key() == ord('L') and not e.isAutoRepeat():
            self.ImageThread.requestRight(False)
        if e.key() == ord('J') and not e.isAutoRepeat():
            self.ImageThread.requestLeft(False)
        if e.key() == ord('K') and not e.isAutoRepeat():
            self.ImageThread.requestBw(False)   
        if e.key() == ord('I') and not e.isAutoRepeat():
            self.ImageThread.requestFw(False)        

