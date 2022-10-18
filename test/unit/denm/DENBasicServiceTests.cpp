#include <DENBasicService.hpp>
#include <DENMIndexer.hpp>
#include <Mocks.hpp>
#include <QRYLatestMsg.hpp>
#include <QRYStationID.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

using namespace level;
using namespace level::denm;

namespace level::DENBasicServiceTests {

float randFloat() { return rand() * 3.1415962535; }

std::shared_ptr<NiceMock<MNetworkInterface<DENM>>> getNAL() {
  return std::make_shared<NiceMock<MNetworkInterface<DENM>>>();
}

std::shared_ptr<NiceMock<MPOTI>> getPOTI() {
  return std::make_shared<NiceMock<MPOTI>>();
}

std::shared_ptr<NiceMock<MDatabase<DENMWrapper>>> getDB() {
  return std::make_shared<NiceMock<MDatabase<DENMWrapper>>>();
}

std::shared_ptr<DENBasicService>
getService(std::shared_ptr<INetworkInterface<DENM>> networkInterface,
           std::shared_ptr<IPOTI> poti,
           std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase) {
  return std::make_shared<DENBasicService>(networkInterface, poti,
                                           denmDatabase);
}

std::shared_ptr<DENBasicService> getService() {
  return std::make_shared<DENBasicService>(getNAL(), getPOTI(), getDB());
}

std::shared_ptr<DENBasicService>
getService(std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase) {
  return std::make_shared<DENBasicService>(getNAL(), getPOTI(), denmDatabase);
}

} // namespace level::DENBasicServiceTests

using namespace level::DENBasicServiceTests;

TEST(DENBasicService, TryGetDENMFromDatabase) {
  auto db = getDB();
  auto service = getService(db);
  auto stationId = (unsigned int)rand();
  auto sequenceNumber = rand();
  DBElement<DENMWrapper> element;
  element.setData(std::make_unique<DENMWrapper>());
  element.data()->header.stationID = stationId;
  element.data()->denm.management.actionID.sequenceNumber = sequenceNumber;
  DBView<DENMWrapper> view(&element);

  EXPECT_CALL(*db, get(_))
      .WillOnce(Invoke([&view, stationId](std::shared_ptr<IQuery> q) {
        auto cQuery = dynamic_cast<QRYLatestMsg *>(q.get());
        std::vector<DBView<DENMWrapper>> views;
        if (cQuery && cQuery->stationID == stationId) {
          views.push_back(std::move(view));
        }
        return views;
      }));

  DENMWrapper denm;
  ActionId_t actionId = {stationId, sequenceNumber};
  EXPECT_TRUE(service->tryGetDENM(actionId, &denm));
  EXPECT_EQ(actionId.stationID, denm->header.stationID);
  EXPECT_EQ(actionId.sequenceNumber,
            denm->denm.management.actionID.sequenceNumber);
}

TEST(DENBasicService, TryGetDENMFromDatabaseWithMultipleSequencNumbers) {
  auto db = getDB();
  auto service = getService(db);
  auto stationId = (unsigned int)rand();
  auto sequenceNumber = rand();

  // 1. Element
  DBElement<DENMWrapper> element1;
  element1.setData(std::make_unique<DENMWrapper>());
  element1.data()->header.stationID = stationId;
  element1.data()->denm.management.actionID.sequenceNumber = sequenceNumber + 1;
  DBView<DENMWrapper> view1(&element1);

  // 2. Element
  DBElement<DENMWrapper> element2;
  element2.setData(std::make_unique<DENMWrapper>());
  element2.data()->header.stationID = stationId;
  element2.data()->denm.management.actionID.sequenceNumber = sequenceNumber;
  DBView<DENMWrapper> view2(&element2);

  EXPECT_CALL(*db, get(_))
      .WillOnce(Invoke([&view1, &view2, stationId](std::shared_ptr<IQuery> q) {
        auto cQuery = dynamic_cast<QRYLatestMsg *>(q.get());
        std::vector<DBView<DENMWrapper>> views;
        if (cQuery && cQuery->stationID == stationId) {
          views.push_back(std::move(view1));
          views.push_back(std::move(view2));
        }
        return views;
      }));

  DENMWrapper denm;
  ActionId_t actionId = {stationId, sequenceNumber};
  EXPECT_TRUE(service->tryGetDENM(actionId, &denm));
  EXPECT_EQ(actionId.stationID, denm->header.stationID);
  EXPECT_EQ(actionId.sequenceNumber,
            denm->denm.management.actionID.sequenceNumber);
}

TEST(DENBasicService, TryGetNotExistingDENMFromDatabase) {
  auto db = getDB();
  auto service = getService(db);
  auto stationId = (unsigned int)rand();
  auto sequenceNumber = rand();

  EXPECT_CALL(*db, get(_)).WillOnce(Invoke([](std::shared_ptr<IQuery> q) {
    return std::vector<DBView<DENMWrapper>>();
  }));

  DENMWrapper denm;
  ActionId_t actionId = {stationId, sequenceNumber};
  EXPECT_FALSE(service->tryGetDENM(actionId, &denm));
}