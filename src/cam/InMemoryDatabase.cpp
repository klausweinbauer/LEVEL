#include <DBException.hpp>
#include <InMemoryDatabase.hpp>
#include <exception>
#include <level_error.h>
#include <sstream>

namespace level::cam {

InMemoryDatabase::InMemoryDatabase() {}

InMemoryDatabase::~InMemoryDatabase() {

  std::lock_guard<std::mutex> guard(_lock);

  for (auto it = _database.begin(); it != _database.end(); it++) {
    (*it)->lock();
    delete (*it)->value();
    (*it)->unlock();
    delete *it;
  }
  _database.clear();
}

bool InMemoryDatabase::exists(long unsigned int stationID) {

  std::lock_guard<std::mutex> guard(_lock);

  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->cam()->header.stationID == stationID) {
      return true;
    }
  }

  return false;
}

DBView<CAMData> InMemoryDatabase::get(long unsigned int stationID) {

  std::lock_guard<std::mutex> guard(_lock);

  DBElement<CAMData> *entry = nullptr;
  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->cam()->header.stationID == stationID) {
      entry = *it;
      break;
    }
  }

  if (!entry) {
    throw DBException(ERR_MSG_NOT_FOUND);
  }

  return entry->getView();
}

DBView<CAMData> InMemoryDatabase::create(long unsigned int stationID) {

  std::lock_guard<std::mutex> guard(_lock);

  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->cam()->header.stationID == stationID) {
      throw DBException(ERR_CAM_ALREADY_EXISTS);
    }
  }

  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->header.stationID = stationID;
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_NOTHING;

  CAMData *camData = new CAMData(cam);
  DBElement<CAMData> *entry = new DBElement<CAMData>(camData);
  entry->modifiedCallback = [this](DBElement<CAMData> *entry) {
    this->entryModified(entry);
  };
  _database.push_back(entry);

  return entry->getView();
}

void InMemoryDatabase::remove(long unsigned int stationID) {

  std::lock_guard<std::mutex> guard(_lock);

  std::vector<DBElement<CAMData> *>::iterator iterator;
  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->cam()->header.stationID == stationID) {
      iterator = it;
      break;
    }
  }

  if (iterator != _database.end()) {
    deleteEntry(iterator);
  }
}

size_t InMemoryDatabase::entryCount() {

  std::lock_guard<std::mutex> guard(_lock);
  return _database.size();
}

void InMemoryDatabase::entryModified(DBElement<CAMData> *entry) {

  std::lock_guard<std::mutex> guard(_lock);

  bool overrideEntry = false;
  std::vector<DBElement<CAMData> *>::iterator iterator;
  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->cam()->header.stationID ==
            entry->value()->cam()->header.stationID &&
        *it != entry) {
      iterator = it;
      overrideEntry = true;
      break;
    }
  }

  if (overrideEntry) {
    deleteEntry(iterator);
  }

  entry->clearModifiedFlag();
}

void InMemoryDatabase::deleteEntry(
    std::vector<DBElement<CAMData> *>::iterator iterator) {
  delete (*iterator)->value();
  delete *iterator;
  _database.erase(iterator);
}

}; // namespace level::cam