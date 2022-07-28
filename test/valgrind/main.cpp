#include <iostream>

#ifdef __linux__

#include <chrono>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <mutex>
#include <regex>
#include <thread>
#include <time.h>

using namespace std;
namespace fs = filesystem;

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

#define VALGRIND_EXECUTABLE valgrind
#define STILL_REACHABLE_BYTES_BY_GTEST 160

string getPath(string path, string file) { return path + "/" + file; }
string getLogFile(string file) { return "./" + file + ".log"; }

void removeFile(string filePath) {
  fs::path path(filePath);
  if (fs::exists(path)) {
    fs::remove(path);
  }
}

string readContent(string filePath) {
  fstream fileStream;
  string content;
  fileStream.open(filePath, ios::in);
  if (fileStream.is_open()) {
    string line;
    while (getline(fileStream, line)) {
      content += line;
    }
  } else {
    throw runtime_error("Could not open file " + filePath + ".");
  }
  fileStream.close();

  return content;
}

string waitForFileContent(string filePath) {
  fs::path path(filePath);
  while (!fs::exists(path)) {
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  return readContent(filePath);
}

string getVersion() {
  string versionFilePath("./valgrind-version.txt");
  stringstream ssCmd;
  ssCmd << TO_STRING(VALGRIND_EXECUTABLE) << " --version > " << versionFilePath;
  removeFile(versionFilePath);
  system(ssCmd.str().c_str());
  auto content = readContent(versionFilePath);
  return regex_replace(content, std::regex("^valgrind-"), "");
}

int runTarget(string path, string file) {
  string targetPath = getPath(path, file);
  string logFilePath = getLogFile(file);
  stringstream ssCmd;
  ssCmd << TO_STRING(VALGRIND_EXECUTABLE)
        << " --leak-check=full --show-leak-kinds=all --track-origins=yes "
           "--verbose --log-file="
        << logFilePath << " " << targetPath;
  return system(ssCmd.str().c_str());
}

void ensureContent(mutex *m, string *content, string *path, string *name,
                   bool *initialized) {
  unique_lock<mutex> lock(*m);
  if (!*initialized) {
    int executionResult = runTarget(*path, *name);
    ASSERT_EQ(0, executionResult);
    *content = readContent(getLogFile(*name));
    *initialized = true;
  }
}

bool hasLeaks(string *content) {
  regex rLeaks(".+in use at exit: ([0-9]+)");
  smatch mLeaks;
  assert(true == regex_search(*content, mLeaks, rLeaks));
  return stoi(mLeaks.str(1));
}

void testMemoryLeak(mutex *m, string *content, string *path, string *name,
                    bool *initialized) {
  ensureContent(m, content, path, name, initialized);
  if (hasLeaks(content)) {
    regex rDefinitelyLost(".+definitely lost: ([0-9]+)");
    regex rIndirectlyLost(".+indirectly lost: ([0-9]+)");
    smatch mDefinitelyLost;
    smatch mIndirectlyLost;
    ASSERT_TRUE(regex_search(*content, mDefinitelyLost, rDefinitelyLost));
    ASSERT_TRUE(regex_search(*content, mIndirectlyLost, rIndirectlyLost));
    int definitelyLost = stoi(mDefinitelyLost.str(1));
    int indirectlyLost = stoi(mIndirectlyLost.str(1));
    ASSERT_EQ(0, definitelyLost);
    ASSERT_EQ(0, indirectlyLost);
  }
}

void testPossiblyLostMemory(mutex *m, string *content, string *path,
                            string *name, bool *initialized) {
  ensureContent(m, content, path, name, initialized);
  if (hasLeaks(content)) {
    regex rPossiblyLost(".+possibly lost: ([0-9]+)");
    smatch mPossiblyLost;
    ASSERT_TRUE(regex_search(*content, mPossiblyLost, rPossiblyLost));
    int possiblyLost = stoi(mPossiblyLost.str(1));
    ASSERT_EQ(0, possiblyLost);
  }
}

void testStillReachableMemory(mutex *m, string *content, string *path,
                              string *name, bool *initialized) {
  ensureContent(m, content, path, name, initialized);
  if (hasLeaks(content)) {
    regex rStillReachable(".+still reachable: ([0-9]+)");
    smatch mStillReachable;
    ASSERT_TRUE(regex_search(*content, mStillReachable, rStillReachable));
    int stillReachable = stoi(mStillReachable.str(1));
    stillReachable -= STILL_REACHABLE_BYTES_BY_GTEST;
    assert(stillReachable >= 0 && "You fixed the leak by gtest? Nice work! You "
                                  "can now delete this assert. Thank you!");
    ASSERT_EQ(0, stillReachable);
  }
}

void testSuppressed(mutex *m, string *content, string *path, string *name,
                    bool *initialized) {
  ensureContent(m, content, path, name, initialized);
  if (hasLeaks(content)) {
    regex rSuppressed(".+suppressed: ([0-9]+)");
    smatch mSuppressed;
    ASSERT_TRUE(regex_search(*content, mSuppressed, rSuppressed));
    int suppressed = stoi(mSuppressed.str(1));
    ASSERT_EQ(0, suppressed);
  }
}

void testErrors(mutex *m, string *content, string *path, string *name,
                bool *initialized) {
  ensureContent(m, content, path, name, initialized);
  if (hasLeaks(content)) {
    regex rErrors(".+ERROR SUMMARY: ([0-9]+)");
    smatch mErrors;
    ASSERT_TRUE(regex_search(*content, mErrors, rErrors));
    int errors = stoi(mErrors.str(1));
    ASSERT_EQ(0, errors);
  }
}

TEST(SetupTest, ValgrindInstalledAndFound) {
  auto versionString = getVersion();
  ASSERT_TRUE(versionString.length() >= 5);
}

#define DEFINE_TESTS(target)                                                   \
  static mutex target##_Mutex;                                                 \
  static string target##_Content;                                              \
  static string target##_Path(TO_STRING(target##_PATH));                       \
  static string target##_Name(TO_STRING(target));                              \
  static bool target##_Initialized = false;                                    \
  TEST(target, NoMemoryLeaks) {                                                \
    testMemoryLeak(&target##_Mutex, &target##_Content, &target##_Path,         \
                   &target##_Name, &target##_Initialized);                     \
  }                                                                            \
  TEST(target, NoPossiblyLostMemory) {                                         \
    testPossiblyLostMemory(&target##_Mutex, &target##_Content, &target##_Path, \
                           &target##_Name, &target##_Initialized);             \
  }                                                                            \
  TEST(target, NoStillReachablePointers) {                                     \
    testStillReachableMemory(&target##_Mutex, &target##_Content,               \
                             &target##_Path, &target##_Name,                   \
                             &target##_Initialized);                           \
  }                                                                            \
  TEST(target, NothingSuppressed) {                                            \
    testSuppressed(&target##_Mutex, &target##_Content, &target##_Path,         \
                   &target##_Name, &target##_Initialized);                     \
  }                                                                            \
  TEST(target, NoErrors) {                                                     \
    testErrors(&target##_Mutex, &target##_Content, &target##_Path,             \
               &target##_Name, &target##_Initialized);                         \
  }

// Unit test target definition
#ifdef UNIT_TEST_TARGET
DEFINE_TESTS(UNIT_TEST_TARGET)
#endif

// Integration test target definition
#ifdef INTEGRATION_TEST_TARGET
DEFINE_TESTS(INTEGRATION_TEST_TARGET)
#endif

int main(int argc, char **argv) {
  srand(time(0));

  ::testing::InitGoogleTest(&argc, argv);

  int ret = RUN_ALL_TESTS();
  return ret;
}

#else

int main(int argc, char **argv) {

  cout << "Valgrind tests are only supported on linux." << endl;
  return 0;
}

#endif