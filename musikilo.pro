TEMPLATE = subdirs

musikilo.depends = qwebdavlib/qwebdavlib smpc

SUBDIRS += \
    musikilo \
    qwebdavlib/qwebdavlib \
    smpc

OTHER_FILES += rpm/musikilo.spec
