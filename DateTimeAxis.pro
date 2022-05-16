 QT += charts

  SOURCES += \
      main.cpp

  RESOURCES += \
      sundata.qrc \
    sundata.qrc

  target.path = $$[QT_INSTALL_EXAMPLES]/charts/datetimeaxis
  INSTALLS += target
