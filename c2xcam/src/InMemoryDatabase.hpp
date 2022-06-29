#pragma once

#include <IDatabase.hpp>
#include <map>
#include <mutex>

namespace c2x::cam {

class InMemoryDatabase : public IDatabase {
private:
  std::map<int, DBElement<int, double> *> _database;
  std::mutex _lock;

public:
  InMemoryDatabase();
  ~InMemoryDatabase();

  virtual DBView<int, double> getById(int id);
  virtual DBView<int, double> create(int id);
};

} // namespace c2x::cam