TEMPLATE = subdirs

Musikilo.depends = qtmpris/src qwebdavlib/qwebdavlib

SUBDIRS += \
    qwebdavlib/qwebdavlib \
    Musikilo \
    qtmpris/src

OTHER_FILES += rpm/Musikilo.spec
