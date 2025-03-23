# CMake generated Testfile for 
# Source directory: C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests
# Build directory: C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(MyTests "C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/build/Debug/my_tests.exe")
  set_tests_properties(MyTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/CMakeLists.txt;26;add_test;C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(MyTests "C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/build/Release/my_tests.exe")
  set_tests_properties(MyTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/CMakeLists.txt;26;add_test;C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(MyTests "C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/build/MinSizeRel/my_tests.exe")
  set_tests_properties(MyTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/CMakeLists.txt;26;add_test;C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(MyTests "C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/build/RelWithDebInfo/my_tests.exe")
  set_tests_properties(MyTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/CMakeLists.txt;26;add_test;C:/Users/trent/Documents/Spring 2025/CPTS 471/PA2/Tests/CMakeLists.txt;0;")
else()
  add_test(MyTests NOT_AVAILABLE)
endif()
subdirs("googletest")
