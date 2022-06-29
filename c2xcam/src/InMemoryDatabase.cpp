#include <DBException.hpp>
#include <InMemoryDatabase.hpp>
#include <c2xerror.h>
#include <exception>
#include <sstream>

namespace c2x::cam {

InMemoryDatabase::InMemoryDatabase() {}

InMemoryDatabase::~InMemoryDatabase() {}

bool InMemoryDatabase::exists(int id) {

  _lock.lock();

  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->header.stationID == id) {
      _lock.unlock();
      return true;
    }
  }

  _lock.unlock();

  return false;
}

DBView<CAM_t> InMemoryDatabase::get(int id) {

  _lock.lock();

  DBElement<CAM_t> *entry = nullptr;
  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->header.stationID == id) {
      entry = *it;
      break;
    }
  }

  if (!entry) {
    _lock.unlock();
    throw DBException(ERR_MSG_NOT_FOUND);
  }

  DBView<CAM_t> view = entry->getView();

  _lock.unlock();

  return view;
}

DBView<CAM_t> InMemoryDatabase::create(int id) {

  _lock.lock();

  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->header.stationID == id) {
      _lock.unlock();
      throw DBException(ERR_CAM_ALREADY_EXISTS);
    }
  }

  CAM_t *cam = new CAM_t();
  DBElement<CAM_t> *entry = new DBElement<CAM_t>(cam);
  _database.push_back(entry);
  auto view = entry->getView();

  _lock.unlock();

  view->header.stationID = id;
  view->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_NOTHING;

  return view;
}

void InMemoryDatabase::remove(int id) {

  _lock.lock();

  std::vector<DBElement<CAM_t> *>::iterator iterator;
  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->header.stationID == id) {
      iterator = it;
      break;
    }
  }

  if (iterator != _database.end()) {
    _database.erase(iterator);
  }

  _lock.unlock();
}

size_t InMemoryDatabase::entryCount() {

  _lock.lock();

  size_t len = _database.size();

  _lock.unlock();

  return len;
}

}; // namespace c2x::cam