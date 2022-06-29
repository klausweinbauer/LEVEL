#pragma once

#include <IDatabase.hpp>
#include <mutex>
#include <vector>

namespace c2x::cam {

class InMemoryDatabase : public IDatabase {
private:
  std::vector<DBElement<CAM_t> *> _database;
  std::mutex _lock;

public:
  InMemoryDatabase();
  ~InMemoryDatabase();

  virtual bool exists(int id);
  virtual DBView<CAM_t> get(int id);
  virtual DBView<CAM_t> create(int id);
  virtual void remove(int id);
  virtual size_t entryCount();
};

} // namespace c2x::cam