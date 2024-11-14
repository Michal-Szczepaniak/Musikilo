TEMPLATE = subdirs

musikilo.depends = qwebdavlib/qwebdavlib

SUBDIRS += \
    musikilo \
    qwebdavlib/qwebdavlib

OTHER_FILES += rpm/musikilo.spec
