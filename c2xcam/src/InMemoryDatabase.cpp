#include <DBException.hpp>
#include <InMemoryDatabase.hpp>
#include <c2xerror.h>
#include <exception>
#include <sstream>

namespace c2x::cam {

InMemoryDatabase::InMemoryDatabase() {}

InMemoryDatabase::~InMemoryDatabase() {

  std::lock_guard<std::mutex> guard(_lock);

  for (auto it = _database.begin(); it != _database.end(); it++) {
    (*it)->lock();
    ASN_STRUCT_FREE(asn_DEF_CAM, (*it)->value());
    (*it)->unlock();
    delete *it;
  }
  _database.clear();
}

bool InMemoryDatabase::exists(long unsigned int stationID) {

  std::lock_guard<std::mutex> guard(_lock);

  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->header.stationID == stationID) {
      return true;
    }
  }

  return false;
}

DBView<CAM_t> InMemoryDatabase::get(long unsigned int stationID) {

  std::lock_guard<std::mutex> guard(_lock);

  DBElement<CAM_t> *entry = nullptr;
  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->header.stationID == stationID) {
      entry = *it;
      break;
    }
  }

  if (!entry) {
    throw DBException(ERR_MSG_NOT_FOUND);
  }

  return entry->getView();
}

DBView<CAM_t> InMemoryDatabase::create(long unsigned int stationID) {

  std::lock_guard<std::mutex> guard(_lock);

  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->header.stationID == stationID) {
      throw DBException(ERR_CAM_ALREADY_EXISTS);
    }
  }

  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->header.stationID = stationID;
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_NOTHING;

  DBElement<CAM_t> *entry = new DBElement<CAM_t>(cam);
  entry->modifiedCallback = [this](DBElement<CAM_t> *entry) {
    this->entryModified(entry);
  };
  _database.push_back(entry);

  return entry->getView();
}

void InMemoryDatabase::remove(long unsigned int stationID) {

  std::lock_guard<std::mutex> guard(_lock);

  std::vector<DBElement<CAM_t> *>::iterator iterator;
  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->header.stationID == stationID) {
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

void InMemoryDatabase::entryModified(DBElement<CAM_t> *entry) {

  std::lock_guard<std::mutex> guard(_lock);

  bool overrideEntry = false;
  std::vector<DBElement<CAM_t> *>::iterator iterator;
  for (auto it = _database.begin(); it != _database.end(); it++) {
    if ((*it)->value()->header.stationID == entry->value()->header.stationID &&
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
    std::vector<DBElement<CAM_t> *>::iterator iterator) {
  ASN_STRUCT_FREE(asn_DEF_CAM, (*iterator)->value());
  delete *iterator;
  _database.erase(iterator);
}

}; // namespace c2x::cam