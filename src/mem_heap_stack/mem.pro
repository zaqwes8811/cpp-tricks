TARGET = a.app
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += ./alloc_app.cc #\
  #../../third_party/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
  #../launcher.cc

INCLUDEPATH += /home/zaqwes/work/third_party/source_release \
  /home/zaqwes/work/third_party/boost_1_55_0
  #../../third_party/gmock-1.6.0/fused-src/

# -ltbb не достаточно
LIBS += -lpthread -lrt -ltbbmalloc -ltbb