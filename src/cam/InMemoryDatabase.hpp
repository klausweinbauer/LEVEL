/**
 * @file InMemoryDatabase.hpp
 * @author Klaus Weinbauer
 * @brief Database implementation for an in-memory storage of CAMs.
 * @version 0.1
 * @date 2022-06-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <IDatabase.hpp>
#include <mutex>
#include <vector>

namespace level::cam {

/**
 * @brief Database implementation for an in-memory storage of CAMs.
 *
 * @details This class is a straightforward database implementation to store
 * CAMs. The underlying data structure is a std::vector and, therefore, not the
 * optimal solution for fast index accessing. This could definitely be improved.
 * The reason for this basic approach is that the id for the stored object is
 * part of the object itself, making it tricky to store these objects in
 * key-value data structures.
 */
class InMemoryDatabase : public IDatabase {
private:
  std::vector<DBElement<CAMData> *> _database;
  std::mutex _lock;

  void entryModified(DBElement<CAMData> *entry);
  void deleteEntry(std::vector<DBElement<CAMData> *>::iterator iterator);

public:
  InMemoryDatabase();
  ~InMemoryDatabase();

  InMemoryDatabase(const InMemoryDatabase &other) = delete;
  InMemoryDatabase &operator=(const InMemoryDatabase &other) = delete;

  virtual bool exists(long unsigned int stationID) override;
  virtual DBView<CAMData> get(long unsigned int stationID) override;
  virtual DBView<CAMData> create(long unsigned int stationID) override;
  virtual void remove(long unsigned int stationID) override;
  virtual size_t entryCount() override;
};

} // namespace level::cam