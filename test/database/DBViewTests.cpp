#include <DBElement.hpp>
#include <DBView.hpp>
#include <Mocks.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

using ::testing::NiceMock;

// DBView(DBElement<T> *element, DBElementStatus *status)
// virtual ~DBView()
// DBView(DBView<T> &&view) : _element(nullptr), _status(nullptr)
// DBView<T> &operator=(DBView<T> &&view)
// T &operator->()
// T &operator*()

TEST(DBView, Initialize_With_Element_Nullptr) {
  DBElementStatus status;
  ASSERT_THROW(DBView<int> view(nullptr, &status), Exception);
}

TEST(DBView, Initialize_With_Status_Nullptr) {
  DBElement<int> element(0);
  ASSERT_THROW(DBView<int> view(&element, nullptr), Exception);
}

TEST(DBView, Lock_Element_During_Construction) {
  MDBElement<int> element;
  auto *status = new DBElementStatus();
  EXPECT_CALL(element, lock()).Times(1);
  EXPECT_CALL(element, unlock());
  DBView<int> view(&element, status);
}

TEST(DBView, Unlock_Element_At_End_Of_Life) {
  MDBElement<int> element;
  auto *status = new DBElementStatus();
  EXPECT_CALL(element, lock());
  EXPECT_CALL(element, unlock()).Times(1);
  { DBView<int> view(&element, status); }
}

TEST(DBView, Do_Not_Unlock_Element_At_End_Of_Life_When_Deleted) {
  MDBElement<int> element;
  auto *status = new DBElementStatus();
  status->_deleted = true;
  EXPECT_CALL(element, lock());
  EXPECT_CALL(element, unlock()).Times(0);
  { DBView<int> view(&element, status); }
}

TEST(DBView, Move_Construction) {
  MDBElement<int> element;
  auto *status = new DBElementStatus();
  EXPECT_CALL(element, lock());
  EXPECT_CALL(element, unlock());
  DBView<int> view1(&element, status);
  DBView<int> view2(std::move(view1));
}