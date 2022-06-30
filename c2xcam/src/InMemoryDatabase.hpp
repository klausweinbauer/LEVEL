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

  void entryModified(DBElement<CAM_t> *entry);
  void deleteEntry(std::vector<DBElement<CAM_t> *>::iterator iterator);

public:
  InMemoryDatabase();
  ~InMemoryDatabase();

  virtual bool exists(long unsigned int id) override;
  virtual DBView<CAM_t> get(long unsigned int id) override;
  virtual DBView<CAM_t> create(long unsigned int id) override;
  virtual void remove(long unsigned int id) override;
  virtual size_t entryCount() override;
};

} // namespace c2x::cam