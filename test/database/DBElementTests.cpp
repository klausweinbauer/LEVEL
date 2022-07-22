#include <DBElement.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

struct Helper {
  int _value;
};

TEST(Database_DBElement, Test_Element_Get_View) {
  int x = rand();
  DBElement<int> element(&x);
  DBView<int> view = element.getView();

  ASSERT_EQ(x, *view);
}

TEST(Database_DBElement, Test_Modify_Element_By_Ref) {
  int xBase = rand();
  int x = xBase;
  DBElement<int> element(&x);
  DBView<int> view = element.getView();
  *&view += 1;

  ASSERT_EQ(xBase + 1, *view);
}

TEST(Database_DBElement, Test_Modify_Element_By_Arrow) {
  int xBase = rand();
  Helper x;
  x._value = xBase;
  DBElement<Helper> element(&x);
  DBView<Helper> view = element.getView();
  view->_value += 1;

  ASSERT_EQ(xBase + 1, view->_value);
}

TEST(Database_DBElement, Test_Modification_Status_By_Ref) {
  int x = rand();
  DBElement<int> element(&x);
  DBView<int> view = element.getView();
  &view;

  ASSERT_TRUE(view.accessed());
}

TEST(Database_DBElement, Test_Modification_Status_By_Arrow) {
  Helper x;
  DBElement<Helper> element(&x);
  DBView<Helper> view = element.getView();
  view->_value;

  ASSERT_TRUE(view.accessed());
}

TEST(Database_DBElement, Test_Get_Concurrent_Views) {
  int x;
  DBElement<int> element(&x);
  { DBView<int> view = element.getView(); }
  { DBView<int> view = element.getView(); }
}

TEST(Database_DBElement, Test_Modification_Callback) {
  int xBase = rand();
  int x = xBase;
  int valueInCallback;
  bool callbackReached = false;
  DBElement<int> element(&x);
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
  int x = rand();
  bool callbackReached = false;
  DBElement<int> element(&x);
  element.modifiedCallback = [&](DBElement<int> *elmt) {
    callbackReached = true;
  };
  { DBView<int> view = element.getView(); }

  ASSERT_FALSE(callbackReached);
}

TEST(Database_DBElement, Test_Modified_Flag_After_Callback) {
  int x = rand();
  bool modifiedInCallback = false;
  bool modifiedAfterCallback = true;
  DBElement<int> element(&x);
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
  int x = rand();
  DBElement<int> element(&x);
  {
    DBView<int> view = element.getView();
    &view;
  }

  ASSERT_TRUE(element.modified());
}

TEST(Database_DBElement, Test_Clear_Modified_Flag) {
  int x = rand();
  DBElement<int> element(&x);
  {
    DBView<int> view = element.getView();
    &view;
  }
  element.clearModifiedFlag();

  ASSERT_FALSE(element.modified());
}

TEST(Database_DBElement, Test_Fail_On_Open_Multiple_Views) {
  int x = rand();
  DBElement<int> element(&x);
  DBView<int> view = element.getView();
  bool threwException = false;

  try {
    view = element.getView();
  } catch (const DBException &) {
    threwException = true;
  }

  ASSERT_TRUE(threwException);
}