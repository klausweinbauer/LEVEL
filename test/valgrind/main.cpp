// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

#include <iostream>

#ifdef __linux__

#include <algorithm>
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

// TODO improve reading; read blockwise to prevent crashes

using namespace std;
namespace fs = filesystem;

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

#define VALGRIND_EXECUTABLE valgrind
#define STILL_REACHABLE_BYTES_BY_GTEST 160

string getPath(string path, string file) { return path + "/" + file; }
string getLogFile(string path, string file, string hash) {
  return path + "/" + file + "." + hash + ".log";
}

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

vector<char> readBinary(string filePath) {
  streampos fileSize;
  ifstream file(filePath, ios::binary);

  file.seekg(0, ios::end);
  fileSize = file.tellg();
  file.seekg(0, ios::beg);

  vector<char> fileData(fileSize);
  file.read((char *)&fileData[0], fileSize);
  return fileData;
}

unsigned long long computeHash(const vector<char> &bytes) {
  unsigned long long hash;
  for (auto b : bytes) {
    // Hash function: sdbm
    hash = b + (hash << 6) + (hash << 16) - hash;
  }
  return hash;
}

string computeStringHash(const vector<char> &bytes) {
  stringstream ss;
  unsigned long long hash = computeHash(bytes);
  ss << setfill('0') << setw(sizeof(unsigned long long) * 2) << hex << hash;
  return ss.str();
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

int runTarget(string path, string file, string hash) {
  string targetPath = getPath(path, file);
  string logFilePath = getLogFile(path, file, hash);
  stringstream ssCmd;
  ssCmd << TO_STRING(VALGRIND_EXECUTABLE)
        << " --leak-check=full --show-leak-kinds=all --track-origins=yes "
           "--verbose --log-file="
        << logFilePath << " " << targetPath;
  return system(ssCmd.str().c_str());
}

bool validContent(const string &content) {
  // TODO Regex should operate on chunks to prevent seg fault
  // Valid output should be smaller than ~32kB
  if (content.size() > 32000) {
    return false;
  }
  regex rLeaks(".+in use at exit: ([0-9,]+)");
  smatch mLeaks;
  return regex_search(content, mLeaks, rLeaks);
}

string removeChar(string s, char c) {
  s.erase(remove(s.begin(), s.end(), c), s.end());
  return s;
}

bool hasLeaks(string *content) {
  // TODO Regex should operate on chunks to prevent seg fault
  regex rLeaks(".+in use at exit: ([0-9,]+)");
  smatch mLeaks;
  assert(true == regex_search(*content, mLeaks, rLeaks));
  return stoi(removeChar(mLeaks.str(1), ','));
}

bool ensureContent(mutex *m, string *content, string *path, string *name,
                   bool *initialized) {
  unique_lock<mutex> lock(*m);
  if (!*initialized) {
    string targetPath = getPath(*path, *name);
    auto bytes = readBinary(targetPath);
    auto hash = computeStringHash(bytes);

    bool foundFile = false;
    for (const auto &entry : fs::directory_iterator(*path)) {
      regex rHash(".+\\/" + *name + "\\.([0-9a-fA-F]{16})\\.log");
      smatch mHash;
      string entryPath = entry.path();
      if (regex_search(entryPath, mHash, rHash)) {
        string logHash = mHash.str(1);
        if (logHash == hash) {
          foundFile = true;
          break;
        }
      }
    }

    if (foundFile) {
      *content = readContent(getLogFile(*path, *name, hash));
    } else {
      int executionResult = runTarget(*path, *name, hash);
      if (executionResult != 0) {
        return false;
      }
      *content = readContent(getLogFile(*path, *name, hash));
    }

    *initialized = true;
    return validContent(*content);
  }
}

void testMemoryLeak(mutex *m, string *content, string *path, string *name,
                    bool *initialized) {
  ASSERT_TRUE(ensureContent(m, content, path, name, initialized));
  if (hasLeaks(content)) {
    // TODO Regex should operate on chunks to prevent seg fault
    regex rDefinitelyLost(".+definitely lost: ([0-9,]+)");
    regex rIndirectlyLost(".+indirectly lost: ([0-9,]+)");
    smatch mDefinitelyLost;
    smatch mIndirectlyLost;
    ASSERT_TRUE(regex_search(*content, mDefinitelyLost, rDefinitelyLost));
    ASSERT_TRUE(regex_search(*content, mIndirectlyLost, rIndirectlyLost));
    int definitelyLost = stoi(removeChar(mDefinitelyLost.str(1), ','));
    int indirectlyLost = stoi(removeChar(mIndirectlyLost.str(1), ','));
    ASSERT_EQ(0, definitelyLost);
    ASSERT_EQ(0, indirectlyLost);
  }
}

void testPossiblyLostMemory(mutex *m, string *content, string *path,
                            string *name, bool *initialized) {
  ASSERT_TRUE(ensureContent(m, content, path, name, initialized));
  if (hasLeaks(content)) {
    // TODO Regex should operate on chunks to prevent seg fault
    regex rPossiblyLost(".+possibly lost: ([0-9,]+)");
    smatch mPossiblyLost;
    ASSERT_TRUE(regex_search(*content, mPossiblyLost, rPossiblyLost));
    int possiblyLost = stoi(removeChar(mPossiblyLost.str(1), ','));
    ASSERT_EQ(0, possiblyLost);
  }
}

void testStillReachableMemory(mutex *m, string *content, string *path,
                              string *name, bool *initialized) {
  ASSERT_TRUE(ensureContent(m, content, path, name, initialized));
  if (hasLeaks(content)) {
    // TODO Regex should operate on chunks to prevent seg fault
    regex rStillReachable(".+still reachable: ([0-9,]+)");
    smatch mStillReachable;
    ASSERT_TRUE(regex_search(*content, mStillReachable, rStillReachable));
    int stillReachable = stoi(removeChar(mStillReachable.str(1), ','));
    stillReachable -= STILL_REACHABLE_BYTES_BY_GTEST;
    assert(stillReachable >= 0 && "You fixed the leak by gtest? Nice work! You "
                                  "can now delete this assert. Thank you!");
    ASSERT_EQ(0, stillReachable);
  }
}

void testSuppressed(mutex *m, string *content, string *path, string *name,
                    bool *initialized) {
  ASSERT_TRUE(ensureContent(m, content, path, name, initialized));
  if (hasLeaks(content)) {
    // TODO Regex should operate on chunks to prevent seg fault
    regex rSuppressed(".+suppressed: ([0-9,]+)");
    smatch mSuppressed;
    ASSERT_TRUE(regex_search(*content, mSuppressed, rSuppressed));
    int suppressed = stoi(removeChar(mSuppressed.str(1), ','));
    ASSERT_EQ(0, suppressed);
  }
}

void testErrors(mutex *m, string *content, string *path, string *name,
                bool *initialized) {
  ASSERT_TRUE(ensureContent(m, content, path, name, initialized));
  if (hasLeaks(content)) {
    // TODO Regex should operate on chunks to prevent seg fault
    regex rErrors(".+ERROR SUMMARY: ([0-9,]+)");
    smatch mErrors;
    ASSERT_TRUE(regex_search(*content, mErrors, rErrors));
    int errors = stoi(removeChar(mErrors.str(1), ','));
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