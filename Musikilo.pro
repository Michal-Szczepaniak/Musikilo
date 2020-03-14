TEMPLATE = subdirs

Musikilo.depends = qtmpris/src qwebdavlib/qwebdavlib libqofonoext/src qtdbusextended/src

SUBDIRS += \
    Musikilo \
    qwebdavlib/qwebdavlib \
    qtmpris/src \
    libqofonoext/src \
    qtdbusextended/src

OTHER_FILES += rpm/Musikilo.spec
