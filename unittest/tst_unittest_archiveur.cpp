#include <QtTest>

// add necessary includes here
#include "c_document.h"
#include "c_tag.h"
#include "c_dbmanager.h"

class unittest_archiveur : public QObject {
    Q_OBJECT

public:
    unittest_archiveur();
    ~unittest_archiveur();

private:
    c_dbManager database_manager;
private slots:
    void test_tag();
    void test_document();
    void test_add_tag();
    void test_remove_tag();
    void test_add_remove_document();
};

unittest_archiveur::unittest_archiveur() {
    database_manager.open(QString("../../bdd/database.db"));
}

unittest_archiveur::~unittest_archiveur() {
}

void unittest_archiveur::test_tag()
{
    c_tag tag_1(QString("test_tag_1"),QColor(0,0,0,0));
    c_tag tag_2(tag_1);
    c_tag tag_3 = tag_1;
    tag_3.set_name(QString("Tag_3"));
    tag_3.set_color(QColor(1,1,1,0));

    QCOMPARE(tag_1.get_name(), QString("test_tag_1"));
    QCOMPARE(tag_1.get_color(), QColor(0,0,0,0));
    QCOMPARE(tag_1.get_name(),tag_2.get_name());
    QCOMPARE(tag_1.get_color(),tag_2.get_color());
    QCOMPARE(tag_3.get_name(), QString("tag_3"));
    QVERIFY(tag_3.get_name() != QString("Tag_3"));
    QCOMPARE(tag_3.get_color(), QColor(1,1,1,0));
}

void unittest_archiveur::test_document() {
    c_tag tag_1(QString("test_tag_1"),QColor(0,0,0,0));
    c_tag tag_2(tag_1);
    c_tag tag_3 = tag_1;
    c_tag tag_4(QString("essaie_tag_1"),QColor(0,0,0,0));
    QVector<c_tag> tag_list;
    QVector<c_tag> rigth_tag_list;
    tag_list.push_back(tag_1);
    tag_list.push_back(tag_2);
    tag_list.push_back(tag_3);
    tag_list.push_back(tag_4);
    rigth_tag_list.push_back(tag_4);
    rigth_tag_list.push_back(tag_1);
    c_document doc_1(QString("Doc1"),QString("c:/"),tag_list);

    QCOMPARE(doc_1.get_name(),QString("Doc1"));
    QCOMPARE(doc_1.get_path(),QString("c:/"));
    QVERIFY(doc_1.get_tag_list() != tag_list);
    QCOMPARE(doc_1.get_tag_list(),rigth_tag_list);
    QCOMPARE(doc_1.get_date(),QDate::currentDate());
}

void unittest_archiveur::test_add_tag() {
    c_tag new_tag_u(QString("Test_2"),QColor(1,1,1));
    c_tag new_tag_l(QString("test_2"),QColor(1,1,1));
    QVERIFY(database_manager.add_tag(new_tag_u));
    QCOMPARE(new_tag_l,database_manager.get_tag(new_tag_l.get_name()));
    QVERIFY(!database_manager.add_tag(new_tag_l));
}

void unittest_archiveur::test_remove_tag() {
    QVERIFY(database_manager.remove_tag(QString("test_2")));
    QCOMPARE(c_tag(),database_manager.get_tag(QString("test_2")));
    QVERIFY(!database_manager.remove_tag(QString("test_2")));
}

void unittest_archiveur::test_add_remove_document() {
    QVector<c_tag> tag_list;
    c_tag tag_1 = c_tag(QString("tag_1"),QColor(1,1,1));
    c_tag tag_2 = c_tag(QString("tag_2"),QColor(1,1,1));
    c_tag tag_3 = c_tag(QString("tag_3"),QColor(1,1,1));
    tag_list.push_back(tag_1);
    tag_list.push_back(tag_2);
    tag_list.push_back(tag_3);
    c_document doc_1(QString("doc1"),QString("c:/test_add_document/doc1"),tag_list);
    QVERIFY(database_manager.add_document(doc_1));
    QCOMPARE(tag_1,database_manager.get_tag("tag_1"));
    QCOMPARE(tag_2,database_manager.get_tag("tag_2"));
    QCOMPARE(tag_3,database_manager.get_tag("tag_3"));
    QCOMPARE(doc_1,database_manager.get_document(QString("c:/test_add_document/doc1")));
    QVERIFY(database_manager.remove_document(QString("c:/test_add_document/doc1")));
    QCOMPARE(c_document(),database_manager.get_document(QString("c:/test_add_document/doc1")));
    QVERIFY(!database_manager.remove_document(QString("c:/test_add_document/doc1")));
}

QTEST_APPLESS_MAIN(unittest_archiveur)

#include "tst_unittest_archiveur.moc"
