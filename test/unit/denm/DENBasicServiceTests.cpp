#include <DENBasicService.hpp>
#include <DENMIndexer.hpp>
#include <Database.hpp>
#include <Mocks.hpp>
#include <QRYLatestMsg.hpp>
#include <QRYStationID.hpp>
#include <ValueConverter.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::An;
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

std::shared_ptr<NiceMock<MValueConverter>> getConverter() {
  return std::make_shared<NiceMock<MValueConverter>>();
}

std::shared_ptr<DENBasicService> getService() {
  return std::make_shared<DENBasicService>(getNAL(), getPOTI(), getDB(),
                                           getConverter());
}

std::shared_ptr<DENBasicService>
getService(std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase) {
  return std::make_shared<DENBasicService>(getNAL(), getPOTI(), denmDatabase,
                                           getConverter());
}

std::shared_ptr<DENBasicService>
getService(std::shared_ptr<IPOTI> poti,
           std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase) {
  return std::make_shared<DENBasicService>(getNAL(), poti, denmDatabase,
                                           getConverter());
}

std::shared_ptr<DENBasicService>
getService(std::shared_ptr<INetworkInterface<DENM>> nal,
           std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase) {
  return std::make_shared<DENBasicService>(nal, getPOTI(), denmDatabase,
                                           getConverter());
}

std::shared_ptr<DENBasicService>
getService(std::shared_ptr<IPOTI> poti,
           std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase,
           std::shared_ptr<IValueConverter> valueConverter) {
  return std::make_shared<DENBasicService>(getNAL(), poti, denmDatabase,
                                           valueConverter);
}

std::shared_ptr<DENBasicService>
getService(std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase,
           std::shared_ptr<NiceMock<MNetworkInterface<DENM>>> nal,
           std::shared_ptr<NiceMock<MPOTI>> poti) {
  return std::make_shared<DENBasicService>(nal, poti, denmDatabase,
                                           getConverter());
}

std::shared_ptr<DENBasicService>
getService(std::shared_ptr<INetworkInterface<DENM>> networkInterface,
           std::shared_ptr<IPOTI> poti,
           std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase) {
  return std::make_shared<DENBasicService>(networkInterface, poti, denmDatabase,
                                           getConverter());
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

TEST(DENBasicService, SetAndGetConfiguration) {
  auto service = getService();
  DENBasicServiceConfig config;
  config.stationID = (unsigned int)rand();
  config.stationType = StationType_PassengerCar;
  service->configure(config);
  auto result = service->getConfiguration();
  ASSERT_EQ(config.stationID, result.stationID);
  ASSERT_EQ(config.stationType, result.stationType);
}

TEST(DENBasicService, UseNewActionId) {
  DBElement<DENMWrapper> elmt;
  auto db = getDB();
  auto service = getService(db);
  EXPECT_CALL(*db, insert(An<DENMWrapper>()))
      .WillRepeatedly(Invoke([&elmt](DENMWrapper entry) {
        elmt.setData(std::make_unique<DENMWrapper>(entry));
        DBView<DENMWrapper> view(&elmt);
        return view;
      }));

  auto id1 = service->createDENM(nullptr);
  auto id2 = service->createDENM(nullptr);

  ASSERT_EQ(id1.stationID, id2.stationID);
  ASSERT_NE(id1.sequenceNumber, id2.sequenceNumber);
}

TEST(DENBasicService, InsertNewDENMIntoDatabase) {
  auto converter = std::make_shared<ValueConverter>();
  DBElement<DENMWrapper> elmt;
  auto db = getDB();
  auto poti = getPOTI();
  auto service = getService(poti, db, converter);
  auto now = (unsigned long long int)rand();
  auto eventType = EventType_CollisionRisk;
  DENMWrapper denm;
  EXPECT_CALL(*poti, now()).WillRepeatedly(Return(now));
  EXPECT_CALL(*db, insert(An<DENMWrapper>()))
      .WillOnce(Invoke([&denm, &elmt](DENMWrapper entry) {
        denm = entry;
        elmt.setData(std::make_unique<DENMWrapper>(entry));
        DBView<DENMWrapper> view(&elmt);
        return view;
      }));

  service->createDENM(&eventType);

  ASSERT_NE(nullptr, denm->denm.situation);
  ASSERT_EQ(eventType, (EventType)denm->denm.situation->eventType.causeCode);
  ASSERT_EQ(now,
            converter->itsToSITimestamp(denm->denm.management.referenceTime));
}

TEST(DENBasicService, SendNewDENM) {
  DBElement<DENMWrapper> elmt;
  auto db = getDB();
  auto nal = getNAL();
  auto service = getService(nal, db);
  auto eventType = EventType_CollisionRisk;
  EXPECT_CALL(*db, insert(An<DENMWrapper>()))
      .WillOnce(Invoke([&elmt](DENMWrapper entry) {
        elmt.setData(std::make_unique<DENMWrapper>(entry));
        DBView<DENMWrapper> view(&elmt);
        return view;
      }));
  EXPECT_CALL(*nal, send(_)).Times(1);

  service->createDENM(&eventType);
}