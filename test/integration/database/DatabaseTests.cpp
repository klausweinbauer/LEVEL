#include <Database.hpp>
#include <IDXIndex.hpp>
#include <QRYIndex.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

using namespace level;

struct Database_Data {
  int _value;

  Database_Data() : _value(0) {}
  Database_Data(int value) : _value(value) {}
};

void Database_HeavyQueryingTask(Database<Database_Data> *db, int size,
                                int runs) {
  for (int i = 0; i < runs; i++) {
    unsigned int index1 = ((unsigned int)rand()) % size;
    unsigned int index2 = ((unsigned int)rand()) % size;
    if (index2 < index1) {
      auto tmp = index2;
      index2 = index1;
      index1 = tmp;
    }
    auto result = db->get(QRYIndex::byRange(index1, index2));
    for (auto &&view : result) {
      ASSERT_EQ(view.index(), view->_value);
    }
  }
}

TEST(Database, HeavyQuerying) {
  Database<Database_Data> db;
  db.addIndexer(std::make_unique<IDXIndexer<Database_Data>>());
  const int size = 10;
  for (int i = 0; i < size; i++) {
    db.insert({i});
  }

  const int threadCount = 5;
  const int threadRuns = 1000;
  std::thread threads[threadCount];
  for (int i = 0; i < threadCount; i++) {
    threads[i] = std::thread(Database_HeavyQueryingTask, &db, size, threadRuns);
  }
  for (int i = 0; i < threadCount; i++) {
    threads[i].join();
  }
}

void Database_HeavyWritingTask(Database<Database_Data> *db, int propWrite,
                               int runs) {
  for (int i = 0; i < runs; i++) {
    int prop = rand() % 100;
    if (prop <= propWrite) {
      db->insert({rand()});
    } else {
      auto results = db->get(QRYIndex::byRange(0, runs));
      if (results.size() > 0) {
        db->remove(*results.begin());
      }
    }
  }
}

TEST(Database, HeavyWriting) {
  Database<Database_Data> db;
  db.addIndexer(std::make_unique<IDXIndexer<Database_Data>>());

  const int threadCount = 10;
  const int threadRuns = 100;
  const int propWrite = 60;
  std::thread threads[threadCount];
  for (int i = 0; i < threadCount; i++) {
    threads[i] =
        std::thread(Database_HeavyWritingTask, &db, propWrite, threadRuns);
  }
  for (int i = 0; i < threadCount; i++) {
    threads[i].join();
  }
}

void Database_HeavyUpdatingTask(Database<Database_Data> *db, int threadCount,
                                int runs) {
  int addCounter = 0;
  while (addCounter < runs) {
    unsigned int index = ((unsigned int)rand()) % threadCount;
    auto view = db->get(QRYIndex::byIndex(index));
    assert(view.size() == 1);
    if (view[0]->_value < runs) {
      view[0]->_value++;
      addCounter++;
    }
  }
}

TEST(Database, HeavyUpdating) {
  Database<Database_Data> db;
  db.addIndexer(std::make_unique<IDXIndexer<Database_Data>>());
  const int threadCount = 10;
  for (int i = 0; i < threadCount; i++) {
    db.insert(Database_Data());
  }

  const int threadRuns = 1000;
  std::thread threads[threadCount];
  for (int i = 0; i < threadCount; i++) {
    threads[i] =
        std::thread(Database_HeavyUpdatingTask, &db, threadCount, threadRuns);
  }

  for (int i = 0; i < threadCount; i++) {
    threads[i].join();
  }

  auto views = db.get(QRYIndex::byRange(0, threadCount));
  for (auto &&view : views) {
    EXPECT_EQ(threadRuns, view->_value);
  }
}