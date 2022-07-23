#include <DBElement.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

struct Helper {
  int _value;
};

TEST(Database_DBElement, Test_Element_Get_View) {
  int *x = new int(rand());
  DBElement<int> element(x);
  DBView<int> view = element.getView();

  ASSERT_EQ(*x, *view);
}

TEST(Database_DBElement, Test_Modify_Element_By_Ref) {
  int *x = new int(rand());
  int xBase = *x;
  DBElement<int> element(x);
  DBView<int> view = element.getView();
  *&view += 1;

  ASSERT_EQ(xBase + 1, *view);
}

TEST(Database_DBElement, Test_Modify_Element_By_Arrow) {
  Helper *x = new Helper();
  int xBase = rand();
  x->_value = xBase;
  DBElement<Helper> element(x);
  DBView<Helper> view = element.getView();
  view->_value += 1;

  ASSERT_EQ(xBase + 1, view->_value);
}

TEST(Database_DBElement, Test_Modify_Read_Only_Element) {
  Helper *x = new Helper();
  DBElement<Helper> element(x);
  DBView<Helper> view = element.getView();
  view->_value = 1;
  Helper y = *view;
  y._value = 2;

  ASSERT_EQ(1, view->_value);
  ASSERT_EQ(2, y._value);
}

TEST(Database_DBElement, Test_Modification_Status_By_Ref) {
  int *x = new int(rand());
  DBElement<int> element(x);
  DBView<int> view = element.getView();
  &view;

  ASSERT_TRUE(view.accessed());
}

TEST(Database_DBElement, Test_Modification_Status_By_Arrow) {
  Helper *x = new Helper();
  DBElement<Helper> element(x);
  DBView<Helper> view = element.getView();
  view->_value;

  ASSERT_TRUE(view.accessed());
}

TEST(Database_DBElement, Test_Get_Concurrent_Views) {
  int *x = new int(rand());
  DBElement<int> element(x);
  { DBView<int> view = element.getView(); }
  { DBView<int> view = element.getView(); }
}

TEST(Database_DBElement, Test_Modification_Callback) {
  int *x = new int(rand());
  int xBase = *x;
  int valueInCallback;
  bool callbackReached = false;
  DBElement<int> element(x);
  element.modifiedCallback = [&](DBElement<int> *elmt) {
    valueInCallback = *elmt->value();
    callbackReached = true;
  };
  {
    DBView<int> view = element.getView();
    *&view += 1;
  }

  ASSERT_TRUE(callbackReached);
  ASSERT_EQ(valueInCallback, xBase + 1);
}

TEST(Database_DBElement, Test_No_Modification_Callback_Without_Access) {
  int *x = new int(rand());
  bool callbackReached = false;
  DBElement<int> element(x);
  element.modifiedCallback = [&](DBElement<int> *elmt) {
    callbackReached = true;
  };
  { DBView<int> view = element.getView(); }

  ASSERT_FALSE(callbackReached);
}

TEST(Database_DBElement, Test_Modified_Flag_After_Callback) {
  int *x = new int(rand());
  bool modifiedInCallback = false;
  bool modifiedAfterCallback = true;
  DBElement<int> element(x);
  element.modifiedCallback = [&](DBElement<int> *elmt) {
    modifiedInCallback = elmt->modified();
  };
  {
    DBView<int> view = element.getView();
    &view;
  }
  modifiedAfterCallback = element.modified();

  ASSERT_TRUE(modifiedInCallback);
  ASSERT_FALSE(modifiedAfterCallback);
}

TEST(Database_DBElement, Test_Modified_Flag_Without_Callback) {
  int *x = new int(rand());
  DBElement<int> element(x);
  {
    DBView<int> view = element.getView();
    &view;
  }

  ASSERT_TRUE(element.modified());
}

TEST(Database_DBElement, Test_Clear_Modified_Flag) {
  int *x = new int(rand());
  DBElement<int> element(x);
  {
    DBView<int> view = element.getView();
    &view;
  }
  element.clearModifiedFlag();

  ASSERT_FALSE(element.modified());
}

TEST(Database_DBElement, Test_Fail_On_Open_Multiple_Views) {
  int *x = new int(rand());
  DBElement<int> element(x);
  DBView<int> view = element.getView();
  bool threwException = false;

  try {
    view = element.getView();
  } catch (const DBException &) {
    threwException = true;
  }

  ASSERT_TRUE(threwException);
}

#ifndef ENA_SINGLE_VIEW
TEST(Database_DBElement, Test_Open_Multiple_Views) {
  int *x1 = new int(rand());
  int *x2 = new int(rand());
  DBElement<int> element1(x1);
  DBElement<int> element2(x2);
  DBView<int> view1 = element1.getView();
  DBView<int> view2 = element2.getView();

  ASSERT_EQ(*x1, *view1);
  ASSERT_EQ(*x2, *view2);
}

TEST(Database_DBElement, Test_Move_Assignment_For_View) {
  int *x1 = new int(rand());
  int *x2 = new int(rand());
  DBElement<int> element1(x1);
  DBElement<int> element2(x2);
  DBView<int> view2 = element1.getView();
  {
    DBView<int> tmpView = element2.getView();
    view2 = std::move(tmpView);
  }
  DBView<int> view1 = element1.getView();
  *&view2 += 1;

  ASSERT_EQ(*x1, *view1);
  ASSERT_EQ(*x2, *view2);
}
#endif

TEST(Database_DBElement, Test_Correct_Destruction) {
  Helper *helper = new Helper();
  { DBElement<Helper> element(helper); }

  // Should not leak memory (valgrind test)
}