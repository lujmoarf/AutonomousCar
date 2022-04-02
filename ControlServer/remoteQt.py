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
#        self.mvfwButton.setAutoRepeat(True)
#        self.mvfwButton.setAutoRepeatInterval(10)
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

        self.fwButton.setCheckable(True)
        self.fwButton.toggled.connect(self.buttontoggled)
        self.bwButton.setCheckable(True)
        self.bwButton.toggled.connect(self.buttontoggled)
        self.leftButton.setCheckable(True)
        self.leftButton.toggled.connect(self.buttontoggled)
        self.rightButton.setCheckable(True)
        self.rightButton.toggled.connect(self.buttontoggled)
        self.streamButton.setCheckable(True)
        self.streamButton.toggled.connect(self.buttontoggled)

        self.mvfwButton.setCheckable(True)
        self.mvfwButton.toggled.connect(self.buttontoggled)
        self.mvbwButton.setCheckable(True)
        self.mvbwButton.toggled.connect(self.buttontoggled)
        self.mvleftButton.setCheckable(True)
        self.mvleftButton.toggled.connect(self.buttontoggled)
        self.mvrightButton.setCheckable(True)
        self.mvrightButton.toggled.connect(self.buttontoggled)
#        self.mvrightButton.pressed.connect(self.onkeyPressEvent)
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

    def viewImage(self):
      img, fps = self.ImageThread.getImage()
#      cap = cv.VideoCapture("http://192.168.100.157/mjpeg/1")
#      ret, img = cap.read()
#      print("img = {}".format(img))
      if img is not None:
        print("img is no None")
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
        self.fwButton.setShortcut(_translate("MainWindow", "I"))
        self.bwButton.setText(_translate("MainWindow", "BW"))
        self.bwButton.setShortcut(_translate("MainWindow", "K"))
        self.leftButton.setText(_translate("MainWindow", "LEFT"))
        self.leftButton.setShortcut(_translate("MainWindow", "J"))
        self.rightButton.setText(_translate("MainWindow", "RIGHT"))
        self.rightButton.setShortcut(_translate("MainWindow", "l"))
        self.streamButton.setText(_translate("MainWindow", "STREAM"))
        self.streamButton.setShortcut(_translate("MainWindow", "P"))

# Motors control
        self.mvfwButton.setText(_translate("MainWindow", "MVFW"))
#        self.mvfwButton.setShortcut(_translate("MainWindow", "W"))
        self.mvbwButton.setText(_translate("MainWindow", "MVBW"))
#        self.mvbwButton.setShortcut(_translate("MainWindow", "S"))
        self.mvleftButton.setText(_translate("MainWindow", "MVLEFT"))
#        self.mvleftButton.setShortcut(_translate("MainWindow", "A"))
        self.mvrightButton.setText(_translate("MainWindow", "MVRIGHT"))
 #       self.mvrightButton.setShortcut(_translate("MainWindow", "D"))
        self.stopButton.setText(_translate("MainWindow", "STOP"))
#        self.stopButton.setShortcut(_translate("MainWindow", "Q"))

        self.label.setText(_translate("MainWindow", "<html><head/><body><p align=\"center\">Image View</p></body></html>"))
        self.label_2.setText(_translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt;\">FPS: </span></p></body></html>"))
    
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
          if sender.text() == 'LEFT':
            self.ImageThread.requestLeft(True)
          if sender.text() == 'RIGHT':
            self.ImageThread.requestRight(True)
          if sender.text() == 'FW':
            self.ImageThread.requestFw(True)
          if sender.text() == 'BW':
            self.ImageThread.requestBw(True)
          if sender.text() == 'STOP':
            self.ImageThread.requestMotorStop(True)
          if sender.text() == 'MVFW':
            self.ImageThread.requestMotorFW(True)
            self.stopButton.setChecked(False)
          if sender.text() == 'MVBW':
            self.ImageThread.requestMotorBW(True)
            self.stopButton.setChecked(False)


          self.statusBar().showMessage(sender.text() + ' is pressed')

        else:
          if sender.text() == 'STREAM':
            print('STREAM PAUSE')
            self.streamButton.setChecked(False)
            self.ImageThread.pause_signal.emit()
          if sender.text() == 'LEFT':
            self.ImageThread.requestLeft(False)
          if sender.text() == 'RIGHT':
            self.ImageThread.requestRight(False)
          if sender.text() == 'FW':
            self.ImageThread.requestFw(False)
          if sender.text() == 'BW':
            self.ImageThread.requestBw(False)
          if sender.text() == 'STOP':
            self.ImageThread.requestMotorGo(True)
          if sender.text() == 'MVFW':
            self.stopButton.setChecked(True)
          if sender.text() == 'MVBW':
            self.stopButton.setChecked(True)
  
          self.statusBar().showMessage(sender.text() + ' is released')
   
    def keyPressEvent(self, e):
        if e.key() == ord('D') and not e.isAutoRepeat():
#            print('D Pressed')
            self.ImageThread.requestMotorRight(True)
        if e.key() == ord('A') and not e.isAutoRepeat():
            self.ImageThread.requestMotorLeft(True)
        if e.key() == ord('W') and not e.isAutoRepeat():
            self.ImageThread.requestMotorFW(True)
        if e.key() == ord('S') and not e.isAutoRepeat():
            self.ImageThread.requestMotorBW(True)
        if e.key() == ord('Q') and not e.isAutoRepeat():
            self.ImageThread.requestMotorStop(True)

    def keyReleaseEvent(self, e):
        if e.key() == ord('D') and not e.isAutoRepeat():
#            print('D released')
            self.ImageThread.requestMotorRight(False)
        if e.key() == ord('A') and not e.isAutoRepeat():
            self.ImageThread.requestMotorLeft(False)
        if e.key() == ord('W') and not e.isAutoRepeat():
            self.ImageThread.requestMotorFW(False)
        if e.key() == ord('S') and not e.isAutoRepeat():
            self.ImageThread.requestMotorBW(False)
        if e.key() == ord('Q') and not e.isAutoRepeat():
            self.ImageThread.requestMotorStop(False)
        

    def onkeyPressEvent(self,event):
        if event.key() == ord('I') and not event.isAutoRepeat():
          self.fwButton.setChecked(True);
          print('I is pressed');
        if event.key() == ord('K') and not event.isAutoRepeat():
          self.bwButton.setChecked(True)
        if event.key() == ord('J') and not event.isAutoRepeat():
          self.leftButton.setChecked(True)
        if event.key() == ord('L') and not event.isAutoRepeat():
          self.rightButton.setChecked(True)
        if not event.isAutoRepeat():
          print(event.key())
        else:
          print(event.key())

    def onkeyReleaseEvent(self, event):
        if event.key() == ord('I') and not event.isAutoRepeat():
          self.fwButton.setChecked(False)
        if event.key() == ord('K') and not event.isAutoRepeat():
          self.bwButton.setChecked(False)
        if event.key() == ord('J') and not event.isAutoRepeat():
          self.leftButton.setChecked(False)
        if event.key() == ord('L') and not event.isAutoRepeat():
          self.rightButton.setChecked(False)
        if not event.isAutoRepeat():
          print(event.key())

