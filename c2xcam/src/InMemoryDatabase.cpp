#include <InMemoryDatabase.hpp>
#include <exception>

namespace c2x::cam {

InMemoryDatabase::InMemoryDatabase() {}

InMemoryDatabase::~InMemoryDatabase() {}

DBView<int, double> InMemoryDatabase::getById(int id) {
  throw std::exception();
}

DBView<int, double> InMemoryDatabase::create(int id) {
  _lock.lock();

  double *value = new double(3);
  DBElement<int, double> *entry = new DBElement<int, double>(id, value);

  _lock.unlock();

  return entry->getView();
}

}; // namespace c2x::cam