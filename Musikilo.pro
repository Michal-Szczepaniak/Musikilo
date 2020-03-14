TEMPLATE = subdirs

Musikilo.depends = qtmpris/src qwebdavlib/qwebdavlib libqofonoext/src

SUBDIRS += \
    Musikilo \
    qwebdavlib/qwebdavlib \
    qtmpris/src \
    libqofonoext/src

OTHER_FILES += rpm/Musikilo.spec
