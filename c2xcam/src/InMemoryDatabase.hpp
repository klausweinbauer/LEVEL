#pragma once

#include <IDatabase.hpp>
#include <mutex>
#include <vector>

namespace c2x::cam {

class IDatabase;

class InMemoryDatabase : public IDatabase {
private:
  std::vector<DBElement<CAM_t> *> _database;
  std::mutex _lock;

public:
  InMemoryDatabase();
  ~InMemoryDatabase();

  virtual bool exists(int id) override;
  virtual DBView<CAM_t> get(int id) override;
  virtual DBView<CAM_t> create(int id) override;
  virtual void remove(int id) override;
  virtual size_t entryCount() override;
};

} // namespace c2x::cam