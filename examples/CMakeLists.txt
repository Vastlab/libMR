#add_executable(wbl-test  wbl-test.c)
add_executable(mr-test  mr-test.cpp)
add_executable(wbl-test  wbl-test.c)


target_link_libraries(wbl-test MR)
target_link_libraries(mr-test MR)


install(TARGETS mr-test wbl-test
  # IMPORTANT: Add the wlb-test and mr-test executable to the "export-set"
  EXPORT LibMRLibraryDepends
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
