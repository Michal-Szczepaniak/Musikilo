TEMPLATE = subdirs

musikilo.depends = qwebdavlib/qwebdavlib smpc jcon-cpp/src/jcon

SUBDIRS += \
    musikilo \
    qwebdavlib/qwebdavlib \
    smpc \
    jcon-cpp/src/jcon

OTHER_FILES += rpm/musikilo.spec
