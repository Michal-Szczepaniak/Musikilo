TEMPLATE = subdirs

musikilo.depends = qwebdavlib/qwebdavlib smpc

SUBDIRS += \
    musikilo \
    qwebdavlib/qwebdavlib \
    smpc \
    QJsonRPC

OTHER_FILES += rpm/musikilo.spec
